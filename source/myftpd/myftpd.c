/*
 * file:    myftpd.c
 * author:  Kim & Ethan
 * aim:     implements ftp server
 * date:    2020.07.13
*/

#include  <stdlib.h>     /* strlen(), strcmp() etc */
#include  <stdio.h>      /* printf()  */
#include  <sys/types.h>  /* pid_t, u_long, u_short */
#include  <sys/socket.h> /* struct sockaddr, socket(), etc */
#include  <netinet/in.h> /* struct sockaddr_in, htons(), htonl(), */
                         /* and INADDR_ANY */
#include  <sys/wait.h>   /* waitpid(), WNOHAND */
#include  <signal.h>     /* SIGCHLD, sigaction() */
#include  <unistd.h>     /* read(), write() */
#include  <string.h>     /* strlen(), strcmp() etc */
#include  <errno.h>      /* extern int errno, EINTR, perror() */


#define  SERV_TCP_PORT  12345 /* default server listening port */

void claim_children()
{
    pid_t pid = 1;
     
    while (pid > 0) 
    { 
        /* claim as many zombies as we can */
        pid = waitpid(0, (int *)0, WNOHANG); 
    } 
}

void daemon_init(void)
{       
    pid_t pid;
    struct sigaction act;

    if ((pid = fork()) < 0) 
    {
        perror("fork");
        exit(1); 
    } 
    else if (pid > 0) 
    {
        printf("Hay, you'd better remember my PID: %d\n", pid);
        exit(0);                  /* parent goes bye-bye */
    }

    /* child continues */
    setsid();                      /* become session leader */
    chdir("/");                    /* change working directory */
    umask(0);                      /* clear file mode creation mask */

    /* catch SIGCHLD to remove zombies from system */
    act.sa_handler = claim_children; /* use reliable signal */
    sigemptyset(&act.sa_mask);       /* not to block other signals */
    act.sa_flags   = SA_NOCLDSTOP;   /* not catch stopped children */
    sigaction(SIGCHLD,(struct sigaction *)&act,(struct sigaction *)0);
    /* note: a less than perfect method is to use 
       signal(SIGCHLD, claim_children); */
}

int main(int argc, char *argv[])
{
    int sd, nsd, n;
    pid_t pid;
    unsigned short port;
    socklen_t cli_addrlen;
    struct sockaddr_in ser_addr, cli_addr;

    /* get the port number */
    if (argc == 1)
        port = SERV_TCP_PORT;
    else if (argc == 2)
    {
        int n = atoi(argv[1]);

        /* only accept port between 1024 and 65535 */ 
        if (n >= 1024 && 65536) 
            port = n;
        else
        {
            printf("Error: port number must be between 1024 and 65535\n");
            exit(1);
        }
        
    }

    /* turn the program into a daemon */
    daemon_init();

    /* set up listening socket sd */
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("server:socket");
        exit(1);
    }

    /* build server Internet socket address */
    bzero((char*)&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* note: accept client request sent to any one of the
       network interface(s) on this host. */
    
    /* become a listening socket */
    listen(sd, 5);

    while (1)
    {
        /* wait to accept a client request for connection */
        cli_addrlen = sizeof(cli_addr);
        nsd = accept(sd, (struct sockadd *)&cli_addr, &cli_addrlen);

        if (nsd < 0)
        {
            /* if interrupted by SIGCHLD */
            if (errno == EINTR)
                continue;
            
            perror("server:accept");
            exit(1);
        }

        /* create a child process to handle this client */
        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (pid > 0)
        {
            close(nsd); /* parent to wait for connection */
            continue; /* parent to wait for next client */
        }

        /* now in child, serve the current client */
        close(sd); /* data exchange through socket ns */
        //serve_a_client(nsd);
        exit(0);
    }
}