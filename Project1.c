// File: proj1.c
// Author: Scott Walker
// Description: The file that runs the lexical analyzer

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "lex.c"
#include "SetLimits.h"

int main (int argc, char * argv[])
{
    SetLimits ();
	enum token_type t;
	if (argc < 2)
	{
		printf ("format: proj1 <filename>\n");
		exit (1);
	}
    FILE* input;
    input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Error: unable to open file\n");
        exit(1);
    }
    printf("Input file: %s\n", argv[1]);
    const int bsize = 512;
    size_t buffersize = bsize;
    char* buffer = (char*) malloc(bsize * sizeof(char));
    int errors = 0;
	while ((t = get_token(input, buffer, &buffersize)) != EOFT)
    {
		printf ("\t%-12s%s\n", token_names[t], get_lexeme());
        if (t == 0)
        {
            errors++;
        }
    }
    printf("%i errors found in input file\n", errors);
    fclose(input);
	return 0;
}
