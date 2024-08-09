#include <pthread.h>

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore
{
public:
    // Instance variables
    int value;
    int clear_semaphore;
    pthread_mutex_t queue_mutex;
    int queue_lock;

    // Constructor & destructor methods
    Semaphore(int count);
    ~Semaphore();

    // Semaphore calls
    void wait();
    void signal();

private:

};

#endif
