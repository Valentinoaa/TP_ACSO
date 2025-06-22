/**
 * File: tpcustomtest.cc
 * ---------------------
 * Unit tests *you* write to exercise the ThreadPool in a variety
 * of ways.
 */

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <functional>
#include <cstring>
#include <mutex>
#include <sys/types.h> // used to count the number of threads
#include <unistd.h>    // used to count the number of threads
#include <dirent.h>    // for opendir, readdir, closedir

#include "thread-pool.h"


using namespace std;

void sleep_for(int slp){
    this_thread::sleep_for(chrono::milliseconds(slp));
}

static mutex oslock;

static const size_t kNumThreads = 4;
static const size_t kNumFunctions = 10;
static void simpleTest() {
  ThreadPool pool(kNumThreads);
  for (size_t id = 0; id < kNumFunctions; id++) {
    pool.schedule([id] {
      oslock.lock();
      cout << "Thread (ID: " << id << ") has started." << endl;
      oslock.unlock();
      size_t sleepTime = (id % 3) * 10;
      sleep_for(sleepTime);
      oslock.lock();
      cout <<  "Thread (ID: " << id << ") has finished." << endl ;
      oslock.unlock();
    });
  }

  pool.wait();
}


static void singleThreadNoWaitTest() {
    ThreadPool pool(4);

    pool.schedule([&] {
        oslock.lock();
        cout << "This is a test." << endl;
        oslock.unlock();
    });
    sleep_for(1000); // emulate wait without actually calling wait (that's a different test)
}

static void singleThreadSingleWaitTest() {
    ThreadPool pool(4);
    pool.schedule([] {
        oslock.lock();
        cout << "This is a test." << endl;
        oslock.unlock();
        sleep_for(1000);
    });
}

static void noThreadsDoubleWaitTest() {
    ThreadPool pool(4);
    pool.wait();
    pool.wait();
}

static void reuseThreadPoolTest() {
    ThreadPool pool(4);
    for (size_t i = 0; i < 16; i++) {
        pool.schedule([] {
            oslock.lock();
            cout << "This is a test." << endl;
            oslock.unlock();
            sleep_for(50);
        });
    }
    pool.wait();
    pool.schedule([] {
        oslock.lock();
        cout << "This is a code." << endl;
        oslock.unlock();
        sleep_for(1000);
    }); 
    pool.wait();
}

struct testEntry {
    string flag;
    function<void(void)> testfn;
};

static void buildMap(map<string, function<void(void)>>& testFunctionMap) {
    testEntry entries[] = {
        {"--single-thread-no-wait", singleThreadNoWaitTest},
        {"--single-thread-single-wait", singleThreadSingleWaitTest},
        {"--no-threads-double-wait", noThreadsDoubleWaitTest},
        {"--reuse-thread-pool", reuseThreadPoolTest},
        {"--s", simpleTest},
    };

    for (const testEntry& entry: entries) {
        testFunctionMap[entry.flag] = entry.testfn;
    }
}

static void executeAll(const map<string, function<void(void)>>& testFunctionMap) {
    for (const auto& entry: testFunctionMap) {
        cout << entry.first << ":" << endl;
        entry.second();
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Ouch! I need exactly two arguments." << endl;
        return 0;
    }

    map<string, function<void(void)>> testFunctionMap;
    buildMap(testFunctionMap);
    string flag = argv[1];
    if (flag == "--all") {
        executeAll(testFunctionMap);
        return 0;
    }
    auto found = testFunctionMap.find(argv[1]);
    if (found == testFunctionMap.end()) {
        cout << "Oops... we don't recognize the flag \"" << argv[1] << "\"." << endl;
        return 0;
    }

    found->second();
    return 0;
}
