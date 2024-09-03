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
			printf("\nError while killing proc %d\n", *next_pid);
		}
	}
	
	return;
}

void show_buffer(pid_t *buffer_start, pid_t *next_ptr, int buffer_size){

	for(int i = 0; i < buffer_size; i++){
		if(buffer_start + i == next_ptr){
			printf("\n>>");
		}
		else{
			printf("\n");
		}
		
		printf("\t%p:  %d", buffer_start + i, *(buffer_start + i));
	}
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
	terminal_pids = (pid_t *) malloc(pid_buffer_size*sizeof(pid_t));
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
	
	//show_buffer(terminal_pids, next_pid, pid_buffer_size);
	
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
	
	printf("\n\nWe have now launched %d CPU stressors to show the equitable distribution of CPU time amongst the children of the simulation. Pull the Monitor terminal into view and notice that the Completely Fair Scheduler (CFS) apportions all children of the simulation (currently pong, pong controller, top, and %d stressors) equal portions of the total CPU time available. Note that the pong controller is blocked, waiting for user input, the top controller is I/O-bound, and each stressor is made up of 2 processes (terminal and stressor) which each take up half of the CPU time apportioned to that child. This is why each stressor receives half of the total CPU time that the pong game is given.\n\n", num_sibling_stressors, num_sibling_stressors);
	
	printf("\n\nTry playing the pong game and take note of the time stretch value in the lower left corner of the display. Next we will attempt to change the nice value of the pong game to speed the game play back up...\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	/*
	*	Common SID simulation 
	*/
	
	//show_buffer(terminal_pids, next_pid, pid_buffer_size);
	
	// Apply higher priority nice value to pong terminal
	sprintf(command_buffer, "sudo renice -n -20 -p %d", *terminal_pids);
	system(command_buffer);
	
	printf("\n\nNotice that the the nice value (NI in the Monitor terminal) has been modified for the pong terminal, but the CPU allocation has not changed. Click on the Monitor terminal, press 'F', then scroll to SID, press 'd' to add it to the Monitor, then press'q' to escape. Take a look at the SID of the pong game (PID %d) and notice that it differs from the stressor terminals.\n\n", *terminal_pids);
	
	printf("\n\nA change in nice value will only take effect between processes of the same SID. We'll relaunch the stressors to show this...\n\n");
	
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
	
	//show_buffer(terminal_pids, next_pid, pid_buffer_size);
	
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
	
	//show_buffer(terminal_pids, next_pid, pid_buffer_size);
	
	printf("\n\nAccording to the Linux sched documentation, terminals always initialize a new SID for themselves and their child processes. Therefore, we have now launched all %d stressors in the same terminal.\n\n", num_sibling_stressors);
	
	printf("\n\nNotice that all of the stressors listed in the Monitor terminal share a common SID (which is assigned the same value as the parent terminal PID). While we still cannot prioritize the pong game, by containing all of our additional stressors to a parent process at the same level as the pong game, we can manipulate the prioritization of the stressors while maintaining a consistent ~45-50 percent of CPU time allocated to the pong game. Play pong to note the performance improvement, then return to this terminal to continue the simulation.\n\n");
	
	printf("\n\nNext, we will manipulate the nice value of a stressor process to show the prioritization in action...\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	sprintf(command_buffer, "sudo renice -n -20 -p %d", *(next_pid - 1));
	system(command_buffer);
	
	printf("\n\nNotice the prioritization of PID %d in the Monitor terminal, which is now stealing most of the CPU time allotted to the stressor SID, while not effecting the pong game. Next we will reset all nice values and show the effects of scheduling the pong game using a real-time scheduler...\n\n", *(next_pid - 1));
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	// Resest nice value for child stressor
	sprintf(command_buffer, "sudo renice -n 0 -p %d", *(next_pid - 1));
	system(command_buffer);
	
	// Resest nice value for pong game
	sprintf(command_buffer, "sudo renice -n 0 -p %d", *terminal_pids);
	system(command_buffer);
	
	/*
	*	Real-time pong simulation 
	*/
	
	sprintf(command_buffer, "sudo chrt -r -p 2 %d", *terminal_pids);
	system(command_buffer);
	
	printf("\n\nThe pong game has now been switched to the round-robin, real-time scheduler. Notice that CPU allocation has not been affected. This was an unexpected result and the reason is unknown. Presumably, it could be due to the system calls being made in the game loop, which cause ~60 percent of the process to execute in kernel mode. Because of this, it does not appear to be an effective way to prioritize the pong game. Do note the latency of the user interface of ubuntu with the real-time scheduler active. We'll explore this more in the coming steps.\n\n ");
	
	printf("\n\nIn order to still show the expected effect of the real-time scheduler, we'll reset the pong game to the CFS and set one of the child stressors to round-robin, real-time scheduling...\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	sprintf(command_buffer, "sudo chrt -o -p 0 %d", *terminal_pids);
	system(command_buffer);
	
	sprintf(command_buffer, "sudo chrt -r -p 2 %d", *(next_pid - 1));
	system(command_buffer);
	
	//show_buffer(terminal_pids, next_pid, pid_buffer_size);
	
	printf("\n\nThe stressor process (PID %d) is now the only real-time process and is taking all 95 percent of its alloted CPU time, allowing all CFS processes to share the remaining 5 percent. Attempt to play the pong game and note the extreme latency incurred. Next we will modify the value of /proc/sys/kernel/sched_rt_runtime_us to reduce the amount of CPU time alloted to the realtime process and increase the amount shared by the CFS processes.\n\n", *(next_pid - 1));
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	system("sudo sh -c 'echo 500000 > /proc/sys/kernel/sched_rt_runtime_us'");
	
	printf("\n\nNotice the reduction in the real-time stressor CPU usage to 50 percent and the improved performance of the pong game. The 95 percent CPU allocation is a Linux default, but modifiable value. Compared to earlier in the simulation when the pong game was allocated ~50 percent CPU time. This is presumably due to the non-preemptive nature of the real-time scheduler, ensuring that all other processes (under CFS) are only addressed once every 500ms which was the value set in the previous step for the real-time runtime value. By maintaining the ratio of sched_rt_runtime_us/sched_rt_period_us = 50, but reducing the total period, we can allow the CFS shorter, but more frequent bursts to improve the user interface of Ubuntu. For these purposes, we'll increase frequency (i.e. reduce the period) by two orders of magnitude. Note that this will counteract improved system performance to some degree due to the increased overhead of more frequent process switching.\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	system("sudo sh -c 'echo 5000 > /proc/sys/kernel/sched_rt_runtime_us'");
	system("sudo sh -c 'echo 10000 > /proc/sys/kernel/sched_rt_period_us'");
	
	printf("\n\nYou should now notice a distinct reduction in latency while playing the pong game. Take a look at the Time Stretch value compared to the previous iteration. Although the total CPU time remains unchanged, the frequency of screen updates allows up to play the game without the perceived choppiness we experienced previously.\n\n");
	
	if(wait_for_user() == false){
		printf("Terminated by user...\n");
		end_simulation();
		return 0;
	}
	
	system("sudo sh -c 'echo 950000 > /proc/sys/kernel/sched_rt_runtime_us'");
	system("sudo sh -c 'echo 1000000 > /proc/sys/kernel/sched_rt_period_us'");
	
	// Cleanup
	end_simulation();

	return 0;
}
