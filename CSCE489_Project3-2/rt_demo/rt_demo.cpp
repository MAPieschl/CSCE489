#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include "rt_demo.h"
						
pid_t *terminal_pids;
pid_t *next_pid;

int gnome_pipe;

bool wait_for_msg(pid_t expected_child, int expected_msg){

	pid_t child;
	int msg;
	
	read(gnome_pipe, &child, sizeof(pid_t));
	printf("Message received from: %d\n", child);
	
	if(child == expected_child){
		read(gnome_pipe, &msg, sizeof(int));
		printf("Message: %d\n", msg);
		if(msg == expected_msg){
			return true;
		}
	}
	
	return false;
}

int main(){
	
	int ret;
	int continue_simulation = false;
	
	terminal_pids = (pid_t *) malloc((num_pong_terminals + num_monitor_terminals + num_stress_terminals)*sizeof(pid_t));
	next_pid = terminal_pids;
	
	ret = mkfifo(control_path, 0666);
	ret = mkfifo(gnome_path, 0666);
	
	gnome_pipe = open(gnome_path, O_RDWR);
	
	for(int i = 0; i < num_pong_terminals; i++){
		printf("Starting pong game...\n");
		system(pong_terminals[i]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		next_pid++;
	}
	
	for(int i = 0; i < num_monitor_terminals; i++){
		printf("Starting top monitor...\n");
		system(monitor_terminals[i]);
	}
	
	for(int i = 0; i < 1; i++){
		printf("Creating child to create grandchild stressors...\n");
		system(stress_terminals[0]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		next_pid++;
	}
	
	while(continue_simulation == false){
		printf("Waiting for: %d\n", *(next_pid - 1));
		continue_simulation = wait_for_msg(*(next_pid - 1), 1);
	}
	
	for(int i = 0; i < 15; i++){
		printf("Creating additional child stressors...\n");
		system(stress_terminals[1]);
		//read(gnome_pipe, next_pid, sizeof(pid_t));
		//printf("Proc %d stored at %p\n", *next_pid, next_pid);
		//wait_for_msg(*next_pid, 1);
		next_pid++;
	}
	
	printf("Making pong real-time...");
	
	system("

	return 0;
}
