#include <pthread.h>
#include <iostream>
#include "Semaphore.h"

/*************************************************************************************
 * Semaphore (constructor) - creates a generic semaphore
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
 * ~Semaphore (destructor) - clears waiting items in the semaphore
 *
 *************************************************************************************/

Semaphore::~Semaphore(){
    this->clear_semaphore = 1;
}

/*************************************************************************************
 * wait - standard wait semaphore method with additional protections for concurrency
 *
 *************************************************************************************/

void Semaphore::wait(){
    queue_lock = pthread_mutex_lock(&queue_mutex);
    while (this->value <= 0 && this->clear_semaphore == 0){
        continue;
    }
    queue_lock = pthread_mutex_unlock(&queue_mutex);

    if(this->clear_semaphore == 0){
        this->value--;
    }
}

/*************************************************************************************
 * signal - standard signal Semaphore method
 *
 *************************************************************************************/

void Semaphore::signal(){
    if(this->clear_semaphore == 0){
        this->value++;
    }
}
