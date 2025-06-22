/**
 * File: thread-pool.cc
 * --------------------
 * Presents the implementation of the ThreadPool class.
 */

#include "thread-pool.h"
#include <chrono>
#include <stdexcept>
using namespace std;

ThreadPool::ThreadPool(size_t numThreads) : wts(numThreads), done(false), 
    pendingTasks(0), allWorkersAvailable(0) {
    
    for (size_t i = 0; i < numThreads; i++) {
        wts[i].available = true;
        wts[i].hasWork = false;
        wts[i].workReady = unique_ptr<Semaphore>(new Semaphore(0));
        wts[i].ts = thread([this, i] { worker(i); });
    }
    
    dt = thread([this] { dispatcher(); });
}


void ThreadPool::schedule(const function<void(void)>& thunk) {
    if (!thunk) {
        throw invalid_argument("Cannot schedule nullptr function");
    }
    
    {
        lock_guard<mutex> lg(queueLock);
        if (!done) {
            tasks.push(thunk);
            pendingTasks.signal();
        }
    }
}

void ThreadPool::wait() {
    while (true) {
        bool allIdle = true;
        {
            lock_guard<mutex> queueLock_guard(queueLock);
            lock_guard<mutex> workerLock_guard(workerLock);
            
            if (!tasks.empty()) {
                allIdle = false;
            } else {
                for (size_t i = 0; i < wts.size(); i++) {
                    if (!wts[i].available || wts[i].hasWork) {
                        allIdle = false;
                        break;
                    }
                }
            }
        }
        
        if (allIdle) {
            break;
        }
        
        this_thread::sleep_for(chrono::microseconds(100));
    }
}

ThreadPool::~ThreadPool() {
    wait();  
    done = true;  
    
    pendingTasks.signal();
    
    for (size_t i = 0; i < wts.size(); i++) {
        wts[i].workReady->signal();
    }
    
    dt.join();
    
    for (size_t i = 0; i < wts.size(); i++) {
        wts[i].ts.join();
    }
}

void ThreadPool::worker(int id) {
    while (true) {
        wts[id].workReady->wait();
        
        if (done) {
            break;
        }
        
        function<void(void)> localTask = wts[id].thunk;
        
        if (localTask) {
            try {
                localTask();
            } catch (...) {
                // Expecion de usuario
            }
        }
        
        {
            lock_guard<mutex> lg(workerLock);
            wts[id].available = true;
            wts[id].hasWork = false;
        }
    }
}

void ThreadPool::dispatcher() {
    while (true) {
        
        if (done) {
            break;
        }
        
        function<void(void)> task;
        {
            lock_guard<mutex> lg(queueLock);
            if (!tasks.empty()) {
                task = tasks.front();
                tasks.pop();
            } else {
                continue; 
            }
        }
        
        int selectedWorker = -1;
        while (selectedWorker == -1 && !done) {
            {
                lock_guard<mutex> lg(workerLock);
                for (size_t i = 0; i < wts.size(); i++) {
                    if (wts[i].available && !wts[i].hasWork) {
                        selectedWorker = i;
                        wts[i].hasWork = true;
                        wts[i].available = false; 
                        break; 
                    }
                }
            }
            
            if (selectedWorker == -1) {
                this_thread::sleep_for(chrono::microseconds(10));
            }
        }
        
        if (selectedWorker != -1 && !done) {
            wts[selectedWorker].thunk = task;
            wts[selectedWorker].workReady->signal();
        }
    }
}
