#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <cmath>

// The terminals that will spawn when the program is executed
const char *terminals[] = {"gnome-terminal --title Pong -- ./pong",
							"gnome-terminal --title Monitor -- top"};

// Number of terminals to spawn
int num_terminals = sizeof(terminals)/sizeof(terminals[0]);

// Pointer to array of children PIDs
pid_t *terminal_pids;

// PID of process_demo
pid_t top_pid;

void cpu_killer(){
	long new_val = 256;
	
	while(true){
		new_val *= new_val;
		new_val = sqrt(new_val);
	}
}

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
	
	pid_t new_worker[16];
	
	for (int i = 0; i < 16; i++){
		new_worker[i] = fork();
		if(new_worker[i] == 0){
			cpu_killer();
		}
		else{
			printf("\nNew worker generated: %d", new_worker[i]);
		}
		usleep(10000000);
	}

	printf("\nPress RETURN to exit...%d\n", getpid());
	
	int exit_demo = 0;
	
	do{
		exit_demo = getchar();
	} while (exit_demo == 0);

	return 0;
}
