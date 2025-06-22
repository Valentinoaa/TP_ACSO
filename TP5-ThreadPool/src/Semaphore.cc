#include <mutex>
#include "Semaphore.h"
#include <condition_variable>

/**
 * @brief Constructs a Semaphore object with the specified initial count.
 *
 * The Semaphore class provides a synchronization primitive that can be used to control access to a shared resource.
 * The initial count determines the number of threads that can access the resource simultaneously without blocking.
 *
 * @param count The initial count of the semaphore.
 */
Semaphore::Semaphore(int count) : count_(count) {}

/**
 * Signals the semaphore, allowing one waiting thread to proceed.
 */
void Semaphore::signal () 
{
    lock_guard<mutex> lg(mutex_);
    count_++;
    if(count_ == 1) condition_.notify_all();
}

/**
 * @brief Waits until the semaphore is available and then acquires it.
 * 
 * This function blocks the current thread until the semaphore is available.
 * Once the semaphore becomes available, it is acquired by decrementing the count.
 * 
 * @note This function assumes that the caller has already acquired the lock associated with the semaphore.
 * 
 * @param None.
 * @return None.
 */
void Semaphore::wait() 
{
    lock_guard<mutex> lg(mutex_);
    condition_.wait(mutex_, [this](){return count_ > 0;});
    count_--;
}
