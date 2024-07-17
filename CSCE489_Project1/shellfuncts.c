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
 * select_command - parses command and applies error checking to arguments
 *
 *		Params:	cmd_num - index of command in AVAILABLE_CMDS
 			cmd[][] - command.parsed
 *
 *		Returns: void
 *
 *************************************************************************************/

void select_command(int cmd_num, char cmd[MAX_NUM_ARGS][MAX_SIZE_ARGS]){
	int error_check = 0;
	int return_key = 0;
	switch(cmd_num){
		case CREATE:
			error_check = file_name_check(cmd[1]);
			if(error_check != 0){exit(1);}
			error_check = file_exists_check(cmd[1]);
			if(error_check != 0){
				printf("\nFile already exists. Please choose a different name.");
				exit(1);
			}
			
			create(cmd[1]);
			break;
		case UPDATE:
			error_check = file_exists_check(cmd[1]);
			if(error_check != 1){
				printf("\nFile does not exist. Please use - create <file name> - to create a file prior to attempting to write to the file.");
				exit(1);
			}
			
			error_check = number_format_check(cmd[2]);
			if(error_check < 0){exit(1);}
			
			return_key = number_format_check(cmd[MAX_NUM_ARGS - 1]);
			if(return_key < 0){exit(1);}
			
			update(cmd[1], error_check, cmd[3], return_key);
			break;
		case LIST:
			error_check = file_exists_check(cmd[1]);
			if(error_check != 1){
				printf("\nFile does not exist. Please use - create <file name> - to create a file prior to attempting to write to the file.");
				exit(1);
			}
			
			return_key = number_format_check(cmd[MAX_NUM_ARGS - 1]);
			if(return_key < 0){exit(1);}
		
			list(cmd[1]);
			break;
		case DIR:
			dir();
			break;
		case HALT:
			halt();
			break;
	}
}

/*************************************************************************************
 * ERROR CHECKING FUNCTIONS
 *************************************************************************************/

/*************************************************************************************
 * file_name_check - 	ensures file name provided meets UNIX specifications IAW:
 * 
 *			https://learning.oreilly.com/library/view/learning-the-unix/1565923901/ch04s02.html#:~:text=Filenames%20are%20usually%20made%20of,%2C%20dot%2C%20and%20underscore%20characters.
 *
 *			The function does not check for file name length; it is only
 *			limited by MAX_SIZE_ARGS (defualt - 64 characters)
 *
 *		Params:	name[] - file name of size MAX_SIZE_ARGS
 *
 *		Returns: int - 	0 -> file name is acceptable
 				1 -> file name is unacceptable
 *
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

/*************************************************************************************
 * file_exists_check - 	checks to see if file exists
 *
 *		Params:	name[] - file name of size MAX_SIZE_ARGS
 *
 *		Returns: int - 	0 -> file does not exist
 *						1 -> file exists
 *
 *************************************************************************************/

int file_exists_check(char name[MAX_SIZE_ARGS]){
	int file_exists = 0;

	FILE *check_file;
	check_file = fopen(name, "r");
	
	if(check_file != NULL){
		file_exists = 1;
		fclose(check_file);
	}
	else{
		file_exists = 0;
	}
	
	return file_exists;
}

/*************************************************************************************
 * number_format_check - takes a number as a string, ensures it is a positive integer value,
 *			 and returns the number as an integer
 *
 *		Params:	number[] - string value for number
 *
 *		Returns: int - 	number converted to type integer
 *
 *************************************************************************************/

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
 * create -	creates a file in teh current directory using the file name provided
 *
 *		Params:	name[] - file name of size MAX_SIZE_ARGS
 *
 *		Returns: void (exits process with code 0)
 *
 *************************************************************************************/
 
void create(char name[MAX_SIZE_ARGS]){
	FILE *new_file;
	new_file = fopen(name, "w");
	fclose(new_file);
	
	exit(0);
}

/*************************************************************************************
 * update -	updates a file given the file name, the number of times a string shall
 *		be added to the file, and the string to be added
 *
 *		Note:  If the string to be added to the file is placed in quotations,
 *		there is no word limit. If quotations are not used, only the first word
 *		in the argument will be added to the file.
 *
 *		Params:	name[] - file name
 *			number - number of times argument 3 shall be appended to the file
 			text[] - word or phrase to be appended to the file
 *
 *		Returns: void (exits process with code 0)
 *
 *************************************************************************************/

void update(char name[MAX_SIZE_ARGS], int number, char text[MAX_SIZE_ARGS], int key){

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
		fflush(mod_file);
		sleep(strlen(temp_string)/5);
	}
	
	fclose(mod_file);
	
	exit(key);
}

/*************************************************************************************
 * list - print a file to the shell
 *
 *    Params:  name[] - file name of size MAX_SIZE_ARGS
 *
 *	  Returns: void (exits process with code 0)
 *
 *************************************************************************************/

void list(char name[MAX_SIZE_ARGS]){
	// Run cat command to list file
	printf("\n\n");
	execl("/bin/cat", "cat %s", name, NULL);
	exit(1);
}

/*************************************************************************************
 * dir - lists contents of a directory in the shell
 *
 *    Params:  name[] - file name of size MAX_SIZE_ARGS
 *
 *	  Returns: void (exits process with code 0)
 *
 *************************************************************************************/

void dir(){
	execl("/bin/ls", "ls", NULL);
	exit(1);
}

/*************************************************************************************
 * halt - close shell and all associated processes
 *
 *    Params:  None
 *
 *	  Returns: void (exits process with code 0)
 *
 *************************************************************************************/

void halt(){
	pid_t parent_pid = getppid();
	kill(parent_pid, SIGKILL);
	exit(1);
}
