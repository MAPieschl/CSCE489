#include <stdlib.h>
#include <iostream>
#include "animation.h"

int main(int argv, char *argc[]){

	int user_input = 0;
	Animation *animate = NULL;

	if(sizeof(uintptr_t) == sizeof(unsigned long)){
		*animate = strtoul(argc[1], nullptr, 10);
	}
	
	else{
		return 1;
	}
	
	std::cout << animate->paddle_position;
	//Animation *animate = st
	
	while(user_input != 113){
		std::cin >> user_input;
	}

	return 0;
}
