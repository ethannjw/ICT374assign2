/*  File:		command.c for assignment 2 myftp (client side)
 *  Author:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose:		Contains the core functions for the client operation.
 *  Assumptions:
*/

#include "command.h"

int tokenise (char line[], char *token[])
{
    char *tk;
    int i=0;
    tk = strtok(line, tokenSep);
    token[i] = tk;

    while (tk != NULL)
    {

        ++i;
        if (i>=MAX_TOKEN)
        {
            i = -1;
            break;
        }

        tk = strtok(NULL, tokenSep);
        token[i] = tk;
    }
    return i;
}

void cmd_prompt(int socket_desc)
{
	Command commandStruct;
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
	    	//bzero(tokenArray, sizeof(tokenArray));
			numTok = tokenise(input, tokenArray);

			if (numTok < 0)
			{
				fprintf(stdout, "Usage: Command [optional: file/dir path]\n");
				continue;
			}
			/*else if (numTok == 1)
            {
                strcpy(commandStruct.cmd, tokenArray[0]);
            }
			else if (numTok == 2)
			{
				strcpy(commandStruct.cmd, tokenArray[0]);
				strcpy(commandStruct.arg, tokenArray[1]);
			}
            else
			{
				fprintf(stdout, "Usage: [<command> <optional dir>]\n");
            }*/

			if(strcmp(tokenArray[0], CMD_PWD) == 0){
				cli_pwd(socket_desc);
			}
			else if(strcmp(tokenArray[0], CMD_FDR) == 0){
				cli_fdr(socket_desc);
			}

			else if(strcmp(tokenArray[0], CMD_PUT) == 0){
				cli_put(socket_desc, tokenArray[1]);
			}

			else if(strcmp(tokenArray[0], CMD_CD) == 0){
				cli_cd(socket_desc, tokenArray[1]);
			}
			else
			{
				fprintf(stdout, "No valid command available, try again. See documentation for help\n");
			}
		}
	}
}

void cli_cd(int socket_desc, char* cmd_path)
{
    char op_code;
	char ack_code;
    rmReturnChar(cmd_path);
	// process the filepath
	int path_len = strlen(cmd_path);
	char file_path[path_len+1];
	strcpy(file_path, cmd_path);
	// set last character to null
	file_path[path_len] = '\0';

	// Send opcode 'C'
	if(write_opcode(socket_desc, OP_CD) == -1){
		fprintf(stderr, "Client: Failed to send %c opcode\n", OP_CD);
		return;
	}
	else
    {
        fprintf(stdout, "Client: Successful sent %c opcode\n", OP_CD);
    }

    // send path length
	if(write_length(socket_desc, path_len) == -1){
		fprintf(stderr, "Client: Failed to write %d path_len\n", path_len);
		return;
	}
	else
    {
        fprintf(stdout, "Client: Successful written %d path len\n", path_len);

    }

    // send the path
	if(writen(socket_desc, file_path, strlen(file_path)) == -1)
    {
		fprintf(stderr, "Client: Failed to write directory name %s to server\n", file_path);
		return;
	}

    // read the reply from server
	if(read_opcode(socket_desc, &op_code) == -1)
    {
		perror("Client: Failed to read opcode from server\n");
		return;
	}
    else
    {
        fprintf(stdout, "Client: Successful read '%c' opcode\n", op_code);
    }

	if(op_code != OP_CD)
    {
		fprintf(stderr, "Client: Invalid opcode from server: %c\n", op_code);
		return;
	}

	if(read_opcode(socket_desc, &ack_code) == -1)
    {
		perror("Client: Failed to read ackcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Client: Successful read ack code %c\n", ack_code);
    }

	if(ack_code == SUCCESS_CODE)
    {
		return;
	}

	if(ack_code == ERROR_CODE)
    {
		perror("Client: Server cannot find the directory\n");
		return;
	}
}

void cli_fdr(int socket_desc)
{
	char op_code;
	int file_size, read_size;
	char * buf;

	// Send the FDR opcode which is 'F'
	if(write_opcode(socket_desc, OP_FDR) == -1)
	{
		perror("Failed to send FDR 'F' command");
	}

	// Read the opcode from server
	if(read_opcode(socket_desc, &op_code) == -1)
	{
		perror("Failed to read FDR 'F' from server");
	}

	// Check if received correct opcode
	if(op_code != OP_FDR)
	{
		fprintf(stderr, "Invalid opcode from fdr: %c\n", op_code);
	}
    else
    {
        fprintf(stdout, "Successful get opcode '%c'\n", op_code);
	}

	// Read the size of the file string from socket
	if(read_length(socket_desc, &file_size) == -1)
	{
		perror("Failed to read path size\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful get length %d\n", file_size);
	}

    // Allocate memory for the path
	buf = malloc(sizeof(char) * (file_size+1));

	// Read the directory path
	if((read_size = readn(socket_desc, buf, (sizeof(char)*file_size))) == -1)
     {
		perror("Failed to read directory\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful read data of size %d\n", read_size);
    }

	rmReturnChar(buf);

	printf("%s\n", buf);
	free(buf);
}

void cli_pwd(int socket_desc)
{
	char op_code;
	int file_size, read_size;
	char * working_dir_path;

	// Send one opcode which is ASCII char 'W'
	if(write_opcode(socket_desc, OP_PWD) == -1)
	{
		perror("Failed to send pwd command\n");
		return;
	}

	// Read the opcode from the server, supposed to be 'W'
	if(read_opcode(socket_desc, &op_code) == -1)
	{
		perror("Unable to read opcode\n");
		return;
	}

	// Return if wrong opcode
	if(op_code != OP_PWD)
	{
		fprintf(stderr, "Invalid opcode from pwd: %c\n", op_code);
		return;
	}
    else
    {
        fprintf(stdout, "Successful get opcode '%c'\n", op_code);
	}

	// Read the size of the path from socket
	if(read_length(socket_desc, &file_size) == -1)
	{
		perror("Failed to read path size\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful get length %d\n", file_size);
	}

    // Allocate memory for the path
	working_dir_path = malloc(sizeof(char) * (file_size+1));

	// Read the directory path
	if((read_size = readn(socket_desc, working_dir_path, (sizeof(char)*file_size))) == -1)
     {
		perror("Failed to read directory\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful read data of size %d\n", read_size);
    }

	rmReturnChar(working_dir_path);

	printf("%s\n", working_dir_path);
	free(working_dir_path);
}

void cli_put(int socket_desc, char *filename)
{
	char op_code, ack_code;
	int fd, file_size, nr;
	struct stat stats;
	char buf[BUF_SIZE];

	int file_len = strlen(filename);
	char file_name[file_len + 1];

	strcpy(file_name, filename);
	// set last character to null
	file_name[file_len] = '\0';

	// file validation
	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}
	else
	{
		fprintf(stdout, "Successful read the file\n");
	}

    // check for fstat
    if(fstat(fd, &stats) < 0)
    {
            fprintf(stderr, "Failed to read fstat\n");
            return;
    }

	file_size = stats.st_size; // set file size

	// send PUT
	if (write_opcode(socket_desc, OP_PUT) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
	}

	if (write_length(socket_desc, file_len) == -1)
	{
		fprintf(stderr, "Failed to send length\n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful read length of %d\n", file_len);
	}

	if (writen(socket_desc, file_name, file_len) < 0)
	{
		fprintf(stderr, "Failed to send filename\n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful send filename of %s\n", file_name);
	}

	// waiting for server response
	if (read_opcode(socket_desc, &op_code) == -1)
	{
		fprintf(stderr, "Failed to read opcode\n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful reading opcode\n");
	}

	if (read_opcode(socket_desc, &ack_code) == -1)
	{
		fprintf(stderr, "Failed to read ackcode\n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful reading ackcode\n");
	}

	// send file data
	if (ack_code == SUCCESS_CODE)
	{
		if (write_opcode(socket_desc, OP_PUT) == -1)
		{
			fprintf(stderr, "Failed to write opcode\n");
			return;
		}
		else
		{
			fprintf(stdout, "Successful written opcode\n");
		}

		if (write_length(socket_desc, file_size) == -1)
		{
			fprintf(stderr, "Failed to send length\n");
			return;
		}
		else
		{
			fprintf(stdout, "Successful read filesize of %d\n", file_size);
		}

		while ((nr = readn(fd, buf, BUF_SIZE)) > 0)
		{
			if (writen(socket_desc, buf, nr) == -1)
			{
				fprintf(stdout, "Failed to send file content\n");
				return;
			}
		}

		fprintf(stdout, "File sent\n");
	}
	else if (ack_code == FILE_EXIST)
	{
		fprintf(stdout, "File exist on server\n");
	}
	else if (ack_code == ERROR_CODE)
	{
		fprintf(stdout, "Error sending file\n");
	}

	close(fd);
}
