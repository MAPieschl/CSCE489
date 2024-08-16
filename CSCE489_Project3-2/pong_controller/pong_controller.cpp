#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int pong_pipe;

// temp - need to link rt_demo.h
const char fifo_path[] = {"../temp/fifo"};

int main() {

	int input;
	
	pong_pipe = open(fifo_path, O_WRONLY);
	
	printf("Write pipe: %d\n", pong_pipe);

	while(input != 113){
		scanf("%d", &input);
		write(pong_pipe, &input, sizeof(input)); 
	}

	return 0;
}
