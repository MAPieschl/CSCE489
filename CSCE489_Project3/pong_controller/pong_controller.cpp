#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int pong_pipe;
int gnome_pipe;

// temp - need to link rt_demo.h
const char control_path[] = {"../temp/ctl_fifo"};
const char gnome_path[] = {"../temp/gnome_fifo"};

int main() {

	int input;

	// Open pipe with rt_demo, send PID for tracking
	pid_t my_pid = getpid();
	
	gnome_pipe = open(gnome_path, O_RDWR);
	
	write(gnome_pipe, &my_pid, sizeof(pid_t));
	
	pong_pipe = open(control_path, O_WRONLY);
	
	printf("Write pipe: %d\n", pong_pipe);
	
	// Wait for user input
	while(input != 9){
		scanf("%d", &input);
		write(pong_pipe, &input, sizeof(input)); 
	}

	return 0;
}
