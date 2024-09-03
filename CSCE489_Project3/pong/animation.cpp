#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <time.h>
#include "animation.h"


/*************************************************************************************
 * Animation (constructor) - assigns initial values for animation
 *
 *    Params:  none
 *
 *************************************************************************************/

Animation::Animation(){
	this->start_game = false;
	this->run_game = true;
	this->strace = false;
	
	this->paddle_position = 0;
	this->paddle_width = 10;
	
	this->ball_x = TERMINAL_WIDTH/2;
	this->ball_y = TERMINAL_HEIGHT/2;
	
	this->ball_vx = 1;
	this->ball_vy = 1;
	
	this->last_move = 0;
	this->this_move = 0;
	
	this->time_stretch = 1;
}

/*************************************************************************************
 * ~Animation (destructor) - Not used
 *
 *************************************************************************************/

Animation::~Animation(){
}

/*************************************************************************************
 * clear_screen() - clears standard terminal screen (ChatGPT)
 *
 *************************************************************************************/

void Animation::clear_screen(){
    printf("\033[H\033[J");
}

/*************************************************************************************
 * move_ball() - calculates next ball position and updates screen
 *
 * returns  true - ball still in play // false - ball out of bounds
 *
 *************************************************************************************/

bool Animation::move_ball(){

	bool on_paddle = this->ball_x >= this->paddle_width*this->paddle_position && this->ball_x <= this->paddle_width*(this->paddle_position + 1);

	if(this->ball_x <= 0 || this->ball_x >= TERMINAL_WIDTH){
		this->ball_vx *= -1;
	}
	
	if(this->ball_y <= 0 || (this->ball_y >= TERMINAL_HEIGHT - 2 && on_paddle)){
		this->ball_vy *= -1;
	}
	else if(this->ball_y >= TERMINAL_HEIGHT && !on_paddle){
		this->next_paddle_position = 0;
		this->reset_ball();
    	this->update_screen();
		return false;
	}
	
    this->ball_x += this->ball_vx;
    this->ball_y += this->ball_vy;
    
    this->update_screen();
    
    clock_gettime(CLOCK_REALTIME, &ts);
    this->last_move = ts.tv_sec + ((double) ts.tv_nsec)/NSEC_SCALER;
    
    return true;
}

/*************************************************************************************
 * reset_ball() - sets ball to center of screen
 *
 *************************************************************************************/

void Animation::reset_ball(){
	
	this->ball_x = TERMINAL_WIDTH/2;
	this->ball_y = TERMINAL_HEIGHT/2;
	
}

/*************************************************************************************
 * update_screen() - draws game on screen
 *
 *************************************************************************************/

void Animation::update_screen(){

	if(this->strace == false){
		this->clear_screen();
	}

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
		if(i < this->paddle_width*this->paddle_position || i > this->paddle_width*(this->paddle_position + 1)){
			this->paddle_line[i] = ' ';
		}
		else{
			this->paddle_line[i] = '=';
		}
	}
	
	printf("%s\n", this->paddle_line);
	printf("|    1    |    2    |    3    |    4    |    5    |    6    |    7    |    8   |\n");
	
	float percent_time_stretch = (this->time_stretch - 1) * 100;
	
	printf("Time Stretch:  %lf\%\n", percent_time_stretch);
}
