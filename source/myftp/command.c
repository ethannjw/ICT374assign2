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

      while (tk != NULL) {

          ++i;
          if (i>=MAX_TOKEN) {
              i = -1;
              break;
          }

          tk = strtok(NULL, tokenSep);
          token[i] = tk;
      }
      return i;
}

void rmReturnChar(char *line)
{
	// Removes last char if it is a return char
	int n = strlen(line);
	if (line[n-1]=='\n')
	{
		line[n-1]='\0';
	}
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

	// declare number of commands
	int numCommand;

	int i, j;

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
			else if (numTok == 1)
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
            }

			if(strcmp(commandStruct.cmd, CMD_PWD) == 0){
				cli_pwd(socket_desc);
			}
			else
			{
				fprintf(stdout, "No valid command available, try again. See documentation for help\n");
			}
		}
	}
}

void cli_pwd(int socket_desc)
{
	char op_code;
	int file_size;
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

	// Read the size of the path from socket
	if(read_length(socket_desc, &file_size) == -1)
	{
		perror("Failed to read path size\n");
		return;
	}

	// Allocate memory for the path
	working_dir_path = malloc(sizeof(char) * (file_size+1));

	// Read the directory path
	if(readn(socket_desc, working_dir_path, file_size) == -1){
		perror("Failed to read directory\n");
		return;
	}

	working_dir_path[file_size] = '\0';
	fprintf(stdout, "%s\n", working_dir_path);
	free(working_dir_path);
}

