/** File: 		    commandd.c
*   Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:		    25th July 2020
*   Purpose:		This is the command driver code for running the commands for FTP
*/

#include  <stdlib.h>        /* strlen(), strcmp() etc */
#include  <stdio.h>         /* printf()  */
#include  <sys/types.h>     /* pid_t, u_long, u_short */
#include  <sys/socket.h>    /* struct sockaddr, socket(), etc */
#include  <netinet/in.h>    /* struct sockaddr_in, htons(), htonl(), */
                            /* and INADDR_ANY */
#include  <sys/wait.h>      /* waitpid(), WNOHAND */
#include  <signal.h>        /* SIGCHLD, sigaction() */
#include  <unistd.h>        /* read(), write() */
#include  <string.h>        /* strlen(), strcmp() etc */
#include  <errno.h>         /* extern int errno, EINTR, perror() */
#include  <sys/stat.h>      /* fstat(), lstat(), stat() */

/* OP CODES for the network specification */
#define OP_PUT  'P'
#define OP_GET  'G'
#define OP_PWD  'W'
#define OP_FDR  'F'
#define OP_CD   'C'
#define OP_DATA 'D'

/* ACK codes for each network specification */
#define SUCCESS_CODE	'0'
#define ERROR_CODE	    '-1'

int main()
{
    
}