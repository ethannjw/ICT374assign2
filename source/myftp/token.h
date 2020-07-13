// File:	    token.h
// Author:	    Ng Jing Wei
// Purpose: 	To split a string separated by white spaces into their individual token

#define MAX_TOKEN 100
#define tokenSep " \t\n"

// Purpose:     To split a string separated by white spaces into their individual tokens
// Return:      Number of integer
//              All the tokens
int tokenise (char line[], char *token[]);
