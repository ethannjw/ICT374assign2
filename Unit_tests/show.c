#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("Command line arguments:  %d\n", argc);
    
    int i;
    for (i=0; i<argc; ++i) 
       printf("Command line argument %d:   %s\n", i, argv[i]);
   
    exit(0);
}
