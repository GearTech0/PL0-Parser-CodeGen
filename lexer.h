#ifndef LEXER_H
#define LEXER_H

typedef enum { 
	nulsym = 1, identsym, numbersym, plussym, 
	minussym,multsym,  slashsym, oddsym, eqsym, 
	neqsym, lessym, leqsym,gtrsym, geqsym, lparentsym, 
	rparentsym, commasym, semicolonsym,periodsym, becomessym,
	beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, 
	constsym, varsym, procsym, writesym,readsym , elsesym

} token_type;

typedef struct token
{
	
} token;



#endif