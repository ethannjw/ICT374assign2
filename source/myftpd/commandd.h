/** File: 		    commandd.h
*   Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:		    25th July 2020
*   Purpose:		This is the command driver code for running the commands for FTP
*   Bug:            fdr function crashed if the length is too big
*/
#include <stdlib.h>        /* strlen(), strcmp() etc */
#include <stdio.h>         /* printf()  */
#include <sys/types.h>     /* pid_t, u_long, u_short */
#include <sys/socket.h>    /* struct sockaddr, socket(), etc */
#include <netinet/in.h>    /* struct sockaddr_in, htons(), htonl(), */
                            /* and INADDR_ANY */
#include <sys/wait.h>      /* waitpid(), WNOHAND */
#include <signal.h>        /* SIGCHLD, sigaction() */
#include <unistd.h>        /* read(), write() */
#include <string.h>        /* strlen(), strcmp() etc */
#include <errno.h>         /* extern int errno, EINTR, perror() */
#include <sys/stat.h>    /* fstat(), lstat(), stat() */
#include <fcntl.h>
#include <dirent.h>	/* DIR */


#include "../stream.h"

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
#define FILE_EXIST '1'
#define FILE_NOT_EXIST '2'

#define  BUF_SIZE       1000

/* desc type containing socket descriptor, client id */
struct client_struct{
	int sd;
	int cid;
};
typedef struct client_struct cli_desc;

/** Purpose:	To send the list of files in current dir of the server
 *  Param:	int
 *  Return:	void
 *
*/
void serve_a_client(int sd);

/** Purpose:	To send the list of files in current dir of the server
 *  Param:	int
 *  Return:	void
 *
*/
void ser_fdr(int sd);

/** Purpose:	To send the current working directory of the server
 *  Param:	int
 *  Return:	void
 *
*/
//void ser_pwd(cli_desc *des);
void ser_pwd(int sd);

/** Purpose:	To upload files to the server
 *  Param:	int
 *  Return:	void
 *
*/
void ser_put(int sd);

/** Purpose:	To change working directory of the server
 *  Param:	int socket desc
 *  Return:	void
 *
*/
void ser_cd(int sd);

/** Purpose:	To dowbnload file from the server
 *  Param:	int 
 *  Return:	void
 *
*/
void ser_get(int sd);