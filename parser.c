#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 1000
#define MAX_VARLEN 11
#define MAX_DGTLEN 5 

#define MAX_SYMBOL_TABLE_SIZE 100

#define MAX_CODE_LENGTH 1000

// Declaration of Token Types
typedef enum {
	nulsym = 1, identsym, 	numbersym, 	plussym, 	minussym,	multsym,  	
	slashsym, 	oddsym, 	eqsym, 		neqsym, 	lessym, 	leqsym,
	gtrsym, 	geqsym, 	lparentsym, rparentsym, commasym, 	semicolonsym,
	periodsym, 	becomessym, beginsym, 	endsym, 	ifsym, 		thensym,
	whilesym, 	dosym, 		callsym, 	constsym, 	varsym, 	procsym,
	writesym,	readsym , 	elsesym
} token_type;

// Input from the Lexical Analyzer
typedef struct lexeme {
	char name[MAX_VARLEN + 1];
	int val;
	int token;
} lexeme;

// Output to the Intermediate Code Generator
typedef struct symbol{
	int kind;						// const = 1, var = 2, proc = 3
	char name[MAX_VARLEN + 1];		// name up to 11 chars
	int val;						// number (ASCII value)
	int level;						// L level
	int addr;						// M address
} symbol;

// Output to the Virtual Machine
typedef struct instruction{
	int op;
	int l;
	int m;
} instruction;

void printError(char *error);

int program(int *tablePosition, lexeme (*table)[MAX_TOKENS]);
int block(int *tablePosition, lexeme (*table)[MAX_TOKENS]);
int statement(int *tablePosition, lexeme (*table)[MAX_TOKENS]);
int condition(int *tablePosition, lexeme (*table)[MAX_TOKENS]);
int expression(int *tablePosition, lexeme (*table)[MAX_TOKENS]);
int term(int *tablePosition, lexeme (*table)[MAX_TOKENS]);
int factor(int *tablePosition, lexeme (*table)[MAX_TOKENS]);

int getToken(int *index);

instruction code[MAX_CODE_LENGTH];
int codePos = 0;

//lexeme table[MAX_TOKENS];
int tablePosition = 0, position = 0;

int symbolPosition = 0;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

int kind, val, level, addr;

int errNum = 0;

int runParser(int options, char *filename) {
	FILE *input;
	int token;
	int position;
	int print = options & a_num;
  
	lexeme table[MAX_TOKENS];

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
	program(&position, &table);

	if (errNum != 0)
		return 1;

	return 0;
}

int getToken(int *index)
{
	return ++(*index);
}

int factor(int *tablePosition, lexeme (*table)[MAX_TOKENS])
{
	int index = *tablePosition;
  
	// factor ::= ident | number | "(" expression ")“
	if (table[index]->token == identsym)
	{
		getToken(&index);
	}
	else if (table[index]->token == numbersym)
	{
		getToken(&index);
	}
	else if (table[index]->token == lparentsym)
	{
		getToken(&index);
		expression(tablePosition, table);
		if (table[index]->token != rparentsym)
		{
			//printf("%s\n", getError(21));
			return ++errNum;
		}
		getToken(&index);
	}
	else
	{
		// error code?
		return ++errNum;
	}

	return 0;
}

int term(int *tablePosition, lexeme (*table)[MAX_TOKENS])
{
	// term ::= factor {("*"|"/") factor}

	factor(tablePosition, table);
	while (table[*tablePosition]->token == multsym || table[*tablePosition]->token == slashsym)
	{
		(*tablePosition)++;
		factor(tablePosition, table);
	}

	return 0;
}

int expression(int *tablePosition, lexeme (*table)[MAX_TOKENS])
{
	// expression ::= [ "+"|"-"] term { ("+"|"-") term}
	if (table[*tablePosition]->token == plussym || table[*tablePosition]->token == minussym)
	{
		(*tablePosition)++;
	}
	term(tablePosition, table);

	while (table[*tablePosition]->token == plussym || table[*tablePosition]->token == minussym)
	{
		(*tablePosition)++;
		term(tablePosition, table);
	}

	return 0;
}

int condition(int *tablePosition, lexeme (*table)[MAX_TOKENS])
{
	// condition ::= "odd" expression
	// 				 | expression rel-op expression.
	if (table[*tablePosition]->token == oddsym)
	{
		(*tablePosition)++;
		expression(tablePosition, table);
	}
	else
	{
		(*tablePosition)++;
		expression(tablePosition, table);

		// Checking if the token is a relational operator. (eqsym = 9) <= X <= (geqsym = 14)
		if (table[*tablePosition]->token < eqsym || table[*tablePosition]->token > geqsym)
		{
			//(*tablePosition)++;

			//printf("%s\n", getError(8));
			return ++errNum;
		}
		(*tablePosition)++;
		expression(tablePosition, table);
	}

	return 0;
}

int statement(int *tablePosition, lexeme (*table)[MAX_TOKENS])
{
	// statement ::= [ ident ":=" expression
	// 				 | "begin" statement { ";" statement } "end" 
	//				 | "if" condition "then" statement
	//				 | "while" condition "do" statement
	//				 | "read" ident
	//				 | "write" ident
	//				 |e]

	if (table[*tablePosition]->token == identsym)
	{
		(*tablePosition)++;
		if (table[(*tablePosition)++]->token != becomessym)
		{
			//printf("%s\n", getError(12));
			return ++errNum;
		}
		expression(tablePosition, table);
	}
	else if (table[*tablePosition]->token == callsym)
	{
		(*tablePosition)++;
		if (table[(*tablePosition)++]->token != identsym)
		{
			//error code?
			return ++errNum;
		}

	}
	else if (table[*tablePosition]->token == beginsym)
	{
		(*tablePosition)++;

		statement(tablePosition, table);

		while (table[*tablePosition]->token == semicolonsym)
		{
			(*tablePosition)++;
			statement(tablePosition, table);
		}
		if (table[(*tablePosition)++]->token != endsym)
		{
			//error code?
			return ++errNum;
		}
	}
	else if (table[*tablePosition]->token == ifsym)
	{
		(*tablePosition)++;
		condition(tablePosition, table);
		if (table[(*tablePosition)++]->token != thensym)
		{
			//error code?
			return ++errNum;
		}
		statement(tablePosition, table);

	}
	else if (table[*tablePosition]->token == whilesym)
	{
		(*tablePosition)++;
		condition(tablePosition, table);
		if (table[(*tablePosition)++]->token != dosym)
		{
			//error code?
			return ++errNum;
		}
		statement(tablePosition, table);

	}
	else
	{
		// error code?
		return ++errNum;
	}

	return 0;
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

int block(int *tablePosition, lexeme (*table)[MAX_TOKENS]) 
{
	// block ::= const-declaration var-declaration statement
	// constdeclaration ::= [ “const” ident "=" number {"," ident "=" number} “;"]
	// var-declaration ::= [ "var" ident {"," ident} “;"]

	if (table[*tablePosition]->token == constsym)
	{
		do {
			(*tablePosition)++;

			if (table[*tablePosition]->token != identsym)
			{
				printError(getError(3));
				return ++errNum;
			}
			else
			{
				// name for new const
				(*tablePosition)++;
			}

			if (table[*tablePosition]->token != eqsym)
			{
				printError(getError(8));
				return ++errNum;
			}
			else
			{
				(*tablePosition)++;
			}

			if (table[*tablePosition]->token != numbersym)
			{
				printError(getError(8));
				return ++errNum;
			}
			else
			{
				// val for new const
				(*tablePosition)++;
			}

		} while (table[*tablePosition]->token == commasym);

		if (table[(*tablePosition)++]->token != semicolonsym)
			{
				printError(getError(4));
				return ++errNum;
			}
	}

	if (table[*tablePosition]->token == varsym)
	{
		(*tablePosition)++;
		do {
			if (table[(*tablePosition)++]->token != identsym)
			{
				// name for new var

				printError(getError(3));
				return ++errNum;
			}
			// next var
		} while (table[*tablePosition]->token == commasym);

		if (table[(*tablePosition)++]->token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}

	}
	while (table[(*tablePosition)++]->token == procsym)
	{
		if (table[(*tablePosition)++]->token != identsym)
		{
			printError(getError(3));
			return ++errNum;
		}
		// name for new proc
		if (table[(*tablePosition)++]->token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}

		// increase lex lvl?
		block(tablePosition, table);
		if (table[(*tablePosition)++]->token != semicolonsym)
		{
			printError(getError(4));
			return ++errNum;
		}
	}
	statement(tablePosition, table);

	return 0;
}

int program(int *tablePosition, lexeme (*table)[MAX_TOKENS])
{
	// program ::= block "."

	block(tablePosition, table);

	if (table[*tablePosition]->token != periodsym)
	{
		// Expected period.
		//printf("%s\n", getError(8));
		return ++errNum;
	}
	return 0;
}

void printError(char *error)
{
	int print = options & a_num;

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

// DELETE ME
int main(void)
{
	return 0;
}