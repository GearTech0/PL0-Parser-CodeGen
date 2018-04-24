#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 1000
#define MAX_VARLEN 11
#define MAX_DGTLEN 5 

// Declaration of Token Types
typedef enum {
	nulsym = 1, identsym, 	numbersym, 	plussym, 	minussym,	multsym,  	
	slashsym, 	oddsym, 	eqsym, 		neqsym, 	lessym, 	leqsym,
	gtrsym, 	geqsym, 	lparentsym, rparentsym, commasym, 	semicolonsym,
	periodsym, 	becomessym, beginsym, 	endsym, 	ifsym, 		thensym,
	whilesym, 	dosym, 		callsym, 	constsym, 	varsym, 	procsym,
	writesym,	readsym , 	elsesym
} token_type;

typedef struct lexeme {
	char name[MAX_VARLEN + 1];
	int val;
	int token;
} lexeme;

// Returns 1 when within range of valid characters,
//	otherwise return 0.
int digit(char ch)
{
	// TRUE
	if (ch >= '0' && ch <= '9')
		return 1;
	// FALSE
	else
		return 0;
}

// Returns 1 when within range of valid characters,
//	otherwise return 0.
int letter(char ch)
{
	// TRUE
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	// FALSE
	else
		return 0;
}

void number(char *code, int *codePos, lexeme *lex, int *lexPos)
{
	int i = 0, j, value = 0, isValid = 1;

	while (isValid)
	{
		if (i > MAX_DGTLEN)
		{
			// Alternate method (Continue after found error)
			/*
			fprintf(stdout, "Error: Invalid identifier.\n");
			isValid = 0;
			*/
			fprintf(stdout, "Error: Invalid identifier.");
			exit(0);
		}

		if (!digit(code[*codePos + i]))
		{
			if (letter(code[*codePos + i]))
			{
				// Alternate method (Continue after found error)
				/*
				j = 1;
				while (letter(code[*codePos + i + j]) || digit(code[*codePos + i + j]))
					j++;

				*codePos += i + j;
				fprintf(stdout, "Error: Invalid identifier.\n");
				return;
				*/
			}
			else
			{
				break;
			}
		}
		else
		{
			// Horner's method for calculating polynomials in 0(n) time!
			//	multiply by base (10 for digits), then add current base^i num
			value *= 10;
			value += (int)(code[*codePos + i] - '0');

		}
		i++;
	}

	if (isValid)
	{
		lex->token = numbersym;
		lex->val = value;
		(*lexPos)++;

		// Convert value to string and put into name field
		for (j = i - 1; j >= 0; j--)
		{
			lex->name[j] = (char)('0' + (value % 10));
			value /= 10;
		}
	}
	*codePos += i;
}

void ident(char *code, int *codePos, lexeme *lex, int *lexPos)
{
	int i = 0, j, start = code[*codePos];
	int isValid = 1;

	while (isValid)
	{

		if (i > MAX_VARLEN)
		{
			// Alternate method (Continue after found error)
			/*
			j = 1;
			while (letter(code[*codePos + i + j]) || digit(code[*codePos + i + j]))
				j++;

			*codePos += i + j;
			fprintf(stdout, "Error: Invalid identifier.\n");
			return;
			*/
			fprintf(stdout, "Error: Invalid identifier.");
			exit(0);
		}

		if (!(letter(code[*codePos + i]) || digit(code[*codePos + i])))
		{
			break;
		}
		i++;
	}

	if (isValid)
	{
		(*lexPos)++;
		lex->token = identsym;
		strncpy(lex->name, code + *codePos, i);
	}
	*codePos += i;
}

int Lex(char *code, lexeme *table, int length)
{
	token_type token;
	int codePos = 0, tempPos = 0, lexPos = 0;

	while (codePos < length)
	{

		// This section covers all reserved words, in the case that the word does not match, 
		//	it will be treated as a potential identifier.
		//
		// A character which is not a letter or number after each keyword is required for verification.
		//
		// When multiple reserved words begin with the same letter they must be split into a nested if statement

		// ALL 14 RESERVED WORDS MUST BE LOWER CASE !

		if (letter(code[codePos]))
		{
			// ODD
			if (code[codePos    ] == 'o'   &&
				code[codePos + 1] == 'd'   &&
				code[codePos + 2] == 'd'   &&
				!letter(code[codePos + 3]) && !digit(code[codePos + 3]))
			{
				codePos += 3;
				strncpy(table[lexPos].name, "odd", 3);
				table[lexPos++].token = oddsym;

			}
			// BEGIN
			else if (code[codePos    ] == 'b'   &&
					 code[codePos + 1] == 'e'   &&
					 code[codePos + 2] == 'g'   &&
					 code[codePos + 3] == 'i'   &&
					 code[codePos + 4] == 'n'   &&
					 !letter(code[codePos + 5]) && !digit(code[codePos + 5]))
			{
				codePos += 5;
				strncpy(table[lexPos].name, "begin", 5);
				table[lexPos++].token = beginsym;

			}
			// END & ELSE
			else if (code[codePos] == 'e') //2
			{
				if (code[codePos + 1] == 'n'   &&
					code[codePos + 2] == 'd'   &&
					!letter(code[codePos + 3]) && !digit(code[codePos + 3]))
				{
					codePos += 3;
					strncpy(table[lexPos].name, "end", 3);
					table[lexPos++].token = endsym;

				}
				else if (code[codePos + 1] == 'l'   &&
						 code[codePos + 2] == 's'   &&
						 code[codePos + 3] == 'e'   &&
						 !letter(code[codePos + 4]) && !digit(code[codePos + 4]))
				{
					codePos += 4;
					strncpy(table[lexPos].name, "else", 4);
					table[lexPos++].token = elsesym;
				}
				else
				{
					// Possible identifier
					ident(code, &codePos, table + lexPos, &lexPos);
				}
			}
			// IF
			else if (code[codePos    ] == 'i'   &&
					 code[codePos + 1] == 'f'   &&
					 !letter(code[codePos + 2]) && !digit(code[codePos + 2]))
			{
				codePos += 2;
				strncpy(table[lexPos].name, "if", 2);
				table[lexPos++].token = ifsym;
				
			}
			// THEN
			else if (code[codePos    ] == 't'   &&
					 code[codePos + 1] == 'h'   &&
					 code[codePos + 2] == 'e'   &&
					 code[codePos + 3] == 'n'   &&
					 !letter(code[codePos + 4]) && !digit(code[codePos + 4]))
			{
				codePos += 4;
				strncpy(table[lexPos].name, "then", 4);
				table[lexPos++].token = thensym;
				
			}
			// WHILE & WRITE
			else if (code[codePos] == 'w') //2
			{
				if (code[codePos + 1] == 'h'   &&
					code[codePos + 2] == 'i'   &&
					code[codePos + 3] == 'l'   &&
					code[codePos + 4] == 'e'   &&
					!letter(code[codePos + 5]) && !digit(code[codePos + 5]))
				{
					codePos += 5;
					strncpy(table[lexPos].name, "while", 5);
					table[lexPos++].token = whilesym;

				}
				else if (code[codePos + 1] == 'r'   &&
						 code[codePos + 2] == 'i'   &&
						 code[codePos + 3] == 't'   &&
						 code[codePos + 4] == 'e'   &&
						 !letter(code[codePos + 5]) && !digit(code[codePos + 5]))
				{
					codePos += 5;
					strncpy(table[lexPos].name, "write", 5);
					table[lexPos++].token = writesym;

				}
				else
				{
					// Possible identifier
					ident(code, &codePos, table + lexPos, &lexPos);
				}
				
			}
			// DO
			else if (code[codePos    ] == 'd'   &&
					 code[codePos + 1] == 'o'   &&
					 !letter(code[codePos + 2]) && !digit(code[codePos + 2]))
			{
				codePos += 2;
				strncpy(table[lexPos].name, "do", 2);
				table[lexPos++].token = dosym;
				
			}
			// CALL & CONST
			else if (code[codePos] == 'c') //2
			{
				if (code[codePos + 1] == 'a'   &&
					code[codePos + 2] == 'l'   &&
					code[codePos + 3] == 'l'   &&
					!letter(code[codePos + 4]) && !digit(code[codePos + 4]))
				{
					codePos += 4;
					strncpy(table[lexPos].name, "call", 4);
					table[lexPos++].token = callsym;

				}
				else if (code[codePos + 1] == 'o'   &&
						 code[codePos + 2] == 'n'   &&
						 code[codePos + 3] == 's'   &&
						 code[codePos + 4] == 't'   &&
						 !letter(code[codePos + 5]) && !digit(code[codePos + 5]))
				{
					codePos += 5;
					strncpy(table[lexPos].name, "const", 5);
					table[lexPos++].token = constsym;
				}
				
			}
			// VAR
			else if (code[codePos    ] == 'v'   &&
					 code[codePos + 1] == 'a'   &&
					 code[codePos + 2] == 'r'   &&
					 !letter(code[codePos + 3]) && !digit(code[codePos + 3]))
			{
				codePos += 3;
				strncpy(table[lexPos].name, "var", 3);
				table[lexPos++].token = varsym;
				
			}
			// PROCEDURE
			else if (code[codePos    ] == 'p'   &&
					 code[codePos + 1] == 'r'   &&
					 code[codePos + 2] == 'o'   &&
					 code[codePos + 3] == 'c'   &&
					 code[codePos + 4] == 'e'   &&
					 code[codePos + 5] == 'd'   &&
					 code[codePos + 6] == 'u'   &&
					 code[codePos + 7] == 'r'   &&
					 code[codePos + 8] == 'e'   &&
					 !letter(code[codePos + 9]) && !digit(code[codePos + 9]))
			{
				codePos += 9;
				strncpy(table[lexPos].name, "procedure", 9);
				table[lexPos++].token = procsym;
				
			}
			// READ
			else if (code[codePos    ] == 'r'   &&
					 code[codePos + 1] == 'e'   &&
					 code[codePos + 2] == 'a'   &&
					 code[codePos + 3] == 'd'   &&
					 !letter(code[codePos + 4]) && !digit(code[codePos + 4]))
			{
				codePos += 4;
				strncpy(table[lexPos].name, "read", 4);
				table[lexPos++].token = readsym;
				
			}
			// If this point is reached then the token is not a reserved word, but may still be an identifier.
			else
			{
				// void ident(char *code, int *codePos, lexeme *lex, int *lexPos)
				// int Lex(char *code, lexeme *table, int length)

				ident(code, &codePos, table + lexPos, &lexPos);
			}
		}
		// Numbers
		else if (digit(code[codePos]))
		{
			// void number(char *code, int *codePos, lexeme *lex, int *lexPos)
			number(code, &codePos, table + lexPos, &lexPos);
		}
		// Character which need to be skipped
		else if (code[codePos] == '\n' || code[codePos] == '\t' || code[codePos] == '\r' || code[codePos] == ' ')
		{
			codePos++;
			continue;
		}
		// Multiline Comments
		//	~ must be handled before '/' and '*' symbols (div / mult)
		else if (code[codePos] == '/' && code[codePos + 1] == '*')
		{
			// Adjust position for /*
			codePos += 2;

			// Loop for skipping the contents of the comment
			while(code[codePos] != '*' || code[codePos + 1] != '/')
				codePos++;

			// Adjust position for */
			codePos += 2;
			continue;
		}
		// Anything else must be a symbol (valid or otherwise)
		//	Single line comments are not implemented
		else
		{
			// PLUS
			if (code[codePos] == '+')
			{
				codePos += 1;
				strncpy(table[lexPos].name, "+", 1);
				table[lexPos++].token = plussym;
			}
			// MINUS
			else if (code[codePos] == '-')
			{
				codePos += 1;
				strncpy(table[lexPos].name, "-", 1);
				table[lexPos++].token = minussym;
			}
			// MULT
			else if (code[codePos] == '*')
			{
				codePos += 1;
				strncpy(table[lexPos].name, "*", 1);
				table[lexPos++].token = multsym;
			}
			// SLASH
			else if (code[codePos] == '/')
			{
				codePos += 1;
				strncpy(table[lexPos].name, "/", 1);
				table[lexPos++].token = slashsym;
			}
			// L_PARENTHESIS
			else if (code[codePos] == '(')
			{
				codePos += 1;
				strncpy(table[lexPos].name, "(", 1);
				table[lexPos++].token = lparentsym;
			}
			// R_PARENTHESIS
			else if (code[codePos] == ')')
			{
				codePos += 1;
				strncpy(table[lexPos].name, ")", 1);
				table[lexPos++].token = rparentsym;
			}
			// EQ
			else if (code[codePos] == '=')
			{
				codePos += 1;
				strncpy(table[lexPos].name, "=", 1);
				table[lexPos++].token = eqsym;
			}
			// COMMA
			else if (code[codePos] == ',')
			{
				codePos += 1;
				strncpy(table[lexPos].name, ",", 1);
				table[lexPos++].token = commasym;
			}
			// PERIOD
			else if (code[codePos] == '.')
			{
				codePos += 1;
				strncpy(table[lexPos].name, ".", 1);
				table[lexPos++].token = periodsym;
			}
			// NEQ & LEQ & LES
			else if (code[codePos] == '<') // 3
			{
				if (code[codePos + 1] == '>')
				{
					codePos += 2;
					strncpy(table[lexPos].name, "<>", 2);
					table[lexPos++].token = neqsym;
				}
				else if (code[codePos + 1] == '=')
				{
					codePos += 2;
					strncpy(table[lexPos].name, "<=", 2);
					table[lexPos++].token = leqsym;
				}
				else
				{
					codePos += 1;
					strncpy(table[lexPos].name, "<", 1);
					table[lexPos++].token = lessym;
				}
				
			}
			// GEQ & GTR
			else if (code[codePos] == '>')
			{
				if (code[codePos + 1] == '=')
				{
					codePos += 2;
					strncpy(table[lexPos].name, ">=", 2);
					table[lexPos++].token = geqsym;
				}
				else
				{
					codePos += 1;
					strncpy(table[lexPos].name, ">", 1);
					table[lexPos++].token = gtrsym;
				}
			}
			// SEMICOLON
			else if (code[codePos] == ';')
			{
				codePos += 1;
				strncpy(table[lexPos].name, ";", 1);
				table[lexPos++].token = semicolonsym;
				
			}
			// BECOMES
			else if (code[codePos    ] == ':' &&
					 code[codePos + 1] == '=')
			{
				codePos += 2;
				strncpy(table[lexPos].name, ":=", 2);
				table[lexPos++].token = becomessym;
				
			}
			else
			{
				fprintf(stdout, "Error: Invalid symbol.");
				codePos += 1;
				exit(0);
			}
		}
	}
	return lexPos;
}

int lexRun(int options, char *filename)
{
	FILE *input = NULL, *output = NULL;
	lexeme *lex;
	lexeme table[MAX_TOKENS];
	char *buffer;
	int length, k, lexTableIndex;

	int print = options & l_num;

	input = fopen(filename, "r");
	output = fopen("lex_out.txt", "w");

	if (input == NULL)
	{
		printf("Error: File not found.\n");
		return 0;
	}

	fseek (input, 0, SEEK_END);
	length = ftell (input);
	fseek (input, 0, SEEK_SET);

	buffer = malloc(sizeof(char) * length);

	if (buffer != NULL)
		fread(buffer, 1, length, input);
	else
		return 1;

	fclose(input);

	// Only output if requested by driver.
	if (print) {
		// Start
		fprintf(stdout, "Source Program:%s\n", filename);

		// Print Source Program
		fprintf(stdout, "%s\n", buffer);

		lexTableIndex = Lex(buffer, table, length);

		// Print lexeme table

		fprintf(stdout, "\nLexeme Table:\n");
		fprintf(stdout, "lexeme\t\ttoken type\n");

    	for(k = 0; k < lexTableIndex; k++)
    	{
    		fprintf(stdout, "%s\t\t%d\n", table[k].name, table[k].token);
    	}

    	// Print lexeme list
    	fprintf(stdout, "\nLexeme List:\n");

    	for(k = 0; k < lexTableIndex; k++)
    	{
    		lex = &table[k];
    		fprintf(stdout, "%d ", lex->token);

        	// If an identifier, print variable name
        	if(lex->token == 2)
            	fprintf(stdout, "%s ", lex->name);

        	// If number, print its ascii number value
        	else if(lex->token == 3)
            	fprintf(stdout, "%d ", lex->val);
    	}
    	fprintf(stdout, "\n");
	}

	// Output to the Parser
	for(k = 0; k < lexTableIndex; k++)
	{
		lex = &table[k];
		fprintf(output, "%d ", lex->token);

		// If an identifier, print variable name
		if(lex->token == 2)
			fprintf(output, "%s ", lex->name);

		// If number, print its ascii number value
		else if(lex->token == 3)
			fprintf(output, "%d ", lex->val);
	}

    fclose(input);
    fclose(output);
   	return 0;
}

#endif