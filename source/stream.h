/*
 *  file:	stream.h
 *  Purpose: 	head file for stream read and stream write. 
 */

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h> /* struct sockaddr_in, htons(), htonl(), */
#include <string.h> /* strlen() */

#define MAX_BLOCK_SIZE (1024*5)    /* maximum size of any piece of */
                                   /* data that can be sent by client */

/*
 * purpose:  read a stream of bytes from "fd" to "buf".
 * pre:      1) size of buf bufsize >= MAX_BLOCK_SIZE,
 * post:     1) buf contains the byte stream; 
 *           2) return value > 0   : number ofbytes read
 *                           = 0   : connection closed
 *                           = -1  : read error
 *                           = -2  : protocol error
 *                           = -3  : buffer too small
 */           
int readn(int fd, char *buf, int bufsize);



/*
 * purpose:  write "nbytes" bytes from "buf" to "fd".
 * pre:      1) nbytes <= MAX_BLOCK_SIZE,
 * post:     1) nbytes bytes from buf written to fd;
 *           2) return value = nbytes : number ofbytes written
 *                           = -3     : too many bytes to send 
 *                           otherwise: write error
 */           
int writen(int fd, char *buf, int nbytes);


/*
 * Writes a one byte char from opcode to socket sd.
 * return: -1 : write failed
 * 1 : write success
 */
int write_opcode(int socket_desc, char code);

/*
 * Reads a one byte char from socket sd to opcode.
 * return: -1 : read failed
 *					1 : read success
 */
int read_opcode(int socket_desc, char* code);

/*
 * Writes a four byte integer from length to socket sd.
 * return: -1 : write failed
 *					1 : write success
 */
int write_length(int socket_desc, int len);

/*
 * Reads a four byte integer from socket sd to length.
 * return: -1 : read failed
 *					1 : read success
 */
int read_length(int socket_desc, int *len);

/* Removes the endline character if present. 
*  otherwise checks if last char is null char
*  if so, convert the last char to null char
*/
void rmReturnChar(char *line);