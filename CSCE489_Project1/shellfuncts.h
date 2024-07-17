/*************************************************************************************
 * shellfuncts.h - header file for your shell functions
 *
 *************************************************************************************/

#ifndef SHELL_FUNCTS_H
#define SHELL_FUNCTS_H

/*************************************************************************************
 *  Constant definitions
 *************************************************************************************/
 
// Maximum size of an input is 256 characters
#define INPUT_BUFFER_SIZE 256

// Maximum number of arguments per command (including the command)
#define MAX_NUM_ARGS 5

// Maximum size of each argument (in characters)
#define MAX_SIZE_ARGS 64

// Used for convenience to index AVAILABLE_CMDS in shellfuncts.c
//	- Any new commands must be added to:
//		-- AVAILABLE_CMDS (myshell.c)
//		-- AVAILABLE_CMDS_INDEXER (shellfuncts.h)
//		-- select_command() (shellfncts.c)
enum AVAILABLE_CMDS_INDEXER {CREATE, UPDATE, LIST, DIR, HALT};


/*************************************************************************************
 *  Function definitions
 *************************************************************************************/

void select_command(int cmd_num, char cmd[MAX_NUM_ARGS][MAX_SIZE_ARGS]);
int file_name_check(char name[MAX_SIZE_ARGS]);
int file_exists_check(char name[MAX_SIZE_ARGS]);
int number_format_check(char number[MAX_SIZE_ARGS]);
void create(char name[MAX_SIZE_ARGS]);
void update(char name[MAX_SIZE_ARGS], int number, char text[MAX_SIZE_ARGS], int key);
void list(char name[MAX_SIZE_ARGS], int key;
void dir();
void halt();

#endif
