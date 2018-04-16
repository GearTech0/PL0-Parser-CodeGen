#ifndef ERROR_H
#define ERROR_H

char * getError(int errorNum);

char * getError(int errorNum)
{
	char * error = (char *) malloc(140*sizeof(char));
	switch(errorNum)
	{
		case 0:
			stpcpy(error, "Use = instead of :=.");
			break;
		case 1:
			stpcpy(error, "= must be followed by a number.");
			break;
		case 2:
			stpcpy(error, "Identifier must be followed by =.");
			break;
		case 3:
			stpcpy(error, "const, var, procedure must be followed by identifier.");
			break;
		case 4:
			stpcpy(error, "Semicolon or comma missing.");
			break;
		case 5:
			stpcpy(error, "Incorrect symbol after procedure declaration.");
			break;
		case 6:
			stpcpy(error, "Statement expected.");
			break;
		case 7:
			stpcpy(error, "Incorrect symbol after statement part in block.");
			break;
		case 8:
			stpcpy(error, "Period expected.");
			break;
		case 9:
			stpcpy(error, "Semicolon between statements missing.");
			break;
		case 10:
			stpcpy(error, "Undeclared identifier.");
			break;
		case 11:
			stpcpy(error, "Assignment to constant or procedure is not allowed.");
			break;
		case 12:
			stpcpy(error, "Assignment operator expected.");
			break;
		case 13:
			stpcpy(error, "call must be followed by an identifier.");
			break;
		case 14:
			stpcpy(error, "Call of a constant or variable is meaningless.");
			break;
		case 15:
			stpcpy(error, "then expected.");
			break;
		case 16:
			stpcpy(error, "Semicolon or } expected.");
			break;
		case 17:
			stpcpy(error, "do expected.");
			break;
		case 18:
			stpcpy(error, "Incorrect symbol following statement.");
			break;
		case 19:
			stpcpy(error, "Relational operator expected.");
			break;
		case 20:
			stpcpy(error, "Expression must not contain a procedure identifier.");
			break;
		case 21:
			stpcpy(error, "Right parenthesis missing.");
			break;
		case 22:
			stpcpy(error, "The preceding factor cannot begin with this symbol.");
			break;
		case 23:
			stpcpy(error, "An expression cannot begin with this symbol.");
			break;
		case 24:
			stpcpy(error, "This number is too large.");
			break;
	}
	
	return error;
}

#endif