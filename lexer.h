#ifndef LEXER_H
#define LEXER_H

typedef enum { 
	nulsym = 1, identsym, numbersym, plussym, 
	minussym, multsym,  slashsym, oddsym, eqsym, 
	neqsym, lessym, leqsym,gtrsym, geqsym, lparentsym, 
	rparentsym, commasym, semicolonsym, periodsym, becomessym,
	beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, 
	constsym, varsym, procsym, writesym,readsym , elsesym

} token_type;

typedef struct token
{
	int id;
	int val;
	char * name;
	struct token * next;
} token;

void printTokenTable(token * head)
{
	token * temp = head;
	
	printf("\nLexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	while(temp)
	{
		if(temp->id == 3)
			printf("%d\t\t%d\n", temp->val, temp->id);
		else
			printf("%s\t\t%d\n", temp->name, temp->id);
		temp = temp->next;
	}
	return;
}

void printTokenList(token * head)
{
	token * temp = head;
	
	printf("\nLexeme List:\n");
	while(temp)
	{
		printf("%d ", temp->id);
		if(temp->id == 3)
			printf("%d ", temp->name);
		else if(temp->id == 2)
			printf("%s ", temp->val);
		temp = temp->next;
	}
}

void addToken(token * head, token * toAdd)
{
	token * temp = head;
	
	if(!toAdd) return;
	
	while(temp->next)
	{
		temp = temp->next;
	}
	
	temp->next = toAdd;
}

#endif