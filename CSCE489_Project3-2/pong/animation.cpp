#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <time.h>
#include "animation.h"


/*************************************************************************************
 * Semaphore (constructor) - creates a generic semaphore
 *
 *    Params:  count - initialization count for the semaphore
 *
 *************************************************************************************/

Animation::Animation(){
	this->paddle_position = 4;
	this->paddle_width = 10;
	
	this->ball_x = TERMINAL_WIDTH/2;
	this->ball_y = TERMINAL_HEIGHT/2;
	
	this->ball_vx = 1;
	this->ball_vy = 1;
	
	this->last_move = 0;
	this->this_move = 0;
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

void Animation::move_ball(){

	if(this->ball_x <= 0 || this->ball_x >= TERMINAL_WIDTH){
		this->ball_vx *= -1;
	}
	
	if(this->ball_y <= 0 || this->ball_y >= TERMINAL_HEIGHT - 3){
		this->ball_vy *= -1;
	}
	
    this->ball_x += this->ball_vx;
    this->ball_y += this->ball_vy;
    
    this->update_screen();
    
    clock_gettime(CLOCK_MONOTONIC, &ts);
    this->last_move = ts.tv_sec + ((double) ts.tv_nsec)/NSEC_SCALER;
}

/*************************************************************************************
 * wait - standard wait semaphore method with additional protections for concurrency
 *
 *************************************************************************************/

void Animation::update_screen(){

	this->clear_screen();

	for (int i = 0; i < TERMINAL_HEIGHT; i++){
		if (i == this->ball_y){
			for (int j = 0; j < TERMINAL_WIDTH; j++){
				if (j < this->ball_x || j > this->ball_x){
					this->ball_line[j] = ' ';
					continue;
				}
				else{
					this->ball_line[j] = '*';
				}
			}
			printf("%s", this->ball_line);
		}
		else{
			printf("\n");
		}
	}
	
	for (int i = 0; i < TERMINAL_WIDTH; i++){
		if(i < this->paddle_width*paddle_position || i > this->paddle_width*(this->paddle_position + 1)){
			this->paddle_line[i] = ' ';
		}
		else{
			this->paddle_line[i] = '=';
		}
	}
	
	printf("%s\n", this->paddle_line);
	printf("|    1    |    2    |    3    |    4    |    5    |    6    |    7    |    8   |\n");
}
