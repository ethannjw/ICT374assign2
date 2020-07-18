/*  File:			command.h for assignment 2 myftp (client side)
 *  Author:			Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Date:			25th July 2020
 *  Purpose:		Contains the core functions for the client operation.
 *  Assumptions:
*/

#include <stdlib.h>        /* exit(), free() */
#include <stdio.h>         /* printf()  */
#include <sys/types.h>     /* pid_t, u_long, u_short */
#include <unistd.h>        /* read(), write() */
#include <string.h>        /* strlen(), strcmp() etc */
#include <sys/stat.h>      /* fstat(), lstat(), stat() */
#include <fcntl.h>         /* manipulate file descriptor */
#include <dirent.h>	       /* DIR */

#include "../stream.h"     /* head file for stream read and write */

/* OP CODES for the network specification */
#define OP_PUT	'P'
#define OP_GET	'G'
#define OP_PWD	'W'
#define OP_FDR	'F'
#define OP_CD	'C'
#define OP_DATA	'D'

/* ACK codes for each network specification */
#define SUCCESS_CODE	'0'
#define ERROR_CODE	    '1'

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

/* ACK codes for PUT and GET */
#define FILE_EXIST 'E'
#define FILE_NOT_EXIST 'N'

/* ACK codes for DIR */
#define EXCEED_LENGTH   'L'

#define MAX_NUM_COMMANDS    1000
#define MAX_NUM_CHAR        1000
#define MAX_TOKEN           100
#define BUF_SIZE			256
#define tokenSep            " \t\n"

// command type containing the command and argument
struct CommandStruct {
	char cmd[MAX_NUM_CHAR];	// command
	char arg[MAX_NUM_CHAR];	// argument of the command

};
typedef struct CommandStruct Command;


/** Purpose:	To obtain the command from the user and store it into the command structure for further
 *  Param:		socket descriptor of connection
 *  Return:		void
 *
*/
void cmd_prompt(int socket_desc);


/** Purpose:	To send and receive the current working directory of the server
 *  Param:		socket descriptor of connection, command token
 *  Return:		void
 *
*/
void cli_pwd(int socket_desc);

/** Purpose:	To receive and print out the files present in current working directory of the server
 *  Param:		socket descriptor of connection, command token
 *  Return:		void
 *
*/
void cli_fdr(int socket_desc);

/** Purpose:	To send and upload files to the server
 *  Param:		socket descriptor of connection, command token, filename
 *  Return:		void
 *
*/
void cli_put(int socket_desc, char *filename);

/** Purpose:	To change directory of server
 *  Param:		socket descriptor of connection, filepath
 *  Return:		void
 *
*/
void cli_cd(int socket_desc, char* file_path);

/** Purpose:	To change directory of server
 *  Param:		socket descriptor of connection, filepath
 *  Return:		void
 *
*/
void cli_lcd(char * cmd_path);

/** Purpose:	To print out the files present in current working directory of client
 *  Param:		socket descriptor of connection, filepath
 *  Return:		void
 *
*/
void cli_lfdr();

/** Purpose:	Print current working directory of client
 *  Param:		socket descriptor of connection, filepath
 *  Return:		void
 *
*/
void cli_lpwd();

/** Purpose:	To download named file from server to client
 *  Param:		socket descriptor of connection, filepath
 *  Return:		void
 *
*/
void cli_get(int socket_desc, char *file_name);

/** Purpose:	To display the help menu
 *  Return:		void
 *
*/
void cli_help();