# ICT374assign2
Assignment 2 Repo for ICT374 submission

Project 2: A Simple File Transfer Protocol
Design and implement a simple network protocol that can be used to download files from a remote site and to upload files to a remote site, and a client and a server programs that communicate using that protocol. The protocol should use TCP as its transport layer protocol. The server must be able to serve multiple client requests simultaneously. For simplicity, do not consider any authentication process in this project, hence the server will provide its service to any client with the right site address and port number.

myftpd [ initial_current_directory ]
The server process maintains a current directory. Its initial value should be the one inherited from its parent process unless the optional initial_current_directory is given. In the latter case, the user supplied path should be used to set the initial current directory of the server. This can be done using the function chdir. A client can use the cd command to change the (child) server's current directory later.

The client program should be named myftp with the following command line syntax:

myftp [ hostname | IP_address ]
where the optional hostname (IP_address) is the name (address) of the remote host that provides the myftp service. If the hostname or IP_address is omitted, the local host is assumed.

After the connection between the client and the server is established, the client should display a prompt > and wait for one of the following commands:

pwd - to display the current directory of the server that is serving the client;

lpwd - to display the current directory of the client;

dir - to display the file names under the current directory of the server that is serving the client;

ldir - to display the file names under the current directory of the client;

cd directory_pathname - to change the current directory of the server that is serving the client; Must support "." and ".." notations.

lcd directory_pathname - to change the current directory of the client; Must support "." and ".." notations.

get filename - to download the named file from the current directory of the remote server and save it in the current directory of the client;

put filename - to upload the named file from the current directory of the client to the current directory of the remove server.

quit - to terminate the myftp session.

The myftp client should repeatedly display the prompt and wait for a command until the quit command is entered.

# Todo:
- [x] pwd
- [x] lpwd
- [x] dir
- [x] ldir
- [x] cd
- [x] lcd
- [x] get
- [x] put
- [ ] cleanup code syntax
- [ ] update header files with up to date information
- [ ] complete protocol documentation

# Receiving server current working directory

Upon receiving a pwd command from the user: 
- the client sends a 1 byte opcode of ASCII character 'W' to the server
```
-----------------
| 1 byte opcode |
-----------------
```
The server replies with: 
- the opcode 'W' indicating the successful reciept of the request, 
- followed by a 4 byte int indicating the length of the data
- The server then sends a sequence of N bytes which is the current directory path in ASCII
```
------------------------------------------------------------------------
| 1 byte opcode |     4 byte int     |              Data               |
------------------------------------------------------------------------
```

