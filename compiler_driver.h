#ifndef COMPILER_DRIVER_H
#define COMPILER_DRIVER_H

#include <string.h>

enum options
{
	l_num = 1,
	a_num = 2,
	v_num = 4
};

int run(int options, char * filename);
int getOptionNum(char * option);

int run(int options, char * filename)
{
	if(options & l_num)
	{
		
	}
	if(options & a_num)
	{
		
	}
	if(options & v_num)
	{
		
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