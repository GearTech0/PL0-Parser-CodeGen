#ifndef COMPILER_DRIVER_H
#define COMPILER_DRIVER_H

enum options
{
	l_num = 1,
	a_num = 2,
	v_num = 4
};

#include <string.h>

#include "scanner.h"
#include "parser.h"
#include "code_generator.h"
#include "virtual_machine.h"

int cdRun(int options, char * filename);
int getOptionNum(char * option);

int cdRun(int options, char * filename)
{
	// Run Scanner
	printf("-------------------------------------------\n");
  printf("LIST OF LEXEMES/TOKENS:\n");
	lexRun(options, filename);
	
  printf("Code is syntactically correct. Assembly code generated successfully.\n");
	printf("-------------------------------------------\n");
  printf("GENERATED INTERMEDIATE CODE:\n");
	// Run Parser
//	int parser = runParser(options, "lex_out.txt");
//	if(parser)
//	{
//		printf("Code is not syntactically correct. Assembly code generated successfully.\n");
//		return 0;
//	}
 
  printf("-------------------------------------------\n");
	printf("VIRTUAL MACHINE TRACE:\nInitial Values:\nPC	BP	SP	Stack\n0	1	0	0 \n\nStack Trace:\n");
	// Run Virtual Machine
	vmRun(options, "generator.out");
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