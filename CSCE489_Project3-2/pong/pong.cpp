#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <time.h>
#include <cmath>
#include "animation.h"
//#include "../rt_demo/rt_demo.h"

Animation *animate = NULL;

int pong_pipe;

// temp - need to link rt_demo.h
const char fifo_path[] = {"../temp/fifo"};

void *run(void *data){

	int next_paddle = 4;

	while(true){
		clock_gettime(CLOCK_MONOTONIC, &(animate->ts));
		animate->this_move = animate->ts.tv_sec + ((double) animate->ts.tv_nsec)/NSEC_SCALER;
		
		if(floor((animate->this_move - animate->last_move)/REFRESH_PERIOD_S)){
			animate->move_ball();
		}
		
		read(pong_pipe, &next_paddle, sizeof(next_paddle));
		
		if(next_paddle != animate->paddle_position){
			animate->paddle_position = next_paddle;
			animate->update_screen();
		}
	}
	
	return data;
}

int main(){
	
	pong_pipe = open(fifo_path, O_RDONLY);

	pthread_t animation_thread;
	
	animate = new Animation();
	
	pthread_create(&animation_thread, NULL, run, NULL);
	
	pthread_join(animation_thread, NULL);
	
	return 0;
}
