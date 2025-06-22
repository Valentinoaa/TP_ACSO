/**
 * File: thread-pool.cc
 * --------------------
 * Presents the implementation of the ThreadPool class.
 */

#include "thread-pool.h"
using namespace std;

ThreadPool::ThreadPool(size_t numThreads) : wts(numThreads), done(false) {}


void ThreadPool::schedule(const function<void(void)>& thunk) {}

void ThreadPool::wait() {}

ThreadPool::~ThreadPool() {}
