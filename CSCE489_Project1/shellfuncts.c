/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 *
 *************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include "shellfuncts.h"

/*************************************************************************************
 * hello - sends hello world to the user! I'm doing proper function commenting so future
 *         coders might not find my code as painful.
 *
 *		Params:	param1 - I explain my parameters, like this is 1 for American, 2 for
 *                      Australian
 *
 *		Returns: always returns 1, no matter what. Fairly useless.
 *
 *************************************************************************************/

void select_command(int cmd_num, char cmd[MAX_NUM_ARGS][MAX_SIZE_ARGS]){
	switch(cmd_num){
		case 0:
			create(cmd[1]);
			break;
		case 1:
			update(cmd[1], cmd[2], cmd[3]);
			break;
		case 2:
			list(cmd[1]);
			break;
		case 3:
			dir();
			break;
		case 4:
			halt();
			break;
	}
}

/*************************************************************************************
 * hello - sends hello world to the user! I'm doing proper function commenting so future
 *         coders might not find my code as painful.
 *
 *    Params:  param2 - not a very good parameter name - something better might be
 *                      say, msgstr or sendtext
 *
 *************************************************************************************/

void create(char name[MAX_SIZE_ARGS]){
	FILE *newFile;
	newFile = fopen(name, "r");
	
	if(newFile == NULL){
		newFile = fopen(name, "w");
		fclose(newFile);
		exit(0);
	}
	else{
		printf("\nFile already exists!");
		fclose(newFile);
		exit(1);
	}
}

/*************************************************************************************
 * hello - sends hello world to the user! I'm doing proper function commenting so future
 *         coders might not find my code as painful.
 *
 *    Params:  param2 - not a very good parameter name - something better might be
 *                      say, msgstr or sendtext
 *
 *************************************************************************************/

void update(char name[MAX_SIZE_ARGS], char number[MAX_SIZE_ARGS], char text[MAX_SIZE_ARGS]){

	printf("%s", text);

	int numMaxIndex;
	int tempVal;
	int numIterations = 0;
	char tempString[MAX_SIZE_ARGS];
	int tempStringIndex = 0;

	FILE *modFile;
	modFile = fopen(name, "a");
	
	for (int i = 0; i < MAX_SIZE_ARGS; i++){
		if(number[i] != '\0'){
			numMaxIndex = i;
		}
		else{
			break;
		}
	}
	
	for (int i = 0; i <= numMaxIndex; i++){
		tempVal = (number[i] - 0x30)*pow(10.0, (double) numMaxIndex - i);
		numIterations += tempVal;
	}
	
	for (int i = 0; i < MAX_SIZE_ARGS; i++){
		if(text[i] == '\0'){
			break;
		}
		else if(text[i] != '"'){
			tempString[tempStringIndex] = text[i];
			tempStringIndex++;
		}
	}
	
	for (int i = 0; i < numIterations; i++){
		fprintf(modFile, "%s\n", tempString);
	}
	
	exit(0);
}

/*************************************************************************************
 * hello - sends hello world to the user! I'm doing proper function commenting so future
 *         coders might not find my code as painful.
 *
 *    Params:  param2 - not a very good parameter name - something better might be
 *                      say, msgstr or sendtext
 *
 *************************************************************************************/

void list(char name[MAX_SIZE_ARGS]){
	printf("%s", name);
}

/*************************************************************************************
 * hello - sends hello world to the user! I'm doing proper function commenting so future
 *         coders might not find my code as painful.
 *
 *    Params:  param2 - not a very good parameter name - something better might be
 *                      say, msgstr or sendtext
 *
 *************************************************************************************/

void dir(){
	exit(0);
}

/*************************************************************************************
 * hello - sends hello world to the user! I'm doing proper function commenting so future
 *         coders might not find my code as painful.
 *
 *    Params:  param2 - not a very good parameter name - something better might be
 *                      say, msgstr or sendtext
 *
 *************************************************************************************/

void halt(){
	pid_t parent_pid = getppid();
	kill(parent_pid, SIGKILL);
	exit(0);
}
