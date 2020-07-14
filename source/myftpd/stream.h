/*
 *  file:	stream.h
 *  Purpose: 	head file for stream read and stream write. 
 */


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
 * Writes a stream of bytes on socket sd from buf
 * returns:
 * >0 : number of bytes written
 * otherwise : write error
 */
int write_nbytes(int sd, char *buf, int nbytes);

/*
 * Reads a stream of bytes on socket sd to buf
 * returns:
 * >0 : number of bytes read
 * otherwise : read error
 */
int read_nbytes(int sd, char *buf, int nbytes);

/*
 * Writes a one byte char from opcode to socket sd.
 * return: -1 : write failed
 *					1 : write success
 */
int write_code(int sd, char code);

/*
 * Reads a one byte char from socket sd to opcode.
 * return: -1 : read failed
 *					1 : read success
 */
int read_code(int sd,char *code);

/*
 * Writes a two byte integer from length to socket sd.
 * return: -1 : write failed
 *					1 : write success
 */
int write_twobytelength(int sd, int length);

/*
 * Reads a two byte integer from socket sd to length.
 * return: -1 : read failed
 *					1 : read success
 */
int read_twobytelength(int sd, int *length);

/*
 * Writes a four byte integer from length to socket sd.
 * return: -1 : write failed
 *					1 : write success
 */
int write_fourbytelength(int sd, int length);

/*
 * Reads a four byte integer from socket sd to length.
 * return: -1 : read failed
 *					1 : read success
 */
int read_fourbytelength(int sd, int *length);