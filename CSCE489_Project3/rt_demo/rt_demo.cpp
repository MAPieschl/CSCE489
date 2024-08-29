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

// Pointer to start of active child buffer
pid_t *terminal_pids;

// Next open bin for an active child PID
pid_t *next_pid;

// Pipe to communicate with all children
int gnome_pipe;

/* 
*  Non-blocking function to catch for a response from a child
*
*  params:	expected_child - PID of expected message passer
*			expected_message - integer value of expected message
*
*  returns:	true - expected message from expected sender / false - otherwise
*
*/
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


/* 
*  Blocking function to wait for user input
*
*  returns:	true - continue simulation / false - cleanup and quit
*/
bool wait_for_user(){
	char user_in = 0;
	
	while(user_in !='y'){
	
		if(user_in == 0){
			printf("Would you like to continue (y) or quit (n)? ");
		}
		
		scanf("%c", &user_in);
		
		if(user_in == 'n'){
			return false;
		}
	}
	
	return true;
}

/* 
*  Cleanup children & reset scheduling variables
*	NOTE:  `top` monitor will remain open following simulation
*/
void end_simulation(){

	// Number of open child processes
	int num_open_procs = next_pid - terminal_pids;
	int exit_code;
	
	for(int i = 0; i < num_open_procs; i++){
		next_pid--;
		exit_code = kill(*next_pid, SIGKILL);
		if(exit_code != 0){
			printf("Error while killing proc %d", *next_pid);
		}
	}
	
	//
	//
	//  Add sched value recovery
	//
	//
	
	return;
}

/* 
*  Main procedure for real-time process simualtion
*/
int main(){
	
	// Re-usable variable to catch return values
	int ret;
	
	// Catch variable to continue waiting for process response
	int continue_simulation = false;
	
	// Empty string buffer for dynamic command creation
	char command_buffer[128];
	
	// Heap-based child PID storage
	terminal_pids = (pid_t *) malloc((num_pong_terminals + num_monitor_terminals + num_child_stressors + 2*num_sibling_stressors)*sizeof(pid_t));
	next_pid = terminal_pids;
	
	// Control pipe for use as controller in pong game
	mkfifo(control_path, 0666);
	
	// Child process general communication pipe
	mkfifo(gnome_path, 0666);
	
	gnome_pipe = open(gnome_path, O_RDWR);
	
	// Start pong game and store PID
	for(int i = 0; i < num_pong_terminals; i++){
		printf("Starting pong game...\n");
		system(pong_terminals[i]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		next_pid++;
	}
	
	// Start top monitor (not tracked - simulation loses control of this terminal)
	for(int i = 0; i < num_monitor_terminals; i++){
		printf("Starting top monitor...\n");
		system(monitor_terminals[i]);
	}
	
	printf("\n\nFamiliarize yourself with the pong game by clicking inside the pong controller terminal, typing in a number (1 - 8) representing a paddle position, then press Enter. Take note of the time stretch value in the lower left corner. When you're ready, return to this terminal and type y + Enter to continue...\n\n");
	
	// Start pong controller and store PID
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	/*
	*	Uncommon SID simulation 
	*/
	
	// Start child stressors and store PIDs
	printf("Creating child stressors...\n");
	for(int i = 0; i < num_sibling_stressors; i++){
		system(stress_terminals[1]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		wait_for_msg(*next_pid, 1);
		next_pid++;
	}
	
	printf("\n\nWe have now launched %d CPU stressors to show the equitable distribution of CPU time amongst the children of the simulation. Pull the Monitor terminal into view and notice that the Completely Fair Scheduler (CFS) apportions all children of the simulation (currently pong, pong controller, top, and %d stressors) equal portions the total CPU time available. Note that the pong controller is blocked, waiting for user input, the top controller is I/O-bound, and each stressor is made up of 2 processes (terminal and stressor) which each take up half of the CPU time apportioned to that child. This is why each stressor receives half of the total CPU time that the pong game is given.\n\n", num_sibling_stressors, num_sibling_stressors);
	
	printf("\n\nTry playing the pong game and take note of the time stretch value in the lower left corner of the display. Next we will attempt to change the nice value of the pong game to speed the game play back up...\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	// Apply higher priority nice value to pong terminal
	sprintf(command_buffer, "sudo renice -n -20 -p %d", *terminal_pids);
	system(command_buffer);
	
	printf("\n\nNotice that the the nice value (NI in the Monitor terminal) has been modified for the pong terminal, but the CPU allocation has not changed. Next, we'll launch a process printout. Take a look at the SID of the pong game (PID %d) and notice that it differs from the pong games.\n\n", *terminal_pids);
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	// 
	//	Terminal exiting immediately -- needs fixed
	//
	system("gnome-terminal --title PS -- ps -efj");
	
	printf("\n\nA change in nice value will only take effect between processes of the same SID. We'll relaunch the stressors to show this. Note that you'll have to exit out of all PS terminals launched during this simulation yourself...\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	printf("Killing additional child stressors...\n");
	for(int i = 0; i < num_sibling_stressors; i++){
		next_pid--;
		ret = kill(*next_pid, SIGKILL);
		if(ret < 0){
			printf("Failed to kill %d\n", *next_pid);
		}
	}
	
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
	
	printf("\n\nAccording to the Linux sched documentation, terminals always initialize a new SID for themselves and their child processes. Therefore, we have now launched all %d stressors in the same terminal. Continue to relaunch the PS terminal...\n\n", num_sibling_stressors);
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	// 
	//	Terminal exiting immediately -- needs fixed
	//
	system("gnome-terminal --title PS -- ps -ejf");
	
	printf("\n\nNotice that all of the stressors listed in the PS terminal share a common SID (which is assigned the same value as the parent terminal PID). While still cannot prioritize the pong game, by containing all of our additional stressors to a parent process at the same level as the pong game, we can manipulate the prioritization of the stressors while maintaining a consistent ~45-50 percent of CPU time allocated to the pong game. Play pong to note the performance improvement, then return to this terminal to continue the simulation. \n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	printf("\n\n\Next, we will manipulate the nice value of a stressor process to show the prioritization in action...\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	//
	//	Grandchildren not logging correctly -- wrong PID is being grabbed
	//
	// Apply higher priority nice value to last child stressor
	sprintf(command_buffer, "sudo renice -n -20 -p %d", (*next_pid - 1));
	system(command_buffer);
	
	printf("\n\nNotice the prioritization of PID %d in the Monitor terminal, which is now stealing most of the CPU time allotted to the stressor parent, while not effecting the pong game.\n\n", (*next_pid - 1));
	
	printf("\n\n ********** END OF CURRENT BUILD - PENDING RT BUILD ************\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	// Kill Child Stressor Process
	next_pid--;
	kill(*next_pid, SIGKILL);
	
	printf("Creating child stressor for real-time demo...\n");
	
	printf("Creating additional child stressors...\n");
	for(int i = 0; i < num_sibling_stressors; i++){
		system(stress_terminals[1]);
		read(gnome_pipe, next_pid, sizeof(pid_t));
		printf("Proc %d stored at %p\n", *next_pid, next_pid);
		wait_for_msg(*next_pid, 1);
		next_pid++;
	}
	
	
	wait_for_user();
	
	// Kill Child Stressor Process
	next_pid--;
	kill(*next_pid, SIGKILL);
	
	// Cleanup
	end_simulation();

	return 0;
}
