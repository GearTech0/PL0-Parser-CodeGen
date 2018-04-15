#ifndef COMPILER_DRIVER_H
#define COMPILER_DRIVER_H

#include <string.h>

enum options
{
	l_num = 1,
	a_num = 2,
	v_num = 4
};

int run(char option);
int getOptionNum(char * option);

int run(char option)
{
	switch(option)
	{
		// Print list of lexemes/Tokens (Scanner Output) to the screen
		case 'l':
			
		break;
		
		// Print generated assembly code (Parser/Codegen output)
		case 'a':
		
		break;
		
		// Print virtual machine execution stack trace
		case 'v':
		
		break;
		
		// Print nothing to the console except for "in" and "out"
		default:
		
		break;
	}
}

int getOptionNum(char * option)
{
	if(strcmp(option, "-l") == 0)
	{
		return l_num;
	} else if(strcmp(option, "-a") == 0)
	{
		return a_num;
	} else if(strcmp(option, "-v") == 0)
	{
		return v_num;
	} else
	{
		printf("ERROR: Invalid Option");
		exit(1);
	}
}

#endif