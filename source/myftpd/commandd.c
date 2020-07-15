/** File: 		    commandd.c
*   Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:		    25th July 2020
*   Purpose:		This is the command driver code for running the commands for FTP
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
#include <sys/stat.h>      /* fstat(), lstat(), stat() */

#include "commandd.h"
#include "stream.h"

void serve_a_client(int sd)
{
	/*int nr, nw, i;
    char buf[BUF_SIZE];

    while (1){
        // read data from client
        if ((nr = read(sd, buf, sizeof(buf))) <= 0)
            return;   // connection broken down

        // process data
        buf[nr] = '\0';

        printf("TEST\n");
		//ser_pwd(sd);

        // send results to client
        nw = write(sd, buf, nr);
    }*/

	char opcode;

	while (read_opcode(sd, &opcode) > 0)
	{
		printf("OPCODE: %c\n", opcode);
		switch(opcode)
		{
			case OP_PUT:
				break;
			case OP_GET:
				break;
			case OP_PWD:
				ser_pwd(sd);
				printf("PWD comment executed\n");
				break;
			case OP_FDR:
				break;
			case OP_CD:
				break;
			default:
				break;
		}
	}
}

// PWD from client to display cwd of server.
//void ser_pwd(cli_desc *des)
void ser_pwd(int sd)
{
	char buf[1000];

	getcwd(buf, sizeof(buf));
	printf("BUF: %s\n", buf);

	// write the opcode to socket
	//if(write_opcode(des->sd, OP_PWD) == -1){
	if(write_opcode(sd, OP_PWD) == -1){
		fprintf(stderr, "Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
	}

    int buflen = strlen(buf);
	// send the length of working dir
	//if(write_length(des->sd, strlen(buf)) == -1){
	if(write_length(sd, buflen) == -1){
		fprintf(stderr, "Failed to send length\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written length of %d\n", buflen);
	}
	// send the dir info
	//if(writen(des->sd, buf, strlen(buf)) == -1){
	if(writen(sd, buf, buflen) == -1){
		fprintf(stderr, "Failed to send directory info\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful send directory info\n");
	}

	fprintf(stdout, "Send PWD success\n");
}
