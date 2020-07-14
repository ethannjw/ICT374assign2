/*
 *  file: 	stream.c
 *  Authors:
 *  purpose:	stream read and write. 
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

// Code from Daniel code

/*
 * Loops over a write to socket until entire nbytes is written from buf.
 */
int write_nbytes(int sd, char *buf, int nbytes)
{
	int nw = 0;
	int n = 0;
	for (n=0; n < nbytes; n += nw) {
		if ((nw = write(sd, buf+n, nbytes-n)) <= 0)
			return (nw); /* write error */
	}
  return n;
}

/*
 * Loops over a read from socket until entire nbytes read into buf.
 */
int read_nbytes(int sd, char *buf, int nbytes)
{
	int nr = 1;
	int n = 0;
	for (n=0; (n < nbytes) && (nr > 0); n += nr) {
		if ((nr = read(sd, buf+n, nbytes-n)) < 0){
			return (nr); /* read error */
		}
	}
	return (n);
}

/*
 * Writes a one byte char on the socket.
 */
int write_code(int sd, char code)
{
	/* write 1 byte code to socket */
	if (write(sd, (char*)&code, 1) != 1) return (-1);

	return 1;
}

/*
 * Reads a one byte char off the socket.
 */
int read_code(int sd, char* code)
{
	char data;

	/* read 1 byte code from socket */
	if(read(sd,(char *) &data,1) != 1) return -1;

	*code = data;

	return 1;
}

/*
 * Writes a two byte integer on the socket.
 */
int write_twobytelength(int sd, int length)
{
	short data = length;
	data = htons(data);  /* convert to network byte order */

	if (write(sd,&data, 2) != 2) return (-1);

	return 1;
}

/*
 * Reads a two byte integer off the socket.
 */
int read_twobytelength(int sd, int *length)
{
	short data = 0;

  if (read(sd, &data, 2) != 2) return (-1);

  short conv = ntohs(data); /* convert to host byte order */
  int t = (int)conv;
  *length = t;

	return 1;
}

/*
 * Writes a four byte integer on the socket.
 */
int write_fourbytelength(int sd, int length)
{
	int data = htonl(length); /* convert to network byte order */

	if (write(sd,&data, 4) != 4) return (-1);

	return 1;
}

/*
 * Reads a four byte integer off the socket.
 */
int read_fourbytelength(int sd, int *length)
{
	int data = 0;

  if (read(sd, &data, 4) != 4) return (-1);

  int conv = ntohl(data); /* convert to host byte order */
  *length = conv;

	return 1;
}