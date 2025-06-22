#ifndef _semaphore_
#define _semaphore_

#include <condition_variable>
#include <mutex>

using namespace std;

/**
 * @class Semaphore
 * @brief A class that represents a semaphore.
 *
 * A semaphore is a synchronization primitive that controls access to a shared resource.
 * It allows multiple threads to access the resource concurrently, but with a limited capacity.
 */
class Semaphore 
{
    public:

        Semaphore(int count = 0); 
        void signal ();
        void wait(); 

    private:

        int count_;
        mutex mutex_;
        condition_variable_any condition_;
        
        Semaphore(const Semaphore& orig) = delete;              // no copy constructor
        Semaphore& operator=(const Semaphore& orig) = delete;   // no copy assignment
};

#endif
