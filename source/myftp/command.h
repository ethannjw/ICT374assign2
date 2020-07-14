/*  File:		command.h for assignment 2 myftp (client side)
 *  Author:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose:		Contains the core functions for the client operation. 
 *  Assumptions:	
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "token.h"

#define MAX_NUM_COMMANDS    1000
#define MAX_NUM_CHAR        1000

// define the opcodes for server comms
#define OP_PUT	'P'
#define OP_GET	'G'
#define OP_PWD	'W'
#define OP_FDR	'F'
#define OP_CD	'C'
#define OP_DATA	'D'

// acknowledgement codes from the server
#define SUCCESS_CODE	'0'
#define ERROR_CODE	'1'


struct CommandStruct {
	int first;      	// index to the first token in the array "token" of the command
	int last;   	// index to the last token in the array "token" of the command
	char **argv;        // an array of tokens that forms a command
	int argc;	// an int indicating the number of arguments in the array

};

// command type
typedef struct CommandStruct Command;


// purpose:
//		separate the list of token from array "token" into a sequence of commands, to be
//		stored in the array "command".
//
// return:
//		1) the number of commands found in the list of tokens, if successful, or
//		2) -1, if the the array "command" is too small.
//		3) < -1, if there are following syntax errors in the list of tokens.
//			a) -2, if any two successive commands are separated by more than one command separator
//			b) -3, the first token is a command separator
//			c) -4, the last command is followed by command separator "|"
//
// assume:
//		the array "command" must have at least MAX_NUM_COMMANDS number of elements
//
//  note:
//		1) the last command may be followed by "&", or ";", or nothing. If nothing is
//		   followed by the last command, we assume it is followed by ";".
//		2) if return value, nCommands >=0, set command[nCommands] to NULL,
//
int separateCommands(char *token[], Command command[]);

// purpose:
//		scans the array from token[cp->first] to token[cp->last], looking for the standard input redirection
//		symbol "<" and the standard output redirection symbol ">". Once found, the token following the
//		redirection symbol is treated as the redirection file name and is assigned to either cp->stdin_file
//		if it is the standard input redirection, or to stdout_file if it is the standard output redirection.
//
// return:	void
//
// assume:
//		the array "command" must have at least MAX_NUM_COMMANDS number of elements

/** Purpose:	To obtain the command from the user and store it into the command structure for further
 *  		processing
 *  Return:	void
 *  
*/
void cmd_prompt(Command &commandArray[])
