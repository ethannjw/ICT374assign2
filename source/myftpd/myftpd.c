/** File: 		    myftpd.c
*   Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:		    25th July 2020
*   Purpose:		This is the server driver code for running the simple FTP
*			        Usage: myftp [ hostname | IP_address ] [ port_number ]
*			        The hostname or IP address and the port number is the name
*			        of the remote host that provides the myftp service.
*			        If the hostname is omitted, the localhost is assumed.
*			        If the port number is omitted, the default port 12345 is assumed.
*	 		        The program contains the following commands:
*			        - pwd to display the current directory of the server that is serving the client
*			        - dir to display the file names under the current directory of the server that is serving the client
*			        - cd [directory_pathname] - to change the current directory of the server that is serving the client
*			        - get [filename] - to download the named file from the current directory of the remote server and save it in the current directory of the client
*			        - put [filename] - to upload the named file from the current directory of the client to the current directory of the remove server
*			        - quit - to terminate the myftp session
*   Bug:            Once CD to a different directory, logfile no longer has permission access
*/

#include  <stdlib.h>        /* exit(), free() */
#include  <stdio.h>         /* printf(), fprintf(), perror() */
#include  <sys/types.h>     /* pid_t, u_long, u_short */
#include  <sys/socket.h>    /* struct sockaddr, socket(), etc */
#include  <netinet/in.h>    /* struct sockaddr_in, htons(), htonl(), */
#include  <sys/wait.h>      /* waitpid(), WNOHAND */
#include  <signal.h>        /* SIGCHLD, sigaction() */
#include  <unistd.h>        /* read(), write() */
#include  <string.h>        /* strlen(), strcmp() etc */
#include  <errno.h>         /* extern int errno, EINTR, perror() */
#include  <sys/stat.h>      /* fstat(), lstat(), stat() */

#include  "commandd.h"		/* head file all command function */

#define  SERV_TCP_PORT  12345   /* default server listening port */

/* claim as many zombies as we can */
void claim_children()
{
    pid_t pid = 1;

    while (pid > 0)
    {
        pid = waitpid(0, (int *)0, WNOHANG);
    }
}

/* change to daemon process */
void daemon_init(char *dir)
{
    pid_t pid;
    struct sigaction act;

    if ((pid = fork()) < 0)
    {
        perror("Server: Fork");
        exit(1);
    }
    else if (pid > 0)
    {
        printf("Myftp Server PID: %d\n", pid);
        exit(0);                  /* parent goes bye-bye */
    }

    /* child continues */
    setsid();                      /* become session leader */
    chdir(dir);                    /* change working directory */
    umask(0);                      /* clear file mode creation mask */

    /* catch SIGCHLD to remove zombies from system */
    act.sa_handler = claim_children; /* use reliable signal */
    sigemptyset(&act.sa_mask);       /* not to block other signals */
    act.sa_flags   = SA_NOCLDSTOP;   /* not catch stopped children */
    sigaction(SIGCHLD,(struct sigaction *)&act,(struct sigaction *)0);
}

int main(int argc, char *argv[])
{
    // variables
    int sd, nsd;
    pid_t pid;
    unsigned short port;
    socklen_t cli_addrlen;
    struct sockaddr_in ser_addr, cli_addr;
    char mydir[BUF_SIZE];

    port = SERV_TCP_PORT; // set the port number 12345

    /* get the directory */
    if (argc == 1)
    {
        getcwd(mydir, sizeof(mydir));
    }
    else if (argc == 2)
        strncpy(mydir, argv[1], sizeof(mydir));

    /* turn the program into a daemon */
    daemon_init(mydir);

    /* set the absolute path for the log file */
	char log_path[256];
    getcwd(mydir, sizeof(mydir));
    strcpy(log_path, mydir);
	strcat(log_path, LOG_NAME);
	printf("Log File:\n%s\n", log_path);
	log_message(log_path, "Log File:\n%s\n", log_path);

    /* set up listening socket sd */
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Server: Create socket");
        exit(1);
    }

    /* build server Internet socket address */
    bzero((char*)&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind server address to socket sd */
    if (bind(sd, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) <0 )
    {
        perror("Server: Bind");
        exit(1);
    }

    /* become a listening socket */
    listen(sd, 5);
    fprintf(stdout, "Myftp server started up, listening on port %d\n", SERV_TCP_PORT);
    log_message(log_path, "Myftp server started up, listening on port %d\n", SERV_TCP_PORT);

    while (1)
    {
        /* wait to accept a client request for connection */
        cli_addrlen = sizeof(cli_addr);
        nsd = accept(sd, (struct sockaddr *)&cli_addr, &cli_addrlen);

        if (nsd < 0)
        {
            /* if interrupted by SIGCHLD */
            if (errno == EINTR)
                continue;

            perror("Server: Accept");
            exit(1);
        }

        /* create a child process to handle this client */
        if ((pid = fork()) < 0)
        {
            perror("Server: Fork error");
            exit(1);
        }
        else if (pid > 0)
        {
            close(nsd); /* parent to wait for connection */
            continue;   /* parent to wait for next client */
        }

        /* now in child, serve the current client */
        close(sd); /* data exchange through socket ns */
        serve_a_client(nsd, cli_addr, log_path);
        log_message(log_path, "Client session closed\n");

        exit(0);
    }
}
