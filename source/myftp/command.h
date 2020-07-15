/*  File:		command.h for assignment 2 myftp (client side)
 *  Author:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose:		Contains the core functions for the client operation.
 *  Assumptions:
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../stream.h"

#define MAX_NUM_COMMANDS    1000
#define MAX_NUM_CHAR        1000
#define MAX_TOKEN           100
#define tokenSep            " \t\n"

// define the opcodes for server comms
#define OP_PUT	'P'
#define OP_GET	'G'
#define OP_PWD	'W'
#define OP_FDR	'F'
#define OP_CD	'C'
#define OP_DATA	'D'

// define the commands from user
#define CMD_PWD "pwd"

// acknowledgement codes from the server
#define SUCCESS_CODE	'0'
#define ERROR_CODE	'-1'

// command type containing the command and argument
struct CommandStruct {
	char cmd[MAX_NUM_CHAR];	// command
	char arg[MAX_NUM_CHAR];	// argument of the command

};
typedef struct CommandStruct Command;


/** Purpose:	To obtain the command from the user and store it into the command structure for further
 *  Return:	void
 *
*/
void cmd_prompt(int socket_desc);


/** Purpose:	To send and receive the current working directory of the server
 *  Param:	socket descriptor of connection, command token
 *  Return:	void
 *
*/
void cli_pwd(int socket_desc);
