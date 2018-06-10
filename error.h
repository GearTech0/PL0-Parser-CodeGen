#ifndef ERROR_H
#define ERROR_H

#include <string.h>

char * getError(int errorNum)
{
	char * error = (char *) malloc(140*sizeof(char));
	switch(errorNum)
	{
		case 0:
			strcpy(error, "Use = instead of :=.");
			break;
		case 1:
			strcpy(error, "= must be followed by a number.");
			break;
		case 2:
			strcpy(error, "Identifier must be followed by =.");
			break;
		case 3:
			strcpy(error, "const, var, procedure must be followed by identifier.");
			break;
		case 4:
			strcpy(error, "Semicolon or comma missing.");
			break;
		case 5:
			strcpy(error, "Incorrect symbol after procedure declaration.");
			break;
		case 6:
			strcpy(error, "Statement expected.");
			break;
		case 7:
			strcpy(error, "Incorrect symbol after statement part in block.");
			break;
		case 8:
			strcpy(error, "Period expected.");
			break;
		case 9:
			strcpy(error, "Semicolon between statements missing.");
			break;
		case 10:
			strcpy(error, "Undeclared identifier.");
			break;
		case 11:
			strcpy(error, "Assignment to constant or procedure is not allowed.");
			break;
		case 12:
			strcpy(error, "Assignment operator expected.");
			break;
		case 13:
			strcpy(error, "call must be followed by an identifier.");
			break;
		case 14:
			strcpy(error, "Call of a constant or variable is meaningless.");
			break;
		case 15:
			strcpy(error, "then expected.");
			break;
		case 16:
			strcpy(error, "Semicolon or } expected.");
			break;
		case 17:
			strcpy(error, "do expected.");
			break;
		case 18:
			strcpy(error, "Incorrect symbol following statement.");
			break;
		case 19:
			strcpy(error, "Relational operator expected.");
			break;
		case 20:
			strcpy(error, "Expression must not contain a procedure identifier.");
			break;
		case 21:
			strcpy(error, "Right parenthesis missing.");
			break;
		case 22:
			strcpy(error, "The preceding factor cannot begin with this symbol.");
			break;
		case 23:
			strcpy(error, "An expression cannot begin with this symbol.");
			break;
		case 24:
			strcpy(error, "This number is too large.");
			break;
	}
	
	return error;
}

#endif