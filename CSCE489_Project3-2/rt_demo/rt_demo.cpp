#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <cmath>
#include "rt_demo.h"
						
pid_t *terminal_pids;
pid_t *next_pid;

int gnome_pipe;

void int_to_string(int val, char *char_temp){
	if(val > 9 || val < 0){
		*char_temp = 0;
	}
	else{
		*char_temp = (char) (val + 48);
	}
}

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

void wait_for_user(){
	char user_in = 0;
	
	while(user_in !='y'){
		printf("Would you like to continue (y)? ");
		scanf("%c", &user_in);
	}
	
	return;
}

int main(){
	
	int ret;
	int continue_simulation = false;
	int user_catch;
	
	terminal_pids = (pid_t *) malloc((num_pong_terminals + num_monitor_terminals + num_child_stressors + 2*num_sibling_stressors)*sizeof(pid_t));
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
	
	wait_for_user();
	
	for(int i = 0; i < num_child_stressors; i++){
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
	
	wait_for_user();
	
	// Kill Child Stressor Process
	next_pid--;
	kill(*next_pid, SIGKILL);
	
	printf("Creating child stressors...\n");
	for(int i = 0; i < num_sibling_stressors; i++){
		system(stress_terminals[1]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		wait_for_msg(*next_pid, 1);
		next_pid++;
	}
	
	wait_for_user();
	
	printf("Creating additional child stressors...\n");
	for(int i = 0; i < num_sibling_stressors; i++){
		system(stress_terminals[1]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		wait_for_msg(*next_pid, 1);
		next_pid++;
	}
	
	wait_for_user();
	
	printf("Killing additional child stressors...\n");
	for(int i = 0; i < num_sibling_stressors; i++){
		next_pid--;
		ret = kill(*next_pid, SIGKILL);
		if(ret < 0){
			printf("Failed to kill %d\n", *next_pid);
		}
	}
	
	wait_for_user();
	
	pid_t *stressor_pid = terminal_pids + num_pong_terminals;
	for(int i = 0; i < num_sibling_stressors; i++){
		ret = setpgid(*stressor_pid, *(terminal_pids + num_pong_terminals));
		if(ret < 0){
			printf("Failed to set group ID for %d\n", *stressor_pid);
		}
		stressor_pid++;
	}

	return 0;
}
