#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "animation.h"

Animation *animate = NULL;

void *run(void *data){

	while(true){
		animate->update_screen();
		animate->paddle_position = (animate->paddle_position + 1) % 80;
		usleep(1000000);
	}
	
	return data;
}

int main(){
	pthread_t animation_thread;
	
	animate = new Animation();
	
	pthread_create(&animation_thread, NULL, run, NULL);
	pthread_join(animation_thread, NULL);
	
	return 0;
}
