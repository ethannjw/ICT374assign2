/** File: 		myftp.c
* Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
* Date:		    25th July 2020
* Purpose:		This is the client driver code for running the simple FTP
*			    Usage: myftp [ hostname | IP_address ] [ port_number ]
*			    The hostname or IP address and the port number is the name
*			    of the remote host that provides the myftp service.
*			    If the hostname is omitted, the localhost is assumed.*
*			    If the port number is omitted, the default port 12345 is assumed.
*	 		    The program contains the following commands:
*			    - pwd - to display the current directory of the server that is serving the client
*			    - lpwd - to display the current directory of the client
*			    - dir - to display the file names under the current directory of the server that is serving the client
*			    - ldir - to display the file names under the current directory of the client
*			    - cd [directory_pathname] - to change the current directory of the server that is serving the client
*			    - lcd directory_pathname - to change the current directory of the client
*			    - get [filename] - to download the named file from the current directory of the remote server and save it in the current directory of the client
*			    - put [filename] - to upload the named file from the current directory of the client to the current directory of the remove server
*			    - quit - to terminate the myftp session
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>		// struct sockaddr_in, htons, htonl
#include <netdb.h>		    // struct hostent, gethostbyname()
#include <string.h>
#include <stdio.h>		    // stdin(), stdout()
#include <stdlib.h>		    // exit()
#include <unistd.h>
//#include "stream.h"		    // MAX_BLOCK_SIZE, readn(), writen()
#include "command.h"        // all the main commands

#define SERV_TCP_PORT 12345	// default server listening port

int main(int argc, char *argv[])
{
     int sd, n, nr, nw, i=0;
     char buf[MAX_BLOCK_SIZE]; 
    char host[60];
     unsigned short port;
     struct sockaddr_in ser_addr; 
     struct hostent *hp;

     /* get server host name and port number */
     if (argc==1) 
     {  // assume server running on the local host and on default port 
          gethostname(host, sizeof(host));
          port = SERV_TCP_PORT;
     } 
     else if (argc == 2) 
     { // use the given host name 
          strcpy(host, argv[1]);
          port = SERV_TCP_PORT;
     } 
     else if (argc == 3) 
     { // use given host and port for server
         strcpy(host, argv[1]);
         int n = atoi(argv[2]);
         if (n >= 1024 && n < 65536)
             port = n;
         else 
         {
             printf("Error: server port number must be between 1024 and 65535\n");
             exit(1);
         }
     } 
     else 
     {
         printf("Usage: %s [ <server host name> [ <server listening port> ] ]\n", argv[0]);
         exit(1);
     }

    /* get host address, & build a server socket address */
     bzero((char *) &ser_addr, sizeof(ser_addr));
     ser_addr.sin_family = AF_INET;
     ser_addr.sin_port = htons(port);
     if ((hp = gethostbyname(host)) == NULL)
     {
           printf("host %s not found\n", host); 
           exit(1);
     }
     // set the addr
     ser_addr.sin_addr.s_addr = * (u_long *) hp->h_addr;

     /* create TCP socket & connect socket to server address */
     sd = socket(PF_INET, SOCK_STREAM, 0);
     if (connect(sd, (struct sockaddr *) &ser_addr, sizeof(ser_addr)) < 0) 
     {
          perror("Client connect issue"); 
          exit(1);
     }
     cmd_prompt(sd);
     return 0;
     /*
     while (++i) 
     { // for testing purpose.
          printf("Client Input[%d]: ", i);
          fgets(buf, sizeof(buf), stdin); 
          nr = strlen(buf);
          if (buf[nr-1] == '\n') 
          { 
            buf[nr-1] = '\0'; 
            --nr; 
          }

          if (strcmp(buf, "quit")==0) {
               printf("Bye from client\n"); 
               exit(0);
          }

          if (nr > 0) 
          {
               if ((nw=writen(sd, buf, nr)) < nr) {
                    printf("client: send error\n"); 
                    exit(1);
               }
               if ((nr=readn(sd, buf, sizeof(buf))) <=0) {
                    printf("client: receive error\n"); 
                    exit(1);
               }
               buf[nr] = '\0';
               printf("Sever Output[%d]: %s\n", i, buf);
          }
     }*/
}
