#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "animation.h"


/*************************************************************************************
 * Semaphore (constructor) - creates a generic semaphore
 *
 *    Params:  count - initialization count for the semaphore
 *
 *************************************************************************************/

Animation::Animation(){
	this->paddle_position = 40;
	this->paddle_width = 5;
}

/*************************************************************************************
 * ~Semaphore (destructor) - clears waiting items in the semaphore
 *
 *************************************************************************************/

Animation::~Animation(){
}

/*************************************************************************************
 * wait - standard wait semaphore method with additional protections for concurrency
 *
 *************************************************************************************/

void Animation::clear_screen(){
    printf("\033[H\033[J");
}

/*************************************************************************************
 * wait - standard wait semaphore method with additional protections for concurrency
 *
 *************************************************************************************/

void Animation::update_screen(){

	this->clear_screen();

	char paddle_line[80] = " ";
	

	for (int i = 0; i < 23; i++){
		printf("\n");
	}
	
	for (int i = 0; i < this->paddle_position; i++){
		paddle_line += " ";
	}
	
	for (int i = 0; i < this->paddle_width; i++){
		paddle_line += "=";
	}
	
	std::cout << paddle_line << "\n";
	
	usleep(1000000);
}
