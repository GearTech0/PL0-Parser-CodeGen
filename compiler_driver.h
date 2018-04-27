/*
	COP3402 - Spring 2018

	System Software Assignment 3
	PL/0 Parser/Code Generator

	Authors - Daquaris Chadwick, Marcus Sooter
*/

#ifndef COMPILER_DRIVER_H
#define COMPILER_DRIVER_H

enum options
{
	l_num = 1,
	a_num = 2,
	v_num = 4
};

#include <string.h>

#include "error.h"
#include "scanner.h"
#include "parser.h"
#include "virtual_machine.h"

int cdRun(int options, char * filename);
int getOptionNum(char * option);

int cdRun(int options, char * filename)
{
	// Run Scanner
	if(options & l_num)
	{
		printf("-------------------------------------------\n");
		printf("LIST OF LEXEMES/TOKENS:\n");
	}
	lexRun(options, filename);
  
	// Run Parser
	int parser = runParser(options, "lex_out.txt");
	if(options & a_num)
	{
		if(parser)
		{
			printf("Code is not syntactically correct.\n");
			return 0;
		}
		printf("Code is syntactically correct. Assembly code generated successfully.\n");
		printf("-------------------------------------------\n");
		printf("GENERATED INTERMEDIATE CODE:\n");
	}
 
	// Run Virtual Machine
	if(options & v_num)
	{
		printf("-------------------------------------------\n");
		printf("VIRTUAL MACHINE TRACE:\nInitial Values:\nPC	BP	SP	Stack\n0	1	0	0 \n\nStack Trace:\n");
	}
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