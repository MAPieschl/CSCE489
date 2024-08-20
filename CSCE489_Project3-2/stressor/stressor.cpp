#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#define STRESSOR_LAUNCH_US 2000000

int gnome_pipe;

pid_t *stressors;
pid_t *next_stressor;

// temp - need to link rt_demo.h
const char gnome_path[] = {"../temp/gnome_fifo"};

void compute_a_lot(){
	double first_val = 3.14159;
	double second_val = 3.14159;
	
	while(true){
		first_val *= second_val;
		first_val /= second_val;
	}
	
	return;
}

int main(int argv, char *argc[]) {

	int num_stressors = std::stoi(argc[1], nullptr, 10);
	int msg;
	int ret;

	pid_t my_pid = getpid();
	gnome_pipe = open(gnome_path, O_RDWR);
	ret = write(gnome_pipe, &my_pid, sizeof(pid_t));
	printf("Sent PID# %d and received return code %d\n", my_pid, ret);
	
	stressors = (pid_t *) malloc(num_stressors*sizeof(pthread_t));
	next_stressor = stressors;
	
	for(int i = 0; i < num_stressors; i++){
		*next_stressor = fork();
		
		if(*next_stressor == 0){
			compute_a_lot();
		}
		else{
			printf("New stressor (%d) created at %p\n", *next_stressor, next_stressor);
			next_stressor++;
			usleep(STRESSOR_LAUNCH_US);
		}
	}
	
	usleep(1000000);
	printf("Done creating stressors, sending PID...\n");
	
	ret = write(gnome_pipe, &my_pid, sizeof(pid_t));
	printf("Return code: %d\n", ret);
	
	msg = 1;
	
	printf("Sending complete message...\n");
	write(gnome_pipe, &msg, sizeof(int));
	printf("Return code: %d\n", ret);
	
	while(true){
		//read(gnome_pipe, &msg, sizeof(int));
	}

	return 0;
}
