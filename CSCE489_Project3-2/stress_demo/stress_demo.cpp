#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#define STRESSOR_LAUNCH_US 1000000

const char *stressor_terminal[] = {"gnome-terminal --title stress-ng -- stress-ng --cpu 1"};			

int main(int argv, char *argc[]) {

	if(argv < 2){
		printf("Please input number of stressors.\n");
		return 1;
	}
	
	int num_stressors = std::stoi(argc[1], nullptr, 10);

	for(int i = 0; i < num_stressors; i++){
		system(*stressor_terminal);
		
		usleep(STRESSOR_LAUNCH_US);
	}
	
	return 0;
}
