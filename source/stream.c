/** File:			stream.c for assignment 2
 *  Author:			Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Date:			25th July 2020
 *  Purpose:		Contains the stream functions for the server and client
 *  Assumptions:
*/

#include "stream.h"

int readn(int sd, char *buf, int nbytes)
{
	int nr = 1;
	int n = 0;
	for (n = 0; (n < nbytes) && (nr > 0); n += nr)
    {
		if ((nr = read(sd, buf+n, nbytes-n)) < 0)
		{
			return (nr);
		}
	}
	return (n);
}

int writen(int sd, char *buf, int nbytes)
{
	int nw = 0;
	int n = 0;
	for (n = 0; n < nbytes; n += nw)
    {
		if ((nw = write(sd, buf+n, nbytes-n)) <= 0)
        {
            return (nw);
        }

	}
  return n;
}

// Writes one byte char on the socket.
int write_opcode(int socket_desc, char code)
{
	// write the 1 byte code to socket
	if (write(socket_desc, (char*)&code, 1) != 1)
	{
		return (-1);
	}
	return 1;
}

// Reads a one byte char off the socket.
int read_opcode(int socket_desc, char* code)
{
	char data;

	// read 1 byte code from socket
	if(read(socket_desc,(char *) &data,1) != 1)
	{
		return -1;
	}
	*code = data;

	return 1;
}

// Writes the four byte int on the socket.
int write_length(int socket_desc, int len)
{
	// convert to host to network long
	int data = htonl(len);

	if (write(socket_desc,&data, 4) != 4)
	{
		return (-1);
	}

	return 1;
}


// Reads a four byte int off the socket.
int read_length(int socket_desc, int *len)
{
	int data;

	if (read(socket_desc, &data, 4) != 4)
	{
		return (-1);
	}
	/* to network byte order */
	*len = ntohl(data);

	return 1;
}

// process the last char in string
void rmReturnChar(char *line)
{
	// Removes last char if it is a return char
	int n = strlen(line);
	if (line[n-1]=='\n')
	{
		line[n-1]='\0';
	}
	// Adds null last char if not a null char
	else if (line[n-1]!='\0')
    {
		line[n]='\0';
	}
}

int cmpstringp(const void *p1, const void *p2)
{
    	// cmp string pointer of pointers taken from qsort man page
    	// used in the qsort function
    	return strcmp(* (char * const *) p1, * (char * const *) p2);

}
