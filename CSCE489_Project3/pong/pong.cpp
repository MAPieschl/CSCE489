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
int gnome_pipe;

// temp - need to link rt_demo.h
const char control_path[] = {"../temp/ctl_fifo"};
const char gnome_path[] = {"../temp/gnome_fifo"};

bool get_paddle_position(){
	read(pong_pipe, &animate->next_paddle_position, sizeof(animate->next_paddle_position));
	
	if(animate->next_paddle_position == 9){
		return false;
	}
	
	if(animate->next_paddle_position > 0 && animate->next_paddle_position <= 8 && animate->start_game == false){
		animate->paddle_position = animate->next_paddle_position - 1;
		animate->start_game = true;
	}
	
	if(animate->next_paddle_position - 1 != animate->paddle_position){
		animate->paddle_position = animate->next_paddle_position - 1;
		animate->update_screen();
	}
	
	return true;
}

void *run(void *data){

	while(animate->run_game == true){
		clock_gettime(CLOCK_REALTIME, &(animate->ts));
		animate->this_move = animate->ts.tv_sec + ((double) animate->ts.tv_nsec)/NSEC_SCALER;
		
		animate->run_game = get_paddle_position();
		
		animate->time_stretch = (animate->this_move - animate->last_move)/REFRESH_PERIOD_S;
		
		if(floor(animate->time_stretch) && animate->start_game == true){
			animate->start_game = animate->move_ball();
		}
		else if(animate->start_game == false){
			animate->reset_ball();
		}
	}

	
	return data;
}

int main(int argv, char *argc[]){

	printf("%d\n", argv);

	pid_t my_pid = getpid();
	
	gnome_pipe = open(gnome_path, O_RDWR);
	
	int ret = write(gnome_pipe, &my_pid, sizeof(pid_t));
	
	pong_pipe = open(control_path, O_RDONLY | O_NONBLOCK);

	pthread_t animation_thread;
	
	animate = new Animation();

	if(argv > 1){
		if(argc[1] == "-s"){
			animate->strace = true;
			printf("Pong game is in tracing mode...\n");
		}
	}
	
	pthread_create(&animation_thread, NULL, run, NULL);
	
	pthread_join(animation_thread, NULL);
	
	return 0;
}
