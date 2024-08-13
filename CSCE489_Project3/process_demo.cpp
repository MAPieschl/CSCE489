#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string>

// The terminals that will spawn when the program is executed
const char *terminals[] = {"gnome-terminal --title Pong", 
							"gnome-terminal --title Monitor -- top"};

// Number of terminals to spawn
int num_terminals = sizeof(terminals)/sizeof(terminals[0]);

// Pointer to array of children PIDs
pid_t *terminal_pids;

// PID of process_demo
pid_t top_pid;

int main(){

	top_pid = getpid();

	terminal_pids = (pid_t*)malloc(num_terminals*sizeof(pid_t));
	
	pid_t next_child;
	
	for (int i = 0; i < num_terminals; i++){
		next_child = fork();
		
		if (next_child == 0){
			system(terminals[i]);
			return 0;
		}
		else{
			*terminal_pids = next_child;
			terminal_pids++;
		}
	}
	
	
	printf("\nPress RETURN to exit...");
	
	int exit_demo = 0;
	
	do{
		exit_demo = getchar();
	} while (exit_demo == 0);

	return 0;
}
