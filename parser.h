#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 1000
#define MAX_VARLEN 11
#define MAX_DGTLEN 5 

#define MAX_SYMBOL_TABLE_SIZE 100

#define MAX_CODE_LENGTH 1000

// Declaration of Token Types
/*
typedef enum {
	nulsym = 1, identsym, 	numbersym, 	plussym, 	minussym,	multsym,  	
	slashsym, 	oddsym, 	eqsym, 		neqsym, 	lessym, 	leqsym,
	gtrsym, 	geqsym, 	lparentsym, rparentsym, commasym, 	semicolonsym,
	periodsym, 	becomessym, beginsym, 	endsym, 	ifsym, 		thensym,
	whilesym, 	dosym, 		callsym, 	constsym, 	varsym, 	procsym,
	writesym,	readsym , 	elsesym
} token_type;
*/

// Input from the Lexical Analyzer
/*
typedef struct lexeme {
	char name[MAX_VARLEN + 1];
	int val;
	int token;
} lexeme;
*/

// Output to the Intermediate Code Generator
typedef struct symbol{
	int kind;						// const = 1, var = 2, proc = 3
	char name[MAX_VARLEN + 1];		// name up to 11 chars
	int val;						// number (ASCII value)
	int level;						// L level
	int addr;						// M address
} symbol;

// Output to the Virtual Machine
typedef struct instr{
	int op;
	int r;
	int l;
	int m;
} instr;

void printError(char *error);

int program(int *tablePosition);
int block(int *tablePosition);
int statement(int *tablePosition);
int condition(int *tablePosition);
int expression(int *tablePosition);
int term(int *tablePosition);
int factor(int *tablePosition);

//int getToken(int *index);

instr code[MAX_CODE_LENGTH];
int codePosition = 0;

int lexicalLevel = 0;

lexeme table[MAX_TOKENS];

int symbolPosition = 0;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

int kind, val, level, addr;

int errNum = 0;
int print;

int runParser(int options, char *filename) {
	FILE *input;
	int token;
	int tablePosition = 0;
	
	print = options & a_num;
  
	//lexeme table[MAX_TOKENS];

	input = fopen(filename, "r");
	if (input == NULL)
	{
		printf("Error: filename does not exist!\n");
		return 0;
	}

	while (fscanf(input, "%d", &token) != EOF)
	{
		table[tablePosition].token = token;
		// Token is an identifier.
		if (token == 2)
		{
			fscanf(input, "%s", table[tablePosition].name);
		}
		// Token is a number.
		else if (token == 3)
		{
			fscanf(input, "%d", &table[tablePosition].val);
		}

		tablePosition++;
	}

	tablePosition = 0;
	program(&tablePosition);

	if (errNum != 0)
		return 1;

	return 0;
}

int factor(int *tablePosition)
{  
	// factor ::= ident | number | "(" expression ")“
	if (table[*tablePosition].token == identsym)
	{
		(*tablePosition)++;
	}
	else if (table[*tablePosition].token == numbersym)
	{
		(*tablePosition)++;
	}
	else if (table[*tablePosition].token == lparentsym)
	{
		(*tablePosition)++;
		expression(tablePosition);
		if (table[*tablePosition].token != rparentsym)
		{
			//printf("%s\n", getError(21));
			return ++errNum;
			//printf("CHECK K!\n");
		}
		(*tablePosition)++;
	}
	else
	{
		// error code?
		//printf("CHECK J!\n");
		return ++errNum;
	}

	return 0;
}

int term(int *tablePosition)
{
	// term ::= factor {("*"|"/") factor}

	factor(tablePosition);
	while (table[*tablePosition].token == multsym || table[*tablePosition].token == slashsym)
	{
		(*tablePosition)++;
		factor(tablePosition);
	}

	return errNum;
}

int expression(int *tablePosition)
{
	// expression ::= [ "+"|"-"] term { ("+"|"-") term}
	if (table[*tablePosition].token == plussym || table[*tablePosition].token == minussym)
	{
		(*tablePosition)++;
	}
	term(tablePosition);

	while (table[*tablePosition].token == plussym || table[*tablePosition].token == minussym)
	{
		(*tablePosition)++;
		term(tablePosition);
	}

	return errNum;
}

int condition(int *tablePosition)
{
	// condition ::= "odd" expression
	// 				 | expression rel-op expression.
	if (table[*tablePosition].token == oddsym)
	{
		// needs code insert for 'odd'
		(*tablePosition)++;
		expression(tablePosition);
	}
	else
	{
		expression(tablePosition);

		// Checking if the token is a relational operator. (eqsym = 9) <= X <= (geqsym = 14)
		if (table[*tablePosition].token < eqsym || table[*tablePosition].token > geqsym)
		{
			//printf("%s\n", getError(8));
			//printf("CHECK H!\n");
			return ++errNum;
		}
		else
		{
			// check for which relational operator and then emit the appropriate code

		}
		(*tablePosition)++;
		expression(tablePosition);
	}

	return errNum;
}

int statement(int *tablePosition)
{
	// statement ::= [ ident ":=" expression
	// 				 | "begin" statement { ";" statement } "end" 
	//				 | "if" condition "then" statement
	//				 | "while" condition "do" statement
	//				 | "read" ident
	//				 | "write" ident
	//				 |e]

	if (table[*tablePosition].token == identsym)
	{
		(*tablePosition)++;
		if (table[*tablePosition].token != becomessym)
		{
			//printf("%s\n", getError(12));
			//printf("CHECK G!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		expression(tablePosition);
	}
	else if (table[*tablePosition].token == callsym)
	{
		(*tablePosition)++;
		if (table[*tablePosition].token != identsym)
		{
			//error code?
			//printf("CHECK F!\n");
			return ++errNum;
		}
		(*tablePosition)++;

	}
	else if (table[*tablePosition].token == beginsym)
	{
		(*tablePosition)++;

		statement(tablePosition);

		while (table[*tablePosition].token == semicolonsym)
		{
			(*tablePosition)++;
			statement(tablePosition);
		}

		if (table[*tablePosition].token != endsym)
		{
			//error code?
			//printf("CHECK E!\n");
			//printf("token: %d\n", table[*tablePosition].token);
			return ++errNum;
		}
		(*tablePosition)++;
	}
	else if (table[*tablePosition].token == ifsym)
	{
		(*tablePosition)++;
		condition(tablePosition);
		if (table[*tablePosition].token != thensym)
		{
			//error code?
			//printf("CHECK D!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		statement(tablePosition);

	}
	else if (table[*tablePosition].token == whilesym)
	{
		(*tablePosition)++;
		condition(tablePosition);
		if (table[*tablePosition].token != dosym)
		{
			//error code?
			//printf("CHECK C!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		statement(tablePosition);

	}
	else if (table[*tablePosition].token == writesym)
	{
		(*tablePosition)++;
		if (table[*tablePosition].token != identsym)
		{
			//error code?
			//printf("CHECK AA!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		if (table[*tablePosition].token != semicolonsym)
		{
			//error code?
			//printf("CHECK AB!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		statement(tablePosition);

	}
	else if (table[*tablePosition].token == readsym)
	{
		(*tablePosition)++;
		if (table[*tablePosition].token != identsym)
		{
			//error code?
			//printf("CHECK AA!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		if (table[*tablePosition].token != semicolonsym)
		{
			//error code?
			//printf("CHECK AB!\n");
			return ++errNum;
		}
		(*tablePosition)++;
		statement(tablePosition);

	}
	/*
	else
	{
		// error code?
		printf("CHECK B!\n");
		return ++errNum;
	}
	*/

	return errNum;
}

/*
int varDeclaration(int *tablePosition, lexeme *table)
{
	// var-declaration ::= [ "var" ident {"," ident} “;"]
}
int constDeclaration(int *tablePosition, lexeme *table)
{
	// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"]
}
*/

int block(int *tablePosition) 
{
	// block ::= const-declaration var-declaration statement
	// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"]
	// var-declaration ::= [ "var" ident {"," ident} “;"]

	if (table[*tablePosition].token == constsym)
	{
		do {
			(*tablePosition)++;

			if (table[*tablePosition].token != identsym)
			{
				printError(getError(3));
				return ++errNum;
			}
			// name for new const
			(*tablePosition)++;

			if (table[*tablePosition].token != eqsym)
			{
				printError(getError(8));
				return ++errNum;
			}
			(*tablePosition)++;

			if (table[*tablePosition].token != numbersym)
			{
				printError(getError(8));
				return ++errNum;
			}
			// val for new const
			(*tablePosition)++;

		} while (table[*tablePosition].token == commasym);

		if (table[(*tablePosition)++].token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}
	}

	if (table[*tablePosition].token == varsym)
	{
		do {
			(*tablePosition)++;
			if (table[*tablePosition].token != identsym)
			{
				// name for new var
				printError(getError(3));
				return ++errNum;
			}
			(*tablePosition)++;
			// next var
		} while (table[*tablePosition].token == commasym);

		if (table[(*tablePosition)++].token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}

	}
	while (table[*tablePosition].token == procsym)
	{
		(*tablePosition)++;
		if (table[*tablePosition].token != identsym)
		{
			printError(getError(3));
			return ++errNum;
		}
		(*tablePosition)++;
		// name for new proc
		if (table[*tablePosition].token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}
		(*tablePosition)++;

		// increase lex lvl?
		block(tablePosition);
		if (table[*tablePosition].token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}
		(*tablePosition)++;
	}
	statement(tablePosition);

	return errNum;
}

int program(int *tablePosition)
{
	// program ::= block "."

	block(tablePosition);

	if (table[*tablePosition].token != periodsym)
	{
		// Expected period.
		//printf("%s\n", getError(8));
		printf("CHECK A!\n");
		printf("token: %d\n", table[*tablePosition].token);
		printf("position: %d\n", *tablePosition);
		return ++errNum;
	}
	return errNum;
}

void printError(char *error)
{
	//int print = options & a_num;

	if (print != 0)
	{
		printf("%s\n", error);
	}
}

void emit(int OP, int L, int M)
{
	char *error;

	strcpy(error, "Maximum Code Length Exceeded.\n");
	if (codePosition > MAX_CODE_LENGTH)
	{
		printError(error);
	}
	else
	{
		code[codePosition].op = OP;
		code[codePosition].l = L;
		code[codePosition].m = M;
		codePosition++;

	}
}
 
// DELETE ME
/*
int main(int argc, char *argv[])
{
	int ErrCheck = 0;
	
	ErrCheck = runParser(2, argv[1]);


	return 0;
}
*/

#endif