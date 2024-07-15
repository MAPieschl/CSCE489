/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
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

struct Command
{
    char raw[INPUT_BUFFER_SIZE];
    char parsed[MAX_NUM_ARGS][MAX_SIZE_ARGS];
};

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

void run_cmd()
{
    struct Command current_cmd;
    char old_chars;
    pid_t pid_child;
    int child_status;

    // Request & read command
    printf("\n>> ");
    scanf("%[^\n]", current_cmd.raw);
    
    // The following code snippet is commonly reused for clearing the input buffer. Sites utilized for finding and understanding the snippet are:
    //  https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/
    //  https://stackoverflow.com/questions/40554617/while-getchar-n

    // Clear input buffer for next iteration
    while ((old_chars = getchar()) != '\n')
    ;

    // Parse command
    parse_cmd(current_cmd.raw, current_cmd.parsed);
    
    pid_child = fork();
    
    if(pid_child != 0){
		for(int i = 0; i < MAX_NUM_ARGS; i++){
			if(strcmp(current_cmd.parsed[i], "&") == 0){
				printf("\n[] %d\n", pid_child);
				return;
			}
		}
		
		// The wait() flow below was derived from:
		// https://www.geeksforgeeks.org/wait-system-call-c/
		
    	wait(&child_status);
    	
    	if(WIFEXITED(child_status)){
    	
    		if(child_status == 0){
    			printf("\n'%s' completed successfully by PID %d!\n", current_cmd.parsed[0], pid_child);
    		}
    		else{
    			printf("\n'%s' failed by PID %d :(\n", current_cmd.parsed[0], pid_child);
    		}
    	}
    	
    	return;
    }

    for (int cmd_index = 0; cmd_index < (int)NUM_CMDS; cmd_index++)
    {
        if (strcmp(AVAILABLE_CMDS[cmd_index], current_cmd.parsed[0]) == 0)
        {
            select_command(cmd_index, current_cmd.parsed);
            exit(1);
        }
        else if (cmd_index >= (int)NUM_CMDS - 1)
        {
            printf("\nCommand not found...\n");
            return;
        }
    }
}

int main()
{
    pid_t pid_shell = getpid();
    
    printf("Welcome to myShell! Your current process ID is %d", pid_shell);
    
    for (int i = 0; i < 10; i++)
    {
        run_cmd();
    }

    return 0;
}
