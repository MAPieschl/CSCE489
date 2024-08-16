#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "rt_demo.h"

int main(){
	
	int ret = mkfifo(fifo_path, 0666);
	printf("New FIFO: %d\n", ret);
	
	system("gnome-terminal --title Pong -- ./../pong/pong");
	system("gnome-terminal --title Controller -- ./../pong_controller/pong_controller");
	system("gnome-terminal --title Monitor -- top");

	return 0;
}
