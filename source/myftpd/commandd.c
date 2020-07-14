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

#include  "commandd.h"

void serve_a_client(int sd)
{   int nr, nw, i;
    char buf[BUF_SIZE];

    while (1){
        /* read data from client */
        if ((nr = read(sd, buf, sizeof(buf))) <= 0) 
            return;   /* connection broken down */

        /* process data */
        buf[nr] = '\0';

        printf("TEST\n");
              
        /* send results to client */
        nw = write(sd, buf, nr);
    } 
}