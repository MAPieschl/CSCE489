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

void select_command(int cmd_num);
void halt();
