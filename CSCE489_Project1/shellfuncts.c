/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 *
 *************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <string.h>
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
	FILE *new_file;
	new_file = fopen(name, "r");
	
	if(new_file == NULL){
		new_file = fopen(name, "w");
		fclose(new_file);
		exit(0);
	}
	else{
		printf("\nFile already exists!");
		fclose(new_file);
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

	int num_max_index;
	int temp_val;
	int num_iterations = 0;
	char temp_string[MAX_SIZE_ARGS];
	int temp_string_index = 0;
	int inside_quotes = 0;

	FILE *mod_file;
	mod_file = fopen(name, "a");
	
	for (int i = 0; i < MAX_SIZE_ARGS; i++){
		if(number[i] != '\0'){
			num_max_index = i;
		}
		else{
			break;
		}
	}
	
	for (int i = 0; i <= num_max_index; i++){
		temp_val = (number[i] - 0x30)*pow(10.0, (double) num_max_index - i);
		num_iterations += temp_val;
	}
	
	for (int i = 0; i < MAX_SIZE_ARGS; i++){
		if(text[i] == '"' && inside_quotes != 0){
			break;
		}
		else if(text[i] != '"'){
			temp_string[temp_string_index] = text[i];
			temp_string_index++;
		}
	}
	
	for (int i = 0; i < num_iterations; i++){
		fprintf(mod_file, "%s\n", temp_string);
	}
	
	sleep(strlen(temp_string)/5);
	
	fclose(mod_file);
	
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

	// Ensure file exists
	FILE *read_file;
	read_file = fopen(name, "r");
	
	// If file doesn't exist, warn user and kill process with error
	if(read_file == NULL){
		printf("\nFile does not exist. Execute - create <file name> - to create a file.\n");
		exit(1);
	}
	
	fclose(read_file);
	
	// Run cat commadn to list file
	printf("\n\n");
	execl("/bin/cat", "cat %s", name, NULL);
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
	execl("/bin/ls", "ls", NULL);
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
