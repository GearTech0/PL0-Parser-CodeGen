#ifndef SCANNER_H
#define SCANNER_H

#define MAX_IDENTIFIERS 11
#define MAX_DIGITS 5

enum tokens 
{
	nulsym  =  1,  identsym  =  2,  numbersym  =  3,  plussym  =  4,  
	minussym  =  5,  multsym  =  6,  slashsym= 7, oddsym = 8,  
	eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym  =  13,  
	geqsym  =  14,  lparentsym  =  15,  rparentsym  =  16,  commasym  =  17, 
	semicolonsym  =  18,  periodsym  =  19,  becomessym  =  20,  beginsym  =  21,  
	endsym  =  22, ifsym  =  23,  thensym  =  24,  whilesym  =  25,  dosym  =  26, 
	callsym  =  27,  constsym  =  28, varsym = 29, procsym = 30, writesym = 31, 
	readsym = 32, elsesym = 33
};



#endif