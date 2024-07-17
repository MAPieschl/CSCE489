/*************************************************************************************
 * myshell - Student code for Project 1 of CSCE 489 
 * BY:		Capt Mike Pieschl
 * DATE: 	19 July 2024
 *
 * ORGANIZATIONAL NOTES: All shell interface functionality is contained within this C
 * 			 program. All commands are defined in shellFuncts.c. 
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shellfuncts.h"


// Commands available for execution
//	- If a command function is added, it must be added to this list to be
//	  recognized by the shell interface
const char AVAILABLE_CMDS[5][7] = {"create", "update", "list", "dir", "halt"};

// Computed number of commands available
const size_t NUM_CMDS = sizeof(AVAILABLE_CMDS);

// Commands not available for background processing
const char NO_BG_PROCESS[3][7] = {"list", "dir", "halt"};

// Commands that view or modify existing files (used to protect files)
const char FILE_MOD_CMDS[2][7] = {"update", "list"};

// Tracks number of children if background processes are initiated
int child_counter = 0;

// Tracks files actively being processed (and therefore locked for used)
//	- Stores up to five files (overflow will cause the tracked file to become untracked
//  - Use of this protection feature requires that the file name be arguement 1 (after command)
char active_files[MAX_ACTIVE_FILES][MAX_SIZE_ARGS];

// Tracks next empty position in active_files
int active_files_indexer = 0;

struct Command
{
    char raw[INPUT_BUFFER_SIZE];
    char parsed[MAX_NUM_ARGS][MAX_SIZE_ARGS];
};

/*************************************************************************************
 * parse_command - parses command and applies error checking to arguments
 *
 *		Params:	raw_cmd - full string output from user input (stored in command struct)
 				parsed_cmd - parsed output (stored in command struct)
 *
 *		Returns: void
 *
 *************************************************************************************/

void parse_cmd(char raw_cmd[INPUT_BUFFER_SIZE], char parsed_cmd[MAX_NUM_ARGS][MAX_SIZE_ARGS])
{
    const char temp_str_fill[64];
    char temp_arg[64];
    int char_index = 0;
    int arg_index = 0;
    int inside_quotes = 0;

    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        if ((raw_cmd[i] != '\0' && raw_cmd[i] != ' ') || inside_quotes != 0)
        {
        	// Add value to temporary argument string
            temp_arg[char_index] = raw_cmd[i];
            char_index++;
            
            // Quotation blocker to prevent parsing inside of quotation marks
            if(raw_cmd[i] == '"'){
            	if(inside_quotes == 0){inside_quotes = 1;}
            	else {inside_quotes = 0;}
            }
        }
        else if (raw_cmd[i] == ' ')
        {
            // Copy completed arg into parsed_cmd
            strcpy(parsed_cmd[arg_index], temp_arg);

            // Ensure terminating character following string
            parsed_cmd[arg_index][char_index] = '\0';

            // Reset temp_arg and char_index for next argument
            strcpy(temp_arg, temp_str_fill);
            char_index = 0;
            arg_index++;
        }
        else
        {
            // Copy completed arg into parsed_cmd
            strcpy(parsed_cmd[arg_index], temp_arg);

            // Ensure terminating character following string
            parsed_cmd[arg_index][char_index] = '\0';

            // EOM - Return from parser
            return;
        }
    }
    return;
}

/*************************************************************************************
 * run_command - handles user input, process forking, child exit conditions, and calls 
 *				 command functions
 *
 *		Params:	(none)
 *
 *		Returns: void
 *
 *************************************************************************************/


void run_cmd()
{

    struct Command current_cmd = {0};
    char old_chars;
    pid_t pid_child;
    int child_status;
    
    // Request & read command
    printf("\n>> ");
    scanf("%[^\n]", current_cmd.raw);
    
    // Catch children and update child counter following background process execution
    for(int i = child_counter; i > 0; i--){
    	waitpid(-1, &child_status, WNOHANG);
    	if(WIFEXITED(child_status)){
    		if(child_status >= 256*MAX_ACTIVE_FILES){
    			strcpy(active_files[child_status/256 - MAX_ACTIVE_FILES], "");
    			child_counter--;
    		}
    	}
    }
    
    // Clean up any remaining active files if all children are complete
    if(child_counter < 1){
		for(int i = 0; i < MAX_ACTIVE_FILES; i++){
			strcpy(active_files[i], "");
		}
	}
    
    // The following code snippet is commonly reused for clearing the input buffer. Sites utilized for finding and understanding the snippet are:
    //  https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/
    //  https://stackoverflow.com/questions/40554617/while-getchar-n

    // Clear input buffer for next iteration
    while ((old_chars = getchar()) != '\n')
    ;

    // Parse command
    parse_cmd(current_cmd.raw, current_cmd.parsed);
    
    // Check if new command requires file lockout
    for(int i = 0; i < (int) sizeof(FILE_MOD_CMDS); i++){
    
		if(strcmp(current_cmd.parsed[0], FILE_MOD_CMDS[i]) == 0){
			
			// Catch and return if file is already locked by another process
			for(int i = 0; i < MAX_ACTIVE_FILES; i++){
				if(strcmp(current_cmd.parsed[1], active_files[i]) == 0){
					printf("\n%s is currently in use. Please wait and try again later.\n", current_cmd.parsed[1]);
					return;
				}
			}
			
			// Store file name in active_files
			strcpy(active_files[active_files_indexer], current_cmd.parsed[1]);
			
			// Store encoded active_files index in last argument of current_cmd.parsed
			// - This value is the key to unlock the file on process exit
			sprintf(current_cmd.parsed[MAX_NUM_ARGS - 1], "%d", active_files_indexer + MAX_ACTIVE_FILES);
			
			// Increment indexer no larger than array length
			active_files_indexer = MAX_ACTIVE_FILES % (active_files_indexer + 1);
			active_files_indexer++;
    	}
    }
    
    // Fork and increment child counter
    pid_child = fork();
    child_counter++;
    
    // Parent conditional
    if(pid_child != 0){
    
		// Search for background process signifier
		for(int i = 0; i < MAX_NUM_ARGS; i++){
		
			if(strcmp(current_cmd.parsed[i], "&") == 0){
			
				// Set flag command is on the "no background process" list
				int background_flag = 0;
				for(int i = 0; i < (int) sizeof(NO_BG_PROCESS); i++){
					if(strcmp(current_cmd.parsed[0], NO_BG_PROCESS[i]) == 0){
						printf("WARNING:  %s ignores '&' arguments.\n", current_cmd.parsed[0]);
						background_flag = 1;
					}
				}
				
				// If background process is allowed, print # of active children and pid
				if(background_flag == 0){
					// Standard linux printout for background process
					printf("\n[%d] %d\n", child_counter, pid_child);
					return;
				}
			}
		}
		
		// The wait() flow below was derived from:
		// https://www.geeksforgeeks.org/wait-system-call-c/ and modified using info
		// from `man wait` to specify child pid for background process execution

    	waitpid(pid_child, &child_status, 0);
    	
    	if(WIFEXITED(child_status)){
    		child_counter--;
    	
    		// Successful child execution (exit(0))
    		if(child_status == 0){
    			printf("\n'%s' completed successfully by PID %d!\n", current_cmd.parsed[0], pid_child);
    		}
    		
    		// Successful with file unlock
    		else if(child_status >= 256*MAX_ACTIVE_FILES){
    			strcpy(active_files[child_status/256 - MAX_ACTIVE_FILES], "");
    			printf("\n'%s' completed successfully by PID %d!\n", current_cmd.parsed[0], pid_child);
    		}
    		
    		// Unsuccessful child execution (exit(!0))
    		else{
    			printf("\n'%s' failed by PID %d :(\n", current_cmd.parsed[0], pid_child);
    		}
    	}
    	
    	return;
    }

	// Child process - execute command
    for (int cmd_index = 0; cmd_index < (int)NUM_CMDS; cmd_index++)
    {
    	// Catch good commands
        if (strcmp(AVAILABLE_CMDS[cmd_index], current_cmd.parsed[0]) == 0)
        {
        	// Find command
            select_command(cmd_index, current_cmd.parsed);
            
            // Catch errored execution (exit(0) is in each individual command function)
            exit(1);
        }
        
        // Catch bad commands
        else if (cmd_index >= (int)NUM_CMDS - 1)
        {
            printf("\nCommand not found...\n");
            
            exit(1);
        }
    }
}

int main()
{
	// Store parent PID
    pid_t pid_shell = getpid();
    
    printf("Welcome to myShell! Your current process ID is %d", pid_shell);
    
    // Runtime loop
    while(1)
    {
        run_cmd();
    }

    return 0;
}
