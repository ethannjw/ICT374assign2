// File:		token.c
// author:		Ethan
// Purpose:		To split a string separated by white spaces into their individual token
// note:		

#include <string.h>
#include "token.h"

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
