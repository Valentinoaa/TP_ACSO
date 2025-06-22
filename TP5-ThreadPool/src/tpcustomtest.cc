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
#include <iomanip>

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

// Utility function to get current timestamp
string get_timestamp() {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    stringstream ss;
    ss << put_time(localtime(&time_t), "%H:%M:%S");
    ss << "." << setfill('0') << setw(3) << ms.count();
    return ss.str();
}

// Logging function
void log_message(const string& message) {
    lock_guard<mutex> lg(oslock);
    cout << "[" << get_timestamp() << "] " << message << endl;
}

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
  log_message("=== INICIANDO SIMPLE TEST ===");
  log_message("Creando ThreadPool con " + to_string(kNumThreads) + " threads");
  
  ThreadPool pool(kNumThreads);
  log_message("ThreadPool creado exitosamente");
  
  for (size_t id = 0; id < kNumFunctions; id++) {
    log_message("Programando tarea " + to_string(id));
    pool.schedule([id] {
      log_message("Thread (ID: " + to_string(id) + ") ha iniciado");
      size_t sleepTime = (id % 3) * 10;
      if (sleepTime > 0) {
        log_message("Thread (ID: " + to_string(id) + ") durmiendo por " + to_string(sleepTime) + "ms");
      }
      sleep_for(sleepTime);
      log_message("Thread (ID: " + to_string(id) + ") ha terminado");
    });
  }

  log_message("Todas las tareas programadas, llamando a wait()");
  pool.wait();
  log_message("=== SIMPLE TEST COMPLETADO ===");
}

static void singleThreadNoWaitTest() {
    log_message("=== INICIANDO SINGLE THREAD NO WAIT TEST ===");
    ThreadPool pool(4);
    log_message("ThreadPool creado con 4 threads");

    pool.schedule([&] {
        log_message("Ejecutando tarea única");
    });
    log_message("Durmiendo 1000ms sin llamar wait()");
    sleep_for(1000);
    log_message("=== SINGLE THREAD NO WAIT TEST COMPLETADO ===");
}

static void singleThreadSingleWaitTest() {
    log_message("=== INICIANDO SINGLE THREAD SINGLE WAIT TEST ===");
    ThreadPool pool(4);
    log_message("ThreadPool creado con 4 threads");
    
    pool.schedule([] {
        log_message("Tarea iniciada, durmiendo 1000ms");
        sleep_for(1000);
        log_message("Tarea completada");
    });
    
    log_message("Llamando a wait()");
    pool.wait();
    log_message("=== SINGLE THREAD SINGLE WAIT TEST COMPLETADO ===");
}

static void noThreadsDoubleWaitTest() {
    log_message("=== INICIANDO NO THREADS DOUBLE WAIT TEST ===");
    ThreadPool pool(4);
    log_message("ThreadPool creado con 4 threads");
    
    log_message("Primera llamada a wait() (sin tareas)");
    pool.wait();
    log_message("Primera wait() completada");
    
    log_message("Segunda llamada a wait() (sin tareas)");
    pool.wait();
    log_message("Segunda wait() completada");
    log_message("=== NO THREADS DOUBLE WAIT TEST COMPLETADO ===");
}

static void reuseThreadPoolTest() {
    log_message("=== INICIANDO REUSE THREAD POOL TEST ===");
    ThreadPool pool(4);
    log_message("ThreadPool creado con 4 threads");
    
    log_message("Programando primera tanda de 16 tareas");
    for (size_t i = 0; i < 16; i++) {
        pool.schedule([i] {
            log_message("Tarea batch 1, ID: " + to_string(i));
            sleep_for(50);
        });
    }
    
    log_message("Primera tanda programada, llamando wait()");
    pool.wait();
    log_message("Primera tanda completada");
    
    log_message("Programando segunda tarea individual");
    pool.schedule([] {
        log_message("Tarea individual iniciada, durmiendo 1000ms");
        sleep_for(1000);
        log_message("Tarea individual completada");
    }); 
    
    log_message("Segunda tarea programada, llamando wait()");
    pool.wait();
    log_message("=== REUSE THREAD POOL TEST COMPLETADO ===");
}

// ===== TESTS CUSTOM=====

static void basicSchedulingTest() {
    log_message("=== INICIANDO BASIC SCHEDULING TEST ===");
    ThreadPool pool(3);
    vector<int> results(5, 0);
    log_message("ThreadPool creado con 3 threads, vector de resultados inicializado");
    
    for (int i = 0; i < 5; i++) {
        log_message("Programando tarea " + to_string(i));
        pool.schedule([&results, i]() {
            log_message("Ejecutando tarea " + to_string(i));
            results[i] = (i + 1) * 10;
            log_message("Tarea " + to_string(i) + " completada, resultado: " + to_string(results[i]));
        });
    }
    
    log_message("Todas las tareas programadas, llamando wait()");
    pool.wait();
    log_message("Wait() completado");
    
    vector<int> expected = {10, 20, 30, 40, 50};
    bool passed = (results == expected);
    
    oslock.lock();
    log_message("Verificando resultados...");
    cout << "Basic Scheduling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Expected: ";
        for (int v : expected) cout << v << " ";
        cout << "\nGot: ";
        for (int v : results) cout << v << " ";
        cout << endl;
    }
    oslock.unlock();
    log_message("=== BASIC SCHEDULING TEST COMPLETADO ===");
}

static void sequentialTimingTest() {
    log_message("=== INICIANDO SEQUENTIAL TIMING TEST ===");
    ThreadPool pool(1);
    vector<int> execution_order;
    mutex order_mutex;
    log_message("ThreadPool creado con 1 thread (forzar ejecución secuencial)");
    
    for (int i = 0; i < 7; i++) {
        log_message("Programando tarea secuencial " + to_string(i));
        pool.schedule([&execution_order, &order_mutex, i]() {
            log_message("Iniciando tarea secuencial " + to_string(i));
            delay_ms(10);
            lock_guard<mutex> lg(order_mutex);
            execution_order.push_back(i);
            log_message("Tarea secuencial " + to_string(i) + " completada");
        });
    }
    
    log_message("Todas las tareas secuenciales programadas, llamando wait()");
    pool.wait();
    log_message("Wait() completado");
    
    bool passed = true;
    for (size_t i = 0; i < execution_order.size(); i++) {
        if (execution_order[i] != static_cast<int>(i)) {
            passed = false;
            break;
        }
    }
    passed = passed && execution_order.size() == 7;
    
    oslock.lock();
    log_message("Verificando orden de ejecución...");
    cout << "Sequential Timing Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Execution order: ";
        for (int v : execution_order) cout << v << " ";
        cout << endl;
    }
    oslock.unlock();
    log_message("=== SEQUENTIAL TIMING TEST COMPLETADO ===");
}

static void atomicStressTest() {
    log_message("=== INICIANDO ATOMIC STRESS TEST ===");
    const int num_tasks = 5000;
    ThreadPool pool(8);
    atomic<int> counter(0);
    atomic<int> sum(0);
    log_message("ThreadPool creado con 8 threads para test de stress con " + to_string(num_tasks) + " tareas");
    
    auto start_time = steady_clock::now();
    for (int i = 0; i < num_tasks; i++) {
        pool.schedule([&counter, &sum, i]() {
            counter.fetch_add(1);
            sum.fetch_add(i);
        });
    }
    
    log_message("Todas las tareas de stress programadas, llamando wait()");
    pool.wait();
    auto end_time = steady_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time).count();
    log_message("Wait() completado en " + to_string(duration) + "ms");
    
    int expected_sum = (num_tasks - 1) * num_tasks / 2;
    bool passed = (counter.load() == num_tasks && sum.load() == expected_sum);
    
    oslock.lock();
    log_message("Verificando resultados del stress test...");
    cout << "Atomic Stress Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Counter: " << counter.load() << " (expected " << num_tasks << ")" << endl;
        cout << "Sum: " << sum.load() << " (expected " << expected_sum << ")" << endl;
    }
    oslock.unlock();
    log_message("=== ATOMIC STRESS TEST COMPLETADO ===");
}

static void multipleWaitsTest() {
    log_message("=== INICIANDO MULTIPLE WAITS TEST ===");
    ThreadPool pool(2);
    atomic<int> task_count(0);
    log_message("ThreadPool creado con 2 threads");
    
    log_message("Programando primer lote de 3 tareas");
    for (int i = 0; i < 3; i++) {
        pool.schedule([&task_count, i]() {
            log_message("Ejecutando tarea del primer lote " + to_string(i));
            delay_ms(30);
            task_count++;
            log_message("Tarea del primer lote " + to_string(i) + " completada");
        });
    }
    
    log_message("Primer lote programado, llamando wait()");
    pool.wait();
    log_message("Primer wait() completado, task_count: " + to_string(task_count.load()));
    
    if (task_count.load() != 3) {
        oslock.lock();
        cout << "Multiple Waits Test: FAILED - First batch" << endl;
        oslock.unlock();
        return;
    }
    
    log_message("Segundo wait() inmediato (sin nuevas tareas)");
    auto start = steady_clock::now();
    pool.wait();
    auto end = steady_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    log_message("Segundo wait() completado en " + to_string(duration) + "ms");
    
    if (task_count.load() != 3 || duration > 50) {
        oslock.lock();
        cout << "Multiple Waits Test: FAILED - Second wait took " << duration << "ms" << endl;
        oslock.unlock();
        return;
    }
    
    log_message("Programando segundo lote de 2 tareas");
    for (int i = 0; i < 2; i++) {
        pool.schedule([&task_count, i]() {
            log_message("Ejecutando tarea del segundo lote " + to_string(i));
            task_count++;
            log_message("Tarea del segundo lote " + to_string(i) + " completada");
        });
    }
    
    log_message("Segundo lote programado, llamando wait()");
    pool.wait();
    log_message("Tercer wait() completado, task_count final: " + to_string(task_count.load()));
    
    bool passed = (task_count.load() == 5);
    
    oslock.lock();
    cout << "Multiple Waits Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Task count: " << task_count.load() << " (expected 5)" << endl;
    }
    oslock.unlock();
    log_message("=== MULTIPLE WAITS TEST COMPLETADO ===");
}

static void concurrentSchedulingTest() {
    log_message("=== INICIANDO CONCURRENT SCHEDULING TEST ===");
    ThreadPool pool(4);
    atomic<int> task_counter(0);
    const int tasks_per_thread = 100;
    const int num_scheduler_threads = 3;
    log_message("ThreadPool creado con 4 threads, " + to_string(num_scheduler_threads) + " threads programadores, " + to_string(tasks_per_thread) + " tareas cada uno");
    
    vector<thread> schedulers;
    
    for (int t = 0; t < num_scheduler_threads; t++) {
        log_message("Creando thread programador " + to_string(t));
        schedulers.emplace_back([&pool, &task_counter, tasks_per_thread, t]() {
            log_message("Thread programador " + to_string(t) + " iniciado");
            for (int i = 0; i < tasks_per_thread; i++) {
                pool.schedule([&task_counter]() {
                    task_counter.fetch_add(1, memory_order_relaxed);
                });
            }
            log_message("Thread programador " + to_string(t) + " completado");
        });
    }
    
    log_message("Esperando que terminen los threads programadores");
    for (auto& t : schedulers) {
        t.join();
    }
    log_message("Todos los threads programadores terminaron");
    
    log_message("Llamando wait() para esperar todas las tareas");
    pool.wait();
    log_message("Wait() completado, task_counter: " + to_string(task_counter.load()));
    
    bool passed = (task_counter.load() == (tasks_per_thread * num_scheduler_threads));
    
    oslock.lock();
    cout << "Concurrent Scheduling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Task counter: " << task_counter.load() << " (expected " << (tasks_per_thread * num_scheduler_threads) << ")" << endl;
    }
    oslock.unlock();
    log_message("=== CONCURRENT SCHEDULING TEST COMPLETADO ===");
}

static void nestedSchedulingTest() {
    log_message("=== INICIANDO NESTED SCHEDULING TEST ===");
    ThreadPool pool(3);
    atomic<int> leaf_tasks(0);
    log_message("ThreadPool creado con 3 threads");
    
    log_message("Programando tarea nivel 1 (que programará tarea nivel 2)");
    pool.schedule([&pool, &leaf_tasks]() {
        log_message("Ejecutando tarea nivel 1");
        delay_ms(10);
        log_message("Programando tarea nivel 2 desde nivel 1");
        pool.schedule([&pool, &leaf_tasks]() {
            log_message("Ejecutando tarea nivel 2");
            delay_ms(10);
            log_message("Programando tarea nivel 3 desde nivel 2");
            pool.schedule([&leaf_tasks]() {
                log_message("Ejecutando tarea nivel 3 (hoja)");
                leaf_tasks++;
                log_message("Tarea nivel 3 completada");
            });
            log_message("Tarea nivel 2 completada");
        });
        log_message("Tarea nivel 1 completada");
    });
    
    log_message("Tarea anidada programada, llamando wait()");
    pool.wait();
    log_message("Wait() completado, leaf_tasks: " + to_string(leaf_tasks.load()));
    
    bool passed = (leaf_tasks.load() == 1);
    
    oslock.lock();
    cout << "Nested Scheduling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Leaf tasks: " << leaf_tasks.load() << " (expected 1)" << endl;
    }
    oslock.unlock();
    log_message("=== NESTED SCHEDULING TEST COMPLETADO ===");
}

static void exceptionHandlingTest() {
    log_message("=== INICIANDO EXCEPTION HANDLING TEST ===");
    ThreadPool pool(2);
    atomic<int> good_tasks(0);
    atomic<int> total_tasks(0);
    atomic<int> exception_count(0);
    log_message("ThreadPool creado con 2 threads");
    
    log_message("Programando 10 tareas (una lanzará excepción)");
    for (int i = 0; i < 10; i++) {
        pool.schedule([&good_tasks, &total_tasks, &exception_count, i]() {
            log_message("Iniciando tarea " + to_string(i));
            total_tasks++;
            try {
                if (i == 5) {
                    log_message("Tarea " + to_string(i) + " lanzando excepción");
                    throw runtime_error("Test exception");
                }
                good_tasks++;
                log_message("Tarea " + to_string(i) + " completada exitosamente");
            } catch (const runtime_error&) {
                exception_count++;
                log_message("Tarea " + to_string(i) + " excepción capturada");
            }
        });
    }
    
    log_message("Todas las tareas programadas, llamando wait()");
    pool.wait();
    log_message("Wait() completado");
    log_message("Total: " + to_string(total_tasks.load()) + ", Good: " + to_string(good_tasks.load()) + ", Exceptions: " + to_string(exception_count.load()));
    
    bool passed = (total_tasks.load() == 10 && 
                   good_tasks.load() == 9 && 
                   exception_count.load() == 1);
    
    oslock.lock();
    cout << "Exception Handling Test: " << (passed ? "PASSED" : "FAILED") << endl;
    if (!passed) {
        cout << "Total: " << total_tasks.load() << ", Good: " << good_tasks.load() << ", Exceptions: " << exception_count.load() << endl;
    }
    oslock.unlock();
    log_message("=== EXCEPTION HANDLING TEST COMPLETADO ===");
}

static void performanceBenchmarkTest() {
    log_message("=== INICIANDO PERFORMANCE BENCHMARK TEST ===");
    const int num_tasks = 10000;
    ThreadPool pool(6);
    log_message("ThreadPool creado con 6 threads para " + to_string(num_tasks) + " tareas");
    
    auto start = high_resolution_clock::now();
    
    atomic<int> completed(0);
    log_message("Programando " + to_string(num_tasks) + " tareas de benchmark");
    for (int i = 0; i < num_tasks; i++) {
        pool.schedule([&completed]() {
            completed++;
        });
    }
    
    log_message("Todas las tareas de benchmark programadas, llamando wait()");
    pool.wait();
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    log_message("Wait() completado en " + to_string(duration) + "ms");
    log_message("Tareas completadas: " + to_string(completed.load()));
    
    bool passed = (completed.load() == num_tasks && duration < 5000);
    
    oslock.lock();
    cout << "Performance Benchmark Test: " << (passed ? "PASSED" : "FAILED") << endl;
    cout << "Completed " << completed.load() << " tasks in " << duration << "ms" << endl;
    oslock.unlock();
    log_message("=== PERFORMANCE BENCHMARK TEST COMPLETADO ===");
}

static void runAllCustomTests() {
    log_message("=== INICIANDO SUITE COMPLETA DE TESTS CUSTOM ===");
    
    basicSchedulingTest();
    sequentialTimingTest();
    atomicStressTest();
    multipleWaitsTest();
    concurrentSchedulingTest();
    nestedSchedulingTest();
    exceptionHandlingTest();
    performanceBenchmarkTest();
    
    log_message("=== SUITE COMPLETA DE TESTS CUSTOM COMPLETADA ===");
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
