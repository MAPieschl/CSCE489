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
#include <unistd.h>
#include <string.h>
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

    for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        if (raw_cmd[i] != '\0' && raw_cmd[i] != ' ')
        {
            temp_arg[char_index] = raw_cmd[i];
            char_index++;
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

    // Request & read command
    printf("\n>> ");
    scanf("%[^\n]", current_cmd.raw);

    // The following code snippet is commonly reused for clearing the input buffer. Sites utilized for finding and understanding the snippet are:
    //  https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/
    //  https://stackoverflow.com/questions/40554617/while-getchar-n

    // Clear input buffer for next iteration
    char old_chars;
    while ((old_chars = getchar()) != '\n')
        ;

    // Parse command
    parse_cmd(current_cmd.raw, current_cmd.parsed);

    for (int cmd_index = 0; cmd_index < (int)NUM_CMDS; cmd_index++)
    {
        if (strcmp(AVAILABLE_CMDS[cmd_index], current_cmd.parsed[0]) == 0)
        {
            select_command(cmd_index);
            return;
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
    for (int i = 0; i < 10; i++)
    {
        run_cmd();
    }

    return 0;
}
