#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "compiler_driver.h"

int main(int argc, char ** argv)
{
	int options = 0;
	int i = 0;
	
	// for valid argument count
	if(argc < 2)
	{
		printf("ERROR: Invalid argument count");
		exit(1);
	}
	
	if(argc > 2)
	{
		
		// Record options
		for(i = 2; i < argc; i++)
		{
			options |= getOptionNum(argv[i]);
		}
	}
	
	cdRun(options, argv[1]);
	
	return 0;
}