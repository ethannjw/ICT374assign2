/*  File:		command.c for assignment 2 myftp (client side)
 *  Author:		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
 *  Purpose:		Contains the core functions for the client operation. 
 *  Assumptions:	
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"


void rmReturnChar(char *line)
{
	// Removes last char if it is a return char
	int n = strlen(line);
	if (line[n-1]=='\n')
	{
		line[n-1]='\0';
	}
}

void cmd_prompt(Command &commandArray[])
{
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
			return 0;
		}
		else
		{
		    	bzero(tokenArray, sizeof(tokenArray));
			numTok = tokenise(input, tokenArray);
			numCommand = separateCommands(tokenArray, commandArray);
			if (numCommand < 0)
			{
				fprintf(stdout, "Usage: Command [optional: file/dir path]\n");
				continue;
			}
			else
			{
				// insert all the commands here
			}

		}
	}
}

// fill one command structure with the details
void fillCommandStruct(Command *cp, int first, int last)
{
	cp->first = first;
	cp->last = last - 1;
}

// build command line argument array in command array
void buildCommandArgumentArray(char *token[], Command *cp)
{
     int n = (cp->last - cp->first + 1) + 1;    // the numner of tokens in the command
                                                // the last element in argv must be a NULL

    // re-allocate memory for argument vector
    cp->argv = (char **) realloc(cp->argv, sizeof(char *) * n);
    if (cp->argv == NULL) {
        perror("realloc");
        return;
    }

    // build the argument vector
    int k = 0;
    for (int i = cp->first; i <= cp->last; ++i ) {
        cp->argv[k] = token[i];
        k++;
    }
    cp->argv[k] = NULL;
	cp->argc = k + 1;   // number of arguments
}

int separateCommands(char *token[], Command command[])
{
     int i;
     int nTokens;

     // find out the number of tokens
     i = 0;
     while (token[i] != NULL) ++i;
     nTokens = i;

     // if empty command line
     if (nTokens == 0)
          return 0;

     // check the first token
     if (separator(token[0]))
          return -3;

     // check last token, add ";" if necessary
     if (!separator(token[nTokens-1])) {
          token[nTokens] = seqSep;
          ++nTokens;
     }

     int first = 0; // points to the first tokens of a command
     int last;      // points to the last  tokens of a command
     int c = 0;     // command index
     for (i=0; i<nTokens; ++i) {
         last = i;
         if (separator(token[i])) {
             sep = token[i];
             if (first==last)  // two consecutive separators
                 return -2;
             fillCommandStruct(&(command[c]), first, last);
             c++;
             first = i+1;
         }
     }

     // check the last token of the last command
     if (strcmp(token[last], pipeSep) == 0) { // last token is pipe separator
          return -4;
     }

     // calculate the number of commands
     int nCommands = c;

     // handle standard in/out redirection and build command line argument vector
     for (i=0; i<nCommands; ++i) {
         searchRedirection(token, &(command[i]));
         buildCommandArgumentArray(token, &(command[i]));
     }

     return nCommands;
}

void send_pwd(int socket_desc, char *token)
{
	char op_code;
	int file_size;
	char working_dir_path;

	if(write_opcode(socket_desc,PWD_OP) == -1){
		perror("Failed to send pwd\n");
		return;
	}

	if(read_opcode(socket_desc,&op_code) == -1){
		perror("Unable to read opcode\n");
		return;
	}

	if(op_code != PWD_OP){
		perror("Invalid opcode from pwd: %c\n", op_code);
		return;
	}

	if(read_length(socket_desc, &file_size) == -1){
		perror("Failed to read filesize\n");
		return;
	}

	malloc(working_dir_path, sizeof((char) * (file_size+1)));

	if(read_nbytes(socket_desc, directory, file_size) == -1){
		perror("Failed to read directory\n");
		return;
	}

	working_dir_path[file_size] = '\0';
	fprintf(stdout, "%s\n", working_dir_path);
	free(working_dir_path);
}

