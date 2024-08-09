/*************************************************************************************
 * babyyoda - completed CSCE489 - Project 2
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "Semaphore.h"
#include <iostream>

// "empty" semaphore - initializes with value = 0
// -> value represents number of dolls on the shelf
// -> wait() holds customers in the queue
Semaphore *empty = NULL;

// "full" semaphore - intiliazes to user-generated buffer_size
// -> value represents the number of empty spots on the shelf
// -> wait() holds producer if shelf is full
Semaphore *full = NULL;

// pointer to the starting address of the bounded buffer representing the "shelf" (static after initialization)
int *start_of_shelf;

// pointer to the address of the next empty position in the bounded buffer representing the "shelf" (*start_of_shelf + buffer_size) 
int *next_empty;

// pointer to the last address of the bounded buffer representing the "shelf" (static after initialization)
int *end_of_shelf;

// locks the shelf while items are being added or removed
pthread_mutex_t shelf_mutex;
int shelf_lock;

// counters for number of dolls consumed and produced
int consumed = 0;
int produced = 0;

// pointer to the address of the first customer (static after initialization)
pthread_t *first_customer;

// pointer to the address of the next customer to buy a doll
pthread_t *next_customer;

// pointer to the address of the last customer in line
pthread_t *last_customer;

// allows user to change rate of production compared to consumption
int timing_slider = 10;

/*************************************************************************************
 * producer_routine - this function is called when the producer thread is created.
 *
 *			Params: data - pointer to max_items
 *
 *			Returns: always NULL
 *
 *************************************************************************************/

void *producer_routine(void *data){

    // Local (easier to read) pointer to max_items
    int *max_items = (int *)data;

    // ------------------- UI OUTPUT ------------------------

    printf("\nBaby Yoda Inc is open for business! Bob (Employee ID: %lu) plans to produce %d dolls today.\n\n", (uintptr_t)pthread_self(), *max_items);

    // -------------- PRODUCTION SEQUENCE -------------------

    do {

        // Check to see if the shelf is full, wait if so
        full->wait();

        shelf_lock = pthread_mutex_lock(&shelf_mutex);

        // Add another to the shelf
        empty->signal();
        produced++;
        *next_empty = produced;
        next_empty++;

        shelf_lock = pthread_mutex_unlock(&shelf_mutex);
        
        // ------------------- UI OUTPUT ------------------------
        
        printf("\nAnother doll is ready for purchase! %d doll(s) has/have been produced today with %d left on the shelf.\n", produced, empty->value);

        std::cout << "\nCurrent shelf: ";
        for(int i = 0; i < (next_empty - start_of_shelf); i++){
            std::cout << start_of_shelf[i] << " ";
        }
        std::cout << "\n";

        // ------------------ TESTING USE ------------------------

        // Random sleep to vary customer arrival (1 - 5 seconds)
        usleep((useconds_t)(rand() % timing_slider*50000));
    } while (produced < *max_items);

    empty->~Semaphore();
    full->~Semaphore();

    return NULL;
}

/*************************************************************************************
 * customer_routine - this function is called when the customer thread is created.
 *
 *       Params: data - NULL
 *
 *       Returns: always NULL
 *
 *************************************************************************************/

void *customer_routine(void *data){

    // ------------------- UI OUTPUT ------------------------

    printf("\nCustomer #%lu (Customer ID: %lu) has arrived!\n", (unsigned long)(last_customer - first_customer), (uintptr_t)pthread_self());

    printf("    There is/are %lu customer(s) currently in line.\n", (unsigned long)(last_customer - next_customer));

    // --------------- PURCHASE SEQUENCE --------------------

    // Check to see if any dolls are on the shelf
    empty->wait();

    // Buy the doll if available
    full->signal();

    // ------------------- CLEANUP --------------------------
    if(consumed < produced){

        // Update shelf
        shelf_lock = pthread_mutex_lock(&shelf_mutex);

        // Increment total dolls consumed
        consumed++;

        // Increment customer queue pointer
        next_customer++;

        // UI output
        printf("\nCustomer #%ld (Customer ID: %lu) has purchased doll #%d!\n", next_customer - first_customer, (uintptr_t)pthread_self(), *start_of_shelf);

        std::cout << "\nCurrent shelf: ";
        for(int i = 0; i < (next_empty - start_of_shelf); i++){
            start_of_shelf[i] = start_of_shelf[i + 1];

            if(i < (next_empty - start_of_shelf - 1)){
                std::cout << start_of_shelf[i] << " ";
            }
        }
        std::cout << "\n";

        next_empty--;

        shelf_lock = pthread_mutex_unlock(&shelf_mutex);
    }
    pthread_exit(data);

    return NULL;
}

/*************************************************************************************
 * main - Standard C main function for our storefront.
 *
 *	Expected params: \.babyyoda <buffer_size> <num_customers> <max_items>
 *			buffer_size - the maximum number of items that can be stored on the shelf
 *			num_customers - the number of customers to visit the store
 *			max_items - how many items will be produced before the shopkeeper closes
 *
 *************************************************************************************/

int main(int argv, const char *argc[]){

    // ------------------------------------ INPUT HANDLING -------------------------------------------

    // Get parameters
    if (argv < 4){
        printf("Invalid parameters. Format: %s <buffer_size> <num_customers> <max_items>\n", argc[0]);
        exit(0);
    }
    else if (argv == 5){
    	timing_slider = (unsigned int)strtol(argc[4], NULL, 10);
    }

    int buffer_size = (unsigned int)strtol(argc[1], NULL, 10);
    int num_customers = (unsigned int)strtol(argc[2], NULL, 10);
    int max_items = (unsigned int)strtol(argc[3], NULL, 10);

    // Ensure valid values were provided. If not, warn users and end program.
    try{
        if (buffer_size <= 0){
            throw(1);
        }

        if (num_customers <= 0){
            throw(2);
        }

        if (max_items <= 0){
            throw(3);
        }
        
        // Check timing slider values
        if (timing_slider <= 0 || timing_slider >= 20){
        	timing_slider = 10;
        	printf("\nThe timing scale factor %s is not valid. Timing is set to default.\n", argc[4]);
        }

        printf("\nWe are expecting %d customers and the shelf can hold %d doll(s).\n", num_customers, buffer_size);
        
        // Check for an unsafe condition (max_items > num_customers)
        if (max_items > num_customers){
        	max_items = num_customers;
        	printf("\nBased on expected customer turnout, max_items has been reduced to %d.\n", max_items);
        }
    }
    catch (int error_code){
        switch (error_code){
            case 1:
                printf("Invalid parameters. Please ensure <buffer_size> is an integer value greater than 0.\n");
                break;
            case 2:
                printf("Invalid parameters. Please ensure <num_customers> is an integer value greater than 0.\n");
                break;
            case 3:
                printf("Invalid parameters. Please ensure <max_items> is an integer value greater than 0.\n");
                break;
            default:
                printf("Invalid parameters. Format: %s <buffer_size> <num_customers> <max_items>\n", argc[0]);
                break;
        }

        return 0;
    }

    // ------------------------------------ LAUNCH THREADS ----------------------------------------------

    // Initialize semaphores
    empty = new Semaphore(0);
    full = new Semaphore(buffer_size);

    // Hire Bob
    pthread_t producer;

    // Clean the shelf
    start_of_shelf = (int *) malloc(buffer_size*sizeof(int));
    next_empty = start_of_shelf;
    end_of_shelf = start_of_shelf + buffer_size;
    pthread_mutex_init(&shelf_mutex, NULL);

    // Bob driving to the store
    pthread_create(&producer, NULL, producer_routine, (void *)&max_items);

    // Bob sweeps the sidewalk to better welcome customers
    pthread_t *customer  = (pthread_t *)malloc(num_customers*sizeof(pthread_t));

    // Designate the spot in front of the register as the first_customer and next_customer
    first_customer = customer;
    next_customer = customer;
    last_customer = customer;

    // Customers start showing up to the store and stand in line
    // - the customers do not move in line (customer # = &customer - first_customer)
    for (int current_customer = 0; current_customer < (int) num_customers; current_customer++){
        
        pthread_create(&customer[current_customer], NULL, customer_routine, 0);

        // Update pointer to last customer in line
        last_customer++;

        // Random sleep to vary customer arrival (1 - 5 seconds)
        usleep((useconds_t)(rand() % 500000));
    };

    // Wait for Bob to finish making dolls
    pthread_join(producer, NULL);

    printf("\nThe manufacturer has completed his work for the day.\n\n");

	// Give the customers a second to finish snatching up items
	while (consumed < max_items){
		sleep(1);
	    printf("\nWaiting for customers to buy up the rest.\n\n");
    }

	// Now make sure they all exited
    int cancel_code;
	for (int i = 0; i < (int) num_customers; i++) {
		cancel_code = pthread_join(customer[i], NULL);
		if(cancel_code != 0){
		    printf("Error: %d | Bob lost the receipt for customer #%d!", cancel_code, i + 1);
		}
		if(i >= consumed){
		    printf("\nCustomer #%d has left without a Baby Yoda doll :(\n", i + 1);
		}
	}

	// Clean up
	delete empty;
	delete full;
    free(start_of_shelf);
    free(customer);

	printf("\nAll customers are gone!\n");
	printf("\nProducer/customer simulation complete!\n");

    return 0;
}
