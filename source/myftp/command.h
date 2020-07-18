/*  File:		command.h for assignment 2 myftp (client side)
 *  Author:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose:		Contains the core functions for the client operation.
 *  Assumptions:
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../stream.h"
#include <dirent.h>

#define MAX_NUM_COMMANDS    1000
#define MAX_NUM_CHAR        1000
#define MAX_TOKEN           100
#define BUF_SIZE			256
#define tokenSep            " \t\n"

// define the opcodes for server comms
#define OP_PUT	'P'
#define OP_GET	'G'
#define OP_PWD	'W'
#define OP_FDR	'F'
#define OP_CD	'C'
#define OP_DATA	'D'

// define the commands from user
// server commands
#define CMD_PWD 	"pwd"
#define CMD_FDR		"dir"
#define CMD_CD  	"cd"
#define CMD_PUT		"put"
#define CMD_GET		"get"

// local commands
#define CMD_LCD		"lcd"
#define CMD_LFDR	"ldir"
#define CMD_LPWD	"lpwd"
#define CMD_HELP	"help"

// acknowledgement codes from the server
#define SUCCESS_CODE	'0'
#define ERROR_CODE	    '1'

/* ACK codes for PUT */
#define FILE_EXIST '1'
#define FILE_NOT_EXIST '2'

/* ACK codes for DIR */
#define EXCEED_LENGTH   'L'

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

/** Purpose:	To receive and print out the files present in current working directory of the server
 *  Param:	socket descriptor of connection, command token
 *  Return:	void
 *
*/
void cli_fdr(int socket_desc);

/** Purpose:	To send and upload files to the server
 *  Param:	socket descriptor of connection, command token, filename
 *  Return:	void
 *
*/
void cli_put(int socket_desc, char *filename);

/** Purpose:	To change directory of server
 *  Param:	socket descriptor of connection, filepath
 *  Return:	void
 *
*/
void cli_cd(int socket_desc, char* file_path);

/** Purpose:	To change directory of server
 *  Param:	socket descriptor of connection, filepath
 *  Return:	void
 *
*/
void cli_lcd(char * cmd_path);

/** Purpose:	To print out the files present in current working directory of client
 *  Param:	socket descriptor of connection, filepath
 *  Return:	void
 *
*/
void cli_lfdr();

/** Purpose:	Print current working directory of client
 *  Param:	socket descriptor of connection, filepath
 *  Return:	void
 *
*/
void cli_lpwd();

/** Purpose:	To download named file from server to client
 *  Param:	socket descriptor of connection, filepath
 *  Return:	void
 *
*/
void cli_get(int socket_desc, char *file_name);

/** Purpose:	To display the help menu
 *  Param:	Void
 *  Return:	void
 *
*/
void cli_help();