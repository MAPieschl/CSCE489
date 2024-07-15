/*************************************************************************************
 * shellfuncts.h - header file for your shell functions
 *
 *************************************************************************************/

/*************************************************************************************
 *  Constant definitions
 *************************************************************************************/
 
// Maximum size of an input is 256 characters
#define INPUT_BUFFER_SIZE 256

// Maximum number of arguments per command (including the command)
#define MAX_NUM_ARGS 5

// Maximum size of each argument (in characters)
#define MAX_SIZE_ARGS 64


/*************************************************************************************
 *  Function definitions
 *************************************************************************************/

void select_command(int cmd_num, char cmd[MAX_NUM_ARGS][MAX_SIZE_ARGS]);
int file_name_check(char name[MAX_SIZE_ARGS]);
int file_exists_check(char name[MAX_SIZE_ARGS]);
int number_format_check(char number[MAX_SIZE_ARGS]);
void create(char name[MAX_SIZE_ARGS]);
void update(char name[MAX_SIZE_ARGS], int number, char text[MAX_SIZE_ARGS]);
void list(char name[MAX_SIZE_ARGS]);
void dir();
void halt();
