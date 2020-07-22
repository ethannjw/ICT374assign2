/** File:			commandd.h for assignment 2 myftp (server side)
*   Authors:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:			25th July 2020
*   Purpose:		This is the command driver code for running the commands for FTP
*   Bug:            fdr function crashed if the length is too big
*/

#include <stdlib.h>        /* exit(), free() */
#include <stdio.h>         /* printf()  */
#include <sys/types.h>     /* pid_t, u_long, u_short */
#include <unistd.h>        /* read(), write() */
#include <string.h>        /* strlen(), strcmp() etc */
#include <sys/stat.h>      /* fstat(), lstat(), stat() */
#include <fcntl.h>         /* manipulate file descriptor */
#include <dirent.h>	       /* DIR */
#include <time.h>		   /* TIME */
#include <stdarg.h>		   /* macros va_start, va_copy, va_arg, and va_end */
#include <arpa/inet.h>      /* inet_ntoa() */

#include "../stream.h"     /* head file for stream read and write */

/* OP CODES for the network specification */
#define OP_PUT  'P'
#define OP_GET  'G'
#define OP_PWD  'W'
#define OP_FDR  'F'
#define OP_CD   'C'
#define OP_DATA 'D'

/* ACK codes for each network specification */
#define SUCCESS_CODE    '0'
#define ERROR_CODE	    '1'

/* ACK codes for PUT and GET */
#define FILE_EXIST      'E'
#define FILE_NOT_EXIST  'N'

/* ACK codes for DIR */
#define EXCEED_LENGTH   'L'

#define MAX_TOKEN           256
#define BUF_SIZE            256
#define MAX_STRING_LENGTH   10000
#define LOG_NAME		    "/myftpd.log"	/* log file */

/** Purpose:	To log all interactions with the clients
 *  Param:		logfile, string format and arguments
 *  Return:		void
 *
*/void log_message(char *file, const char *format, ...);

/** Purpose:	Process OPCODE received from client
 *  Param:		socket descriptor of connection, IP address, logfile
 *  Return:		void
 *
*/
void serve_a_client(int socket_desc, struct sockaddr_in cli_addr, char *file);

/** Purpose:	To send the list of files in current dir of the server
 *  Param:		socket descriptor of connection, logfile
 *  Return:		void
 *
*/
void ser_fdr(int socket_desc, char *file);

/** Purpose:	To send the current working directory of the server
 *  Param:		socket descriptor of connection, logfile
 *  Return:		void
 *
*/
void ser_pwd(int socket_desc, char *file);

/** Purpose:	To change working directory of the server
 *  Param:		socket descriptor of connection, logfile
 *  Return:		void
 *
*/
void ser_cd(int socket_desc, char *file);

/** Purpose:	To upload files to the server
 *  Param:		socket descriptor of connection, logfile
 *  Return:		void
 *
*/
void ser_put(int socket_desc, char *file);

/** Purpose:	To download file from the server
 *  Param:		socket descriptor of connection, logfile
 *  Return:		void
 *
*/
void ser_get(int socket_desc, char *file);
