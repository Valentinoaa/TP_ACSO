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
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <future>
#include <algorithm>
#include <random>
#include <stdexcept>

#include "thread-pool.h"

using namespace std;
using namespace chrono;

void sleep_for(int slp){
    this_thread::sleep_for(chrono::milliseconds(slp));
}

void delay_ms(int ms) {
    this_thread::sleep_for(milliseconds(ms));
}

static mutex oslock;

atomic<int> tests_passed(0);
atomic<int> tests_failed(0);
mutex output_mutex;

void report_test(const string& test_name, bool passed) {
    lock_guard<mutex> lg(output_mutex);
    if (passed) {
        cout << "[PASS] " << test_name << endl;
        tests_passed++;
    } else {
        cout << "[FAIL] " << test_name << endl;
        tests_failed++;
    }
}

// ===== TESTS ORIGINALES =====
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

// ===== TESTS CUSTOM=====

static void basicSchedulingTest() {
    ThreadPool pool(3);
    vector<int> results(5, 0);
    
    for (int i = 0; i < 5; i++) {
        pool.schedule([&results, i]() {
            results[i] = (i + 1) * 10;
        });
    }
    
    pool.wait();
    
    vector<int> expected = {10, 20, 30, 40, 50};
    bool passed = (results == expected);
    
    oslock.lock();
    cout << "Basic Scheduling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Expected: ";
        for (int v : expected) cout << v << " ";
        cout << "\nGot: ";
        for (int v : results) cout << v << " ";
        cout << endl;
    }
    oslock.unlock();
}

static void sequentialTimingTest() {
    ThreadPool pool(1);
    vector<int> execution_order;
    mutex order_mutex;
    
    for (int i = 0; i < 7; i++) {
        pool.schedule([&execution_order, &order_mutex, i]() {
            delay_ms(10);
            lock_guard<mutex> lg(order_mutex);
            execution_order.push_back(i);
        });
    }
    
    pool.wait();
    
    bool passed = true;
    for (size_t i = 0; i < execution_order.size(); i++) {
        if (execution_order[i] != static_cast<int>(i)) {
            passed = false;
            break;
        }
    }
    passed = passed && execution_order.size() == 7;
    
    oslock.lock();
    cout << "Sequential Timing Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Execution order: ";
        for (int v : execution_order) cout << v << " ";
        cout << endl;
    }
    oslock.unlock();
}

static void atomicStressTest() {
    const int num_tasks = 5000;
    ThreadPool pool(8);
    atomic<int> counter(0);
    atomic<int> sum(0);
    
    for (int i = 0; i < num_tasks; i++) {
        pool.schedule([&counter, &sum, i]() {
            counter.fetch_add(1);
            sum.fetch_add(i);
        });
    }
    
    pool.wait();
    
    int expected_sum = (num_tasks - 1) * num_tasks / 2;
    bool passed = (counter.load() == num_tasks && sum.load() == expected_sum);
    
    oslock.lock();
    cout << "Atomic Stress Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Counter: " << counter.load() << " (expected " << num_tasks << ")" << endl;
        cout << "Sum: " << sum.load() << " (expected " << expected_sum << ")" << endl;
    }
    oslock.unlock();
}

static void multipleWaitsTest() {
    ThreadPool pool(2);
    atomic<int> task_count(0);
    
    for (int i = 0; i < 3; i++) {
        pool.schedule([&task_count]() {
            delay_ms(30);
            task_count++;
        });
    }
    
    pool.wait();
    if (task_count.load() != 3) {
        oslock.lock();
        cout << "Multiple Waits Test: FAILED - First batch" << endl;
        oslock.unlock();
        return;
    }
    
    auto start = steady_clock::now();
    pool.wait();
    auto end = steady_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    
    if (task_count.load() != 3 || duration > 50) {
        oslock.lock();
        cout << "Multiple Waits Test: FAILED - Second wait took " << duration << "ms" << endl;
        oslock.unlock();
        return;
    }
    
    for (int i = 0; i < 2; i++) {
        pool.schedule([&task_count]() {
            task_count++;
        });
    }
    
    pool.wait();
    bool passed = (task_count.load() == 5);
    
    oslock.lock();
    cout << "Multiple Waits Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Task count: " << task_count.load() << " (expected 5)" << endl;
    }
    oslock.unlock();
}

static void concurrentSchedulingTest() {
    ThreadPool pool(4);
    atomic<int> task_counter(0);
    const int tasks_per_thread = 100;
    const int num_scheduler_threads = 3;
    
    vector<thread> schedulers;
    
    for (int t = 0; t < num_scheduler_threads; t++) {
        schedulers.emplace_back([&pool, &task_counter, tasks_per_thread]() {
            for (int i = 0; i < tasks_per_thread; i++) {
                pool.schedule([&task_counter]() {
                    task_counter.fetch_add(1, memory_order_relaxed);
                });
            }
        });
    }
    
    for (auto& t : schedulers) {
        t.join();
    }
    
    pool.wait();
    
    bool passed = (task_counter.load() == (tasks_per_thread * num_scheduler_threads));
    
    oslock.lock();
    cout << "Concurrent Scheduling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Task counter: " << task_counter.load() << " (expected " << (tasks_per_thread * num_scheduler_threads) << ")" << endl;
    }
    oslock.unlock();
}

static void nestedSchedulingTest() {
    ThreadPool pool(3);
    atomic<int> leaf_tasks(0);
    
    pool.schedule([&pool, &leaf_tasks]() {
        delay_ms(10);
        pool.schedule([&pool, &leaf_tasks]() {
            delay_ms(10);
            pool.schedule([&leaf_tasks]() {
                leaf_tasks++;
            });
        });
    });
    
    pool.wait();
    bool passed = (leaf_tasks.load() == 1);
    
    oslock.lock();
    cout << "Nested Scheduling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Leaf tasks: " << leaf_tasks.load() << " (expected 1)" << endl;
    }
    oslock.unlock();
}

static void exceptionHandlingTest() {
    ThreadPool pool(2);
    atomic<int> good_tasks(0);
    atomic<int> total_tasks(0);
    atomic<int> exception_count(0);
    
    for (int i = 0; i < 10; i++) {
        pool.schedule([&good_tasks, &total_tasks, &exception_count, i]() {
            total_tasks++;
            try {
                if (i == 5) {
                    throw runtime_error("Test exception");
                }
                good_tasks++;
            } catch (const runtime_error&) {
                exception_count++;
            }
        });
    }
    
    pool.wait();
    
    bool passed = (total_tasks.load() == 10 && 
                   good_tasks.load() == 9 && 
                   exception_count.load() == 1);
    
    oslock.lock();
    cout << "Exception Handling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Total: " << total_tasks.load() << ", Good: " << good_tasks.load() << ", Exceptions: " << exception_count.load() << endl;
    }
    oslock.unlock();
}

static void performanceBenchmarkTest() {
    const int num_tasks = 10000;
    ThreadPool pool(6);
    
    auto start = high_resolution_clock::now();
    
    atomic<int> completed(0);
    for (int i = 0; i < num_tasks; i++) {
        pool.schedule([&completed]() {
            completed++;
        });
    }
    
    pool.wait();
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    
    bool passed = (completed.load() == num_tasks && duration < 5000);
    
    oslock.lock();
    cout << "Performance Benchmark Test: " << (passed ? "PASSED" : "FAILED") << endl;
    cout << "Completed " << completed.load() << " tasks in " << duration << "ms" << endl;
    oslock.unlock();
}

static void runAllCustomTests() {
    cout << "=== Running All Custom Tests ===" << endl;
    
    basicSchedulingTest();
    sequentialTimingTest();
    atomicStressTest();
    multipleWaitsTest();
    concurrentSchedulingTest();
    nestedSchedulingTest();
    exceptionHandlingTest();
    performanceBenchmarkTest();
    
    cout << "=== Custom Tests Complete ===" << endl;
}

struct testEntry {
    string flag;
    function<void(void)> testfn;
};

static void buildMap(map<string, function<void(void)>>& testFunctionMap) {
    testEntry entries[] = {
        // Original tests
        {"--single-thread-no-wait", singleThreadNoWaitTest},
        {"--single-thread-single-wait", singleThreadSingleWaitTest},
        {"--no-threads-double-wait", noThreadsDoubleWaitTest},
        {"--reuse-thread-pool", reuseThreadPoolTest},
        {"--s", simpleTest},
        
        // Custom tests
        {"--basic-scheduling", basicSchedulingTest},
        {"--sequential-timing", sequentialTimingTest},
        {"--atomic-stress", atomicStressTest},
        {"--multiple-waits", multipleWaitsTest},
        {"--concurrent-scheduling", concurrentSchedulingTest},
        {"--nested-scheduling", nestedSchedulingTest},
        {"--exception-handling", exceptionHandlingTest},
        {"--performance-benchmark", performanceBenchmarkTest},
        {"--all-custom", runAllCustomTests},
    };

    for (const testEntry& entry: entries) {
        testFunctionMap[entry.flag] = entry.testfn;
    }
}

static void executeAll(const map<string, function<void(void)>>& testFunctionMap) {
    for (const auto& entry: testFunctionMap) {
        cout << entry.first << ":" << endl;
        entry.second();
        cout << endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <test-flag>" << endl;
        cout << "Available flags:" << endl;
        cout << "  --all                    Run all tests" << endl;
        cout << "  --all-custom             Run all custom tests" << endl;
        cout << "  --s                      Simple test" << endl;
        cout << "  --single-thread-no-wait  Single thread no wait test" << endl;
        cout << "  --single-thread-single-wait  Single thread single wait test" << endl;
        cout << "  --no-threads-double-wait No threads double wait test" << endl;
        cout << "  --reuse-thread-pool      Reuse thread pool test" << endl;
        cout << "  --basic-scheduling       Basic scheduling test" << endl;
        cout << "  --sequential-timing      Sequential timing test" << endl;
        cout << "  --atomic-stress          Atomic stress test" << endl;
        cout << "  --multiple-waits         Multiple waits test" << endl;
        cout << "  --concurrent-scheduling  Concurrent scheduling test" << endl;
        cout << "  --nested-scheduling      Nested scheduling test" << endl;
        cout << "  --exception-handling     Exception handling test" << endl;
        cout << "  --performance-benchmark  Performance benchmark test" << endl;
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
        cout << "Use --help to see available options." << endl;
        return 0;
    }

    found->second();
    return 0;
}
