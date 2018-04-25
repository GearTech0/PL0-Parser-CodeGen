/*
	COP3402 - Spring 2018

	System Software Assignment 3
	PL/0 Parser/Code Generator

	Authors - Daquaris Chadwick, Marcus Sooter
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 1000
#define MAX_VARLEN 11
#define MAX_DGTLEN 5 
#define MAX_CODE_LENGTH 1000

#define MAX_SYMBOL_TABLE_SIZE 100

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
    int l;
    int m;
} instr;

instr code[MAX_CODE_LENGTH];
int codePos = 0;

int program(lexeme (*table)[MAX_TOKENS]);
int block(lexeme (*table)[MAX_TOKENS]);
int statement(lexeme (*table)[MAX_TOKENS]);
int condition(lexeme (*table)[MAX_TOKENS]);
int expression(lexeme (*table)[MAX_TOKENS]);
int term(lexeme (*table)[MAX_TOKENS]);
int factor(lexeme (*table)[MAX_TOKENS]);

int getToken();


lexeme table[MAX_TOKENS];
int tablePosition = 0, position = 0;
int errNum = 0;

int symbolPosition = 0;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

int kind, val, level, addr;
int print = 0;


void printError(char *error)
{
    if (print != 0)
    {
        printf("%s\n", error);
    }
}

void emit(int OP, int L, int M)
{
    code[codePos].op = OP;
    code[codePos].l = L;
    code[codePos].m = M;
}

int runParser(int options, char *filename) {
	FILE *input;

	int token;
	int position;
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

	position = 0;
	program(&table);

	return 0;
}

int getToken()
{
	return ++tablePosition;
}

int factor(lexeme (*table)[MAX_TOKENS])
{
	
	// factor ::= ident | number | "(" expression ")“
	if (table[tablePosition]->token == identsym)
	{
		getToken();
	}
	else if (table[tablePosition]->token == numbersym)
	{
		getToken();
	}
	else if (table[tablePosition]->token == lparentsym)
	{
		getToken();
		expression(table);
		if (table[tablePosition]->token != rparentsym)
		{
			printError(getError(21));
			return ++errNum;
		}
		getToken();
	}
	else
	{
		// error code?
	}

	return 0;
}

int term(lexeme (*table)[MAX_TOKENS])
{
	// term ::= factor {("*"|"/") factor}

	factor(table);
	while (table[tablePosition]->token == multsym || table[tablePosition]->token == slashsym)
	{
		getToken();
		factor(table);
	}

	return 0;
}

int expression(lexeme (*table)[MAX_TOKENS])
{
	// expression ::= [ "+"|"-"] term { ("+"|"-") term}
	if (table[tablePosition]->token == plussym || table[tablePosition]->token == minussym)
	{
		getToken();
	}
	term(table);

	while (table[tablePosition]->token == plussym || table[tablePosition]->token == minussym)
	{
		getToken();
		term(table);
	}

	return 0;
}

int condition(lexeme (*table)[MAX_TOKENS])
{
	// condition ::= "odd" expression
	// 				 | expression rel-op expression.
	if (table[tablePosition]->token == oddsym)
	{
		getToken();
		expression(table);
	}
	else
	{
		getToken();
		expression(table);

		// Checking if the token is a relational operator. (eqsym = 9) <= X <= (geqsym = 14)
		if (table[tablePosition]->token < eqsym || table[tablePosition]->token > geqsym)
		{
			//getToken();

			printError(getError(4));
			return ++errNum;
		}
		getToken();
		expression(table);
	}

	return 0;
}

int statement(lexeme (*table)[MAX_TOKENS])
{
	// statement ::= [ ident ":=" expression
	// 				 | "begin" statement { ";" statement } "end" 
	//				 | "if" condition "then" statement
	//				 | "while" condition "do" statement
	//				 | "read" ident
	//				 | "write" ident
	//				 |e]

	if (table[tablePosition]->token == identsym)
	{
		getToken();
		if (table[(tablePosition)++]->token != becomessym)
		{
			printError(getError(12));
			return ++errNum;
		}
		expression(table);
	}
	else if (table[tablePosition]->token == callsym)
	{
		getToken();
		if (table[(tablePosition)++]->token != identsym)
		{
			//error code?
			return ++errNum;
		}

	}
	else if (table[tablePosition]->token == beginsym)
	{
		getToken();

		statement(table);

		while (table[tablePosition]->token == semicolonsym)
		{
			getToken();
			statement(table);
		}
		if (table[(tablePosition)++]->token != endsym)
		{
			//error code?
			return ++errNum;
		}
	}
	else if (table[tablePosition]->token == ifsym)
	{
		getToken();
		condition(table);
		if (table[(tablePosition)++]->token != thensym)
		{
			//error code?
			return ++errNum;
		}
		statement(table);

	}
	else if (table[tablePosition]->token == whilesym)
	{
		getToken();
		condition(table);
		if (table[(tablePosition)++]->token != dosym)
		{
			//error code?
			return ++errNum;
		}
		statement(table);

	}
	else
	{
		// error code?
		return ++errNum;
	}

	return 0;
}

/*
int varDeclaration(lexeme *table)
{
	// var-declaration ::= [ "var" ident {"," ident} “;"]

}

int constDeclaration(lexeme *table)
{
	// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"]

}
*/
int enter(int kind, char *name, int value, int addr)
{
	int i;
	for (i = 0; i < symbolPosition; i++)
	{
		if (symbol_table[i].level == level && strcmp(symbol_table[i].name, name) == 0)
		{
			return 0;
		}
	}
	
	if (kind == 1)
	{
		symbol_table[i].kind = kind;
		strcpy(symbol_table[i].name, name);
		symbol_table[i].val = value;
	}
	else if (kind == 2 || kind == 3)
	{
		symbol_table[i].kind = kind;
		strcpy(symbol_table[i].name, name);
		symbol_table[i].addr = addr;
		symbol_table[i].level = level;
	}
	
	return kind;
}

int block(lexeme (*table)[MAX_TOKENS]) 
{
	// block ::= const-declaration var-declaration statement
	// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"]
	// var-declaration ::= [ "var" ident {"," ident} “;"]
	if (table[tablePosition]->token == constsym)
	{
		do {
			getToken();

			if (table[tablePosition]->token != identsym)
			{
				printError(getError(3));
				return ++errNum;
			}
			
			getToken();

			if (table[tablePosition]->token != eqsym)
			{
				printError(getError(8));
				return ++errNum;
			}
			getToken();

			if (table[tablePosition]->token != numbersym)
			{
				printError(getError(8));
				return ++errNum;
			}
			enter(1, table[tablePosition]->name, table[tablePosition]->val, 0);
			getToken();

		} while (table[tablePosition]->token == commasym);

		if (table[tablePosition]->token != semicolonsym)
			{
				printError(getError(4));
				return ++errNum;
			}
      getToken();
	}

	if (table[tablePosition]->token == varsym)
	{
		getToken();
		do {
			if (table[(tablePosition)++]->token != identsym)
			{
				printError(getError(3));
				return ++errNum;
			}
		} while (table[tablePosition]->token == commasym);

		if (table[(tablePosition)++]->token != semicolonsym)
		{
			printError(getError(4));
            return ++errNum;
		}

	}
	while (table[(tablePosition)++]->token == procsym)
	{
		if (table[(tablePosition)++]->token != identsym)
		{
			printError(getError(3));
            return ++errNum;
		}
		if (table[(tablePosition)++]->token != semicolonsym)
		{
			printError(getError(4));
            return ++errNum;
		}
		block(table);
		if (table[(tablePosition)++]->token != semicolonsym)
		{
			printError(getError(4));
            return ++errNum;
		}
	}
	statement(table);

	return 0;
}

int program(lexeme (*table)[MAX_TOKENS])
{
	// program ::= block "."

	block(table);

	if (table[tablePosition]->token != periodsym)
	{
		// Expected period.
		printError(getError(8));
        return ++errNum;
	}
	return 0;
}
#endif