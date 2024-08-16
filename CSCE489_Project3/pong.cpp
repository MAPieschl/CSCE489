#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <time.h>
#include "animation.h"

#define TIME_CONSTANT 100000
#define USEC_SCALER 1000000
#define NSEC_SCALER 1000000000

Animation *animate = NULL;

int pip[2];

void *run(void *data){

	struct timespec ts;
	int dummy_var;

	while(true){
		animate->update_screen();
		clock_gettime(CLOCK_MONOTONIC, &ts);
		animate->start_time = ts.tv_sec + ((double) ts.tv_nsec)/NSEC_SCALER;
		for(int i = 0; i < TIME_CONSTANT*100; i++){
			dummy_var = i;
			dummy_var++;
		}
		clock_gettime(CLOCK_MONOTONIC, &ts);
		animate->end_time = ts.tv_sec + ((double) ts.tv_nsec)/NSEC_SCALER;
		animate->time_warp = (animate->end_time - animate->start_time)/((double)TIME_CONSTANT/USEC_SCALER);
	}
	
	return data;
}

void *open_input_terminal(void *data){

	std::cout << "Please input next paddle position (1-8) and press RETURN: ";

	while(true){
		std::cin >> animate->paddle_position;
	}

	return data;
}

int main(){
	pthread_t animation_thread;
	
	animate = new Animation();
	
	//pipe(pip);
	
	//std::string input_terminal_cmd = {"gnome-terminal --title Pong Input -- ./pong_input"};
	
	//pid_t input_pid = fork();
	
	//if (input_pid == 0){
	//	printf("Starting input system...");
	//	system(input_terminal_cmd.c_str());
	//	return 0;
	//}
	
	pthread_create(&animation_thread, NULL, run, NULL);
	
	pthread_join(animation_thread, NULL);
	
	return 0;
}
