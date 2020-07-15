/*
 *  File: 	stream.c for assign2 
 *  Authors:	Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose: 	Contains the read/write n, read/write opcode, read/write length
 */

#include  <sys/types.h>
#include  <netinet/in.h> /* struct sockaddr_in, htons(), htonl(), */
#include  "stream.h"
#include <unistd.h>

int readn(int fd, char *buf, int bufsize)
{
    short data_size;    /* sizeof (short) must be 2 */ 
    int n, nr, len;

    /* check buffer size len */
    if (bufsize < MAX_BLOCK_SIZE)
         return (-3);     /* buffer too small */

    /* get the size of data sent to me */
    if (read(fd, (char *) &data_size, 1) != 1) return (-1);
    if (read(fd, (char *) (&data_size)+1, 1) != 1) return (-1);
    len = (int) ntohs(data_size);  /* convert to host byte order */ 

    /* read len number of bytes to buf */
    for (n=0; n < len; n += nr) {
        if ((nr = read(fd, buf+n, len-n)) <= 0) 
            return (nr);       /* error in reading */
    }
    return (len); 
}

int writen(int fd, char *buf, int nbytes)
{
    short data_size = nbytes;     /* short must be two bytes long */
    int n, nw;

    if (nbytes > MAX_BLOCK_SIZE) 
         return (-3);    /* too many bytes to send in one go */ 

    /* send the data size */
    data_size = htons(data_size);  
    if (write(fd, (char *) &data_size, 1) != 1) return (-1);      
    if (write(fd, (char *) (&data_size)+1, 1) != 1) return (-1);       

    /* send nbytes */
    for (n=0; n<nbytes; n += nw) {
         if ((nw = write(fd, buf+n, nbytes-n)) <= 0)  
             return (nw);    /* write error */
    } 
    return (n);
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
	// to network byte order
	*len = ntohl(data);

	return 1;
}