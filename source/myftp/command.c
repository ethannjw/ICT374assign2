// File:		command.c for Lab 9
// Author:		Ng Jing Wei 33804877
// Purpose:		to separate a list of tokens into a sequence of commands.
// Assumptions:	any two successive commands in the list of tokens are separated
//			    by one of the following command separators:
//				"|"  - pipe to the next command
//				"&"  - shell does not wait for the proceeding command to terminate
//				";"  - shell waits for the proceeding command to terminate

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

Command do_prompt(Command commandArray[])
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
				fprintf(stdout, "Usage: Command is only separated by individual '|', '&', ';'\n");
				continue;
			}
            return commandArray;
		}
	}
}


int separator(char *token)
// return 1 if the token is a command separator
// return 0 otherwise
{
    int i = 0;
    char *commandSeparators[] = {pipeSep, conSep, seqSep, NULL};

    while (commandSeparators[i] != NULL)
    {
        if (strcmp(commandSeparators[i], token) == 0)
        {
            return 1;
        }
        i++;
    }

    return 0;
}

// fill one command structure with the details
void fillCommandStruct(Command *cp, int first, int last, char *sep)
{
	cp->first = 		first;
	cp->last = 		last - 1;
	cp->sep = 		sep;
}

// process standard in/out redirections in a command
void searchRedirection(char *token[], Command *cp)
{
     int i;
     for (i=cp->first; i<=cp->last; ++i) {
         if (strcmp(token[i], "<") == 0) {   // standard input redirection
              cp->stdin_file = token[i+1];
              ++i;
         } else if (strcmp(token[i], ">") == 0) { // standard output redirection
              cp->stdout_file = token[i+1];
              ++i;
         }
     }
}

// build command line argument vector for execvp function
void buildCommandArgumentArray(char *token[], Command *cp)
{
     int n = (cp->last - cp->first + 1)    // the numner of tokens in the command
          + 1;                             // the last element in argv must be a NULL

     // re-allocate memory for argument vector
     cp->argv = (char **) realloc(cp->argv, sizeof(char *) * n);
     if (cp->argv == NULL) {
         perror("realloc");
         exit(1);
     }

     // build the argument vector
     int i;
     int k = 0;
     for (i=cp->first; i<= cp->last; ++i ) {
         if (strcmp(token[i], ">") == 0 || strcmp(token[i], "<") == 0)
             ++i;    // skip off the std in/out redirection
         else {
             cp->argv[k] = token[i];
             ++k;
         }
     }
     cp->argv[k] = NULL;
	cp->argc = k;
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

     int first=0;   // points to the first tokens of a command
     int last;      // points to the last  tokens of a command
     char *sep;     // command separator at the end of a command
     int c = 0;     // command index
     for (i=0; i<nTokens; ++i) {
         last = i;
         if (separator(token[i])) {
             sep = token[i];
             if (first==last)  // two consecutive separators
                 return -2;
             fillCommandStruct(&(command[c]), first, last, sep);
             ++c;
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
