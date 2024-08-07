#include <pthread.h>
#include <iostream>
#include "Semaphore.h"

/*************************************************************************************
 * Semaphore (constructor) - this should take count and place it into a local variable.
 *						Here you can do any other initialization you may need.
 *
 *    Params:  count - initialization count for the semaphore
 *
 *************************************************************************************/

Semaphore::Semaphore(int count){
    this->value = count;
    this->clear_semaphore = 0;
    pthread_mutex_init(&queue_mutex, NULL);
}

/*************************************************************************************
 * ~Semaphore (destructor) - called when the class is destroyed. Clean up any dynamic
 *						memory.
 *
 *************************************************************************************/

Semaphore::~Semaphore(){
    this->clear_semaphore = 1;
}

/*************************************************************************************
 * wait - implement a standard wait Semaphore method here
 *
 *************************************************************************************/

void Semaphore::wait(){
    int queue_lock = pthread_mutex_lock(&queue_mutex);
    while (this->value <= 0 && this->clear_semaphore == 0){
        continue;
    }
    queue_lock = pthread_mutex_unlock(&queue_mutex);

    if(this->clear_semaphore == 0){
        this->value--;
    }
}

/*************************************************************************************
 * signal - implement a standard signal Semaphore method here
 *
 *************************************************************************************/

void Semaphore::signal(){
    if(this->clear_semaphore == 0){
        this->value++;
    }
}
