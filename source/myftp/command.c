/*  File:		command.c for assignment 2 myftp (client side)
 *  Author:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose:		Contains the core functions for the client operation.
 *  todo: 		Need to handle if user types in cd, put,get, lcd without second argument causing a segmentation fault
 */

#include "command.h"		/* head file all command function */

int tokenise (char line[], char *token[])
{
    	char *tk;
    	int i=0;
    	tk = strtok(line, tokenSep);
    	token[i] = tk;

    	while (tk != NULL)
    	{
        	i++;
        	if (i >= MAX_TOKEN)
        	{
            		i = -1;
            		break;
        	}

        	tk = strtok(NULL, tokenSep);
        	token[i] = tk;
    	}
    	return i;
}

// send OPCODE to server
void cmd_prompt(int socket_desc)
{
	// declare token on 100 char
	char input[MAX_NUM_CHAR];

	// declare array of size max_token
	char * tokenArray[MAX_TOKEN];

	// declare total num of tokens
	int numTok;

	while ( 1 )
	{
		fprintf(stdout, " > "); // command prompt

		fgets( input, 99, stdin);

		// Remove the return char if present
		rmReturnChar(input);

		if (strcmp(input, "quit") == 0)
		{
			return;
		}
		else
		{
	    		bzero(tokenArray, sizeof(tokenArray));
			numTok = tokenise(input, tokenArray);

			if (numTok <= 0)
			{
				fprintf(stdout, "Usage: Command [optional: file/dir path]\n");
				continue;
			}

			else if (numTok == 1)
			{
				if(strcmp(tokenArray[0], CMD_FDR) == 0)
				{
					cli_fdr(socket_desc);
				}
				else if(strcmp(tokenArray[0], CMD_PWD) == 0)
				{
					cli_pwd(socket_desc);
				}
				else if(strcmp(tokenArray[0], CMD_LFDR) == 0)
				{
					cli_lfdr();
				}
				else if(strcmp(tokenArray[0], CMD_LPWD) == 0)
				{
					cli_lpwd();
				}
				else if(strcmp(tokenArray[0], CMD_HELP) == 0)
				{
					cli_help();
				}
				else
				{
					fprintf(stdout, "Invalid command, try again. type \"help\" for documentation\n");
				}
			}

			else if (numTok == 2)
			{
				if(strcmp(tokenArray[0], CMD_CD) == 0)
				{
					cli_cd(socket_desc, tokenArray[1]);
				}
				else if(strcmp(tokenArray[0], CMD_PUT) == 0)
				{
					cli_put(socket_desc, tokenArray[1]);
				}
				else if(strcmp(tokenArray[0], CMD_GET) == 0)
				{
					cli_get(socket_desc, tokenArray[1]);
				}
				else if(strcmp(tokenArray[0], CMD_LCD) == 0)
				{
					cli_lcd(tokenArray[1]);
				}
				else
				{
					fprintf(stdout, "Invalid command, try again. type \"help\" for documentation\n");
				}
			}
			else
            		{
                		fprintf(stdout, "Invalid command, try again. type \"help\" for documentation\n");
			}
		}
	}
}

// list files in server
void cli_fdr(int socket_desc)
{
	// variables
	char op_code, ack_code;
	int file_size, read_size;

	// Send the FDR opcode which is 'F'
	if (write_opcode(socket_desc, OP_FDR) == -1)
	{
		fprintf(stderr, "Client: Failed to write opcode\n");
		return;
	}

	// Read the opcode from server
	if (read_opcode(socket_desc, &op_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read opcode\n");
		return;
	}

	// Check if received correct opcode
	if (op_code != OP_FDR)
	{
		fprintf(stderr, "Client: Invalid opcode from fdr: %c\n", op_code);
		return;
	}

    	// Read the ackcode from server
	if(read_opcode(socket_desc, &ack_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read ackcode\n");
		return;
	}

	if (ack_code == EXCEED_LENGTH)
    	{
	    fprintf(stdout, "Server: Exceed program length with code '%c'\n", ack_code);
	}

	// Read the size of the file string from socket
	if (read_length(socket_desc, &file_size) == -1)
	{
		fprintf(stderr, "Client: Failed to read path size\n");
		return;
	}

    	// Allocate memory for the path
	char buf[(sizeof(char) * (file_size+1))];

	// Read the directory path
	if ((read_size = readn(socket_desc, buf, (sizeof(char) * (file_size)))) == -1)
    	{
		fprintf(stderr, "Client: Failed to read directory\n");
		return;
	}

	buf[file_size] = '\0';

	fprintf(stdout, "%s\n", buf);
}

// list files in client
void cli_lfdr()
{
	// variables
    	char * filearray[MAX_TOKEN];
	int filecount = 0;

	// Open current dir and struct
	DIR *dp;
	struct dirent *direntp;
    	if ((dp = opendir(".")) == NULL)
	{
		// set to first entry
		fprintf(stderr, "Client: Failed to open directory\n");
        return;
	}

	// insert the filenames
	while (( direntp = readdir(dp)) != NULL )
	{
		filearray[filecount] = direntp->d_name;
		filecount++;
        	if (filecount >= MAX_TOKEN - 1)
           	{
			fprintf(stderr, "Client: Exceeded program capacity, truncated\n");
                	break;
            	}
	}

	// Sorts the files by filename
    	qsort(&filearray[0], filecount, sizeof(char *), cmpstringp);

    	for (int i = 0; i < filecount; i++)
    	{
        	fprintf(stdout, "%s\t", filearray[i]);
    	}

	fprintf(stdout, "\n");

	return;
}

// print currect working directory of server
void cli_pwd(int socket_desc)
{
	// variables
	char op_code, ack_code;
	int file_size, read_size;

	// Send one opcode which is ASCII char 'W'
	if (write_opcode(socket_desc, OP_PWD) == -1)
	{
		fprintf(stderr, "Client: Failed to write opcode\n");
		return;
	}

	// Read the opcode from the server, supposed to be 'W'
	if (read_opcode(socket_desc, &op_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read opcode\n");
		return;
	}

    	// Read the opcode from the server, supposed to be 'W'
	if (read_opcode(socket_desc, &ack_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read ackcode\n");
		return;
	}

	// Return if wrong opcode
	if (op_code != OP_PWD)
	{
		fprintf(stderr, "Client: Invalid opcode from pwd: %c\n", op_code);
		return;
	}

    	// Receive the ack_code
	if (ack_code != SUCCESS_CODE)
	{
		fprintf(stderr, "Server: Failed to get pwd with code %c\n", ack_code);
		return;
	}

	// Read the size of the path from socket
	if (read_length(socket_desc, &file_size) == -1)
	{
		fprintf(stderr, "Client: Failed to read path size\n");
		return;
	}

    	// Print out the working dir
	if (ack_code ==  SUCCESS_CODE)
    	{
        	// Allocate memory for the path
        	char working_dir_path[(sizeof(char) * (file_size+1))];

        	// Read the directory path
        	if ((read_size = readn(socket_desc, working_dir_path, file_size)) == -1)
        	{
		    	fprintf(stderr, "Client: Failed to read directory\n");
			return;
		}

        	working_dir_path[file_size] = '\0';

        	printf("%s\n", working_dir_path);
    	}
}

// print current working directory of client
void cli_lpwd()
{
	char buf[BUF_SIZE];
	getcwd(buf, sizeof(buf));
	fprintf(stdout, "%s\n", buf);
	return;
}

// change directory of server
void cli_cd(int socket_desc, char* cmd_path)
{
	// variables
    	char op_code, ack_code;
    	rmReturnChar(cmd_path);

	// process the filepath
	int path_len = strlen(cmd_path);
	char file_path[path_len+1];
	strcpy(file_path, cmd_path);

	file_path[path_len] = '\0'; // set last character to null

	// Send opcode 'C'
	if (write_opcode(socket_desc, OP_CD) == -1)
	{
		fprintf(stderr, "Client: Failed to write opcode\n");
		return;
	}

    	// send path length
	if (write_length(socket_desc, path_len) == -1)
	{
		fprintf(stderr, "Client: Failed to write %d path_len\n", path_len);
		return;
	}

   	// send the path
	if (writen(socket_desc, file_path, strlen(file_path)) == -1)
    	{
		fprintf(stderr, "Client: Failed to write directory name %s to server\n", file_path);
		return;
	}

    	// read the reply from server
	if(read_opcode(socket_desc, &op_code) == -1)
    	{
		fprintf(stderr, "Client: Failed to read opcode\n");
		return;
	}

	if(op_code != OP_CD)
    	{
		fprintf(stderr, "Client: Invalid opcode from server: %c\n", op_code);
		return;
	}

	if(read_opcode(socket_desc, &ack_code) == -1)
    	{
		fprintf(stderr, "Client: Failed to read ackcode\n");
		return;
	}

	if(ack_code == ERROR_CODE)
    	{
		fprintf(stderr, "Server: Cannot find the requested directory\n");
		return;
	}
}

// change directory of client
void cli_lcd(char * cmd_path)
{
	rmReturnChar(cmd_path);

	if(chdir(cmd_path) == -1)
    	{
		fprintf(stderr, "Client: Unable to CD to %s\n", cmd_path);
	}
	return;
}

// upload file from client to server
void cli_put(int socket_desc, char *filename)
{
	// variables
	char op_code, ack_code;
	int fd, file_size, nr;
	struct stat stats;
	char buf[BUF_SIZE]; // buffer for file content

	int file_len = strlen(filename); // set file length
	char file_name[file_len + 1]; // define file_name

	strcpy(file_name, filename); // copy filename to file_name

	file_name[file_len] = '\0'; // set last character to null

	// file validation
	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Client: Failed to read source file\n");
		return;
	}

    	// check for fstat
    	if(fstat(fd, &stats) < 0)
    	{
		fprintf(stderr, "Client: Failed to read fstat\n");
        	return;
    	}

	file_size = stats.st_size; // set file size

	// send opcode to server
	if (write_opcode(socket_desc, OP_PUT) == -1)
	{
		fprintf(stderr, "Client: Failed to write opcode\n");
		return;
	}

	// sent file length
	if (write_length(socket_desc, file_len) == -1)
	{
		fprintf(stderr, "Client: Failed to send length\n");
		return;
	}

	// send file name
	if (writen(socket_desc, file_name, file_len) < 0)
	{
		fprintf(stderr, "Client: Failed to send filename\n");
		return;
	}

	// read opcode from server
	if (read_opcode(socket_desc, &op_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read opcode\n");
		return;
	}

	// read ackcode from server
	if (read_opcode(socket_desc, &ack_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read ackcode\n");
		return;
	}

	// send file data
	if (ack_code == SUCCESS_CODE)
	{
		if (write_opcode(socket_desc, OP_DATA) == -1)
		{
			fprintf(stderr, "Client: Failed to write opcode\n");
			return;
		}

		if (write_length(socket_desc, file_size) == -1)
		{
			fprintf(stderr, "Client: Failed to send length\n");
			return;
		}

		while ((nr = readn(fd, buf, BUF_SIZE)) > 0)
		{
			if (writen(socket_desc, buf, nr) == -1)
			{
				fprintf(stdout, "Client: Failed to send file content\n");
				return;
			}
		}
	}
	else if (ack_code == FILE_EXIST)
	{
		fprintf(stdout, "Server: File exist on server. Unable to send\n");
	}
	else if (ack_code == ERROR_CODE)
	{
		fprintf(stdout, "Server: Error in sending file\n");
	}

	close(fd);
}

// download file from server to client
void cli_get(int socket_desc, char *file_name)
{
	// variables
	char op_code, ack_code;
	int fd, file_size, block_size, nr, nw;
	char buf[BUF_SIZE]; // buffer for file content

	int file_len = strlen(file_name);

	// check for file exist or error creating file
	if (access(file_name, F_OK) >= 0)
	{
		fprintf(stderr, "Client: File exists in current folder. Unable to get\n");
		return;
	}

	// send opcode to server
	if (write_opcode(socket_desc, OP_GET) == -1)
	{
		fprintf(stderr, "Client: Failed to write opcode\n");
		return;
	}

	// send file length to server
	if (write_length(socket_desc, file_len) == -1)
	{
		fprintf(stderr, "Client: Failed to write length\n");
		return;
	}

	// send file name to server
	if (writen(socket_desc, file_name, file_len) == -1)
	{
		fprintf(stderr, "Client: Failed to write filename\n");
		return;
	}

	// read opcode from server
	if (read_opcode(socket_desc, &op_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read opcode\n");
		return;
	}

	// read ackcode from server
	if (read_opcode(socket_desc, &ack_code) == -1)
	{
		fprintf(stderr, "Client: Failed to read ackcode\n");
		return;
	}

	if ((fd = open(file_name, O_WRONLY|O_CREAT, 0766)) < 0)
	{
		ack_code = ERROR_CODE;
		fprintf(stderr, "Client: Failed to create file\n");
		return;
	}

	if (ack_code == SUCCESS_CODE)
	{
		// read opcode from server
		if (read_opcode(socket_desc, &op_code) == -1)
		{
			fprintf(stderr, "Client: Failed to read ackcode\n");
			return;
		}

		// read the file size
		if (read_length(socket_desc, &file_size) == -1)
		{
			fprintf(stderr, "Client: Failed to read size\n");
			return;
		}

		block_size = BUF_SIZE; // set block size

		while (file_size > 0)
		{
			if (block_size > file_size)
			{
				block_size = file_size;
			}

			if ((nr = readn(socket_desc, buf, block_size)) == -1)
			{
				fprintf(stdout, "Client: Failed to read data\n");
				return;
			}

			if ((nw = writen(fd, buf, nr)) < nr)
			{
				fprintf(stdout, "Client: Failed to write data\n");
				return;
			}
			file_size -= nw;
		}
	}
	else if (ack_code == FILE_NOT_EXIST)
	{
		fprintf(stdout, "Server: File does not exist on server\n");
		unlink(file_name);
	}
	else if (ack_code == ERROR_CODE)
	{
		fprintf(stdout, "Server: Error sending file to client\n");
		unlink(file_name);
	}

	close(fd);
}

// display the help menu information
void cli_help()
{
    	printf(
    	"Command\t\t\t Function\n"
    	"pwd\t\t\t Display current directory of the server\n"
    	"lpwd\t\t\t Display current directory of the client\n"
    	"dir\t\t\t List files of current directory from the server\n"
    	"ldir\t\t\t List files of current directory from the client\n"
    	"cd <directory_pathname>  Change directory of the server\n"
    	"lcd <directory_pathname> Change directory of the client\n"
    	"get <filename>\t\t Download file from the server to client\n"
    	"put <filename>\t\t Upload file from client to server\n"
	"quit\t\t\t Terminate session\n"
	"help\t\t\t Display help information\n");
}
