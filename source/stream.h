/** File:			stream.h for assignment 2
 *  Author:			Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Date:			25th July 2020
 *  Purpose:		Contains the stream functions for the server and client
 *  Assumptions:
*/

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h> /* struct sockaddr_in, htons(), htonl(), */
#include <string.h>     /* strlen() */

#define MAX_BLOCK_SIZE (1024*5)    /* maximum size of any piece of */
                                   /* data that can be sent by client */

/**
 * Purpose: read a stream of bytes from "fd" to "buf".
 * Param:   socket descriptor, string to read to, number of bytes to read
 * Return:  number of bytes read from fd to buf;
 */
int readn(int fd, char *buf, int bufsize);

/**
 * Purpose:  Write "nbytes" bytes from "buf" to "fd".
 * Param:    socket descriptor, string to write, number of bytes to write
 * Return:   number of bytes from buf written to fd;
 */
int writen(int fd, char *buf, int nbytes);


/**
 * Purpose: Writes a one byte char from code to socket_desc.
 * Param:   socket descriptor, code to write
 * Return: -1 : write failed
 *          1 : write success
 */
int write_opcode(int socket_desc, char code);

/**
 * Purpose: Reads a one byte char from socket_desc to code.
 * Param:   socket descriptor, code to read to
 * Return: -1 : write failed
 *          1 : write success
 */
int read_opcode(int socket_desc, char* code);

/**
 * Purpose: Writes a four byte int from len to socket_desc.
 * Param:   socket descriptor, length
 * Return: -1 : write failed
 *          1 : write success
 */
int write_length(int socket_desc, int len);

/**
 * Purpose: Reads a four byte char from socket_desc to len.
 * Param:   socket descriptor, length to read to
 * Return: -1 : write failed
 *          1 : write success
 */
int read_length(int socket_desc, int *len);

/**
 * Purpose: Removes the endline character if present.
 * Param:   string to modify
 * Return:  void
*/
void rmReturnChar(char *line);

/**
 *  Purpose: String compare function used in the qsort function.
 *  Param:   Pointer to first item to compare, pointer to second item to compare
 *  Return:  String compare value
 *  Source:  qsort man page
*/
int cmpstringp(const void *p1, const void *p2);
