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
	int error_check = 0;
	switch(cmd_num){
		case 0:
			error_check = file_name_check(cmd[1]);
			if(error_check != 0){exit(1);}
			error_check = file_exists_check(cmd[1]);
			if(error_check != 0){
				printf("\nFile already exists. Please choose a different name.");
				exit(1);
			}
			
			create(cmd[1]);
			break;
		case 1:
			error_check = file_exists_check(cmd[1]);
			if(error_check != 1){
				printf("\nFile does not exist. Please use - create <file name> - to create a file prior to attempting to write to the file.");
				exit(1);
			}
			
			error_check = number_format_check(cmd[2]);
			if(error_check < 0){exit(1);}
			
			update(cmd[1], error_check, cmd[3]);
			break;
		case 2:
			error_check = file_exists_check(cmd[1]);
			if(error_check != 1){
				printf("\nFile does not exist. Please use - create <file name> - to create a file prior to attempting to write to the file.");
				exit(1);
			}
		
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
 * ERROR CHECKING FUNCTIONS
 *************************************************************************************/

int file_name_check(char name[MAX_SIZE_ARGS]){
	for(int i = 0; i < MAX_SIZE_ARGS; i++){
		if(name[i] == '\0'){
			break;
		}
		else if(name[i] == '/' || name[i] == 0){
			printf("\nCould not create file - illegal symbols present in %s", name);
			return 1;
		}
	}
	return 0;
}

int file_exists_check(char name[MAX_SIZE_ARGS]){
	int file_exists = 0;

	FILE *check_file;
	check_file = fopen(name, "r+");
	
	if(check_file != NULL){
		file_exists = 1;
	}
	else{
		file_exists = 0;
	}
	
	//fclose currently skipped - it seems to crash the process just after the fopen() command for an unknown reason
	//fclose(check_file);
	
	return file_exists;
}

int number_format_check(char number[MAX_SIZE_ARGS]){
	int num_max_index;
	int temp_val;
	int num_out = 0;
	
	for (int i = 0; i < MAX_SIZE_ARGS; i++){
		if(number[i] != '\0'){
			num_max_index = i;
		}
		else{
			break;
		}
	}
	
	for (int i = 0; i <= num_max_index; i++){
		temp_val = number[i] - 0x30;
		
		if((temp_val >= 0) && (temp_val <= 9)){
			temp_val *= pow(10.0, (double) num_max_index - i);
			num_out += temp_val;
		}
		else{
			printf("\nUnable to proceed. Please ensure the number of iterations (argument 2) is a positive integer value.");
			return -1;
		}
	}
	
	return num_out;
}


/*************************************************************************************
 * TERMINAL FUNCTIONS
 *************************************************************************************/


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
	new_file = fopen(name, "w");
	fclose(new_file);
	
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

void update(char name[MAX_SIZE_ARGS], int number, char text[MAX_SIZE_ARGS]){

	char temp_string[MAX_SIZE_ARGS];
	int temp_string_index = 0;
	int inside_quotes = 0;

	FILE *mod_file;
	mod_file = fopen(name, "a");
	
	for (int i = 0; i < MAX_SIZE_ARGS; i++){
		if(text[i] == '"' && inside_quotes != 0){
			break;
		}
		else if(text[i] != '"'){
			temp_string[temp_string_index] = text[i];
			temp_string_index++;
		}
	}
	
	for (int i = 0; i < number; i++){
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
