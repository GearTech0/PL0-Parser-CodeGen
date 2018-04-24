#ifndef SCANNER_H
#define SCANNER_H

#include <ctype.h>
#include "lexer.h"

typedef struct inputStream 
{
	int pos;
	int line;
	int col;
	char * input;
	char * toClose;
	int (*isEndOfStream)(struct inputStream *);
	char (*next)(struct inputStream *);
	char (*peek)(const struct inputStream *);
	void (*resetStream)(struct inputStream *);
} inputStream;

char next(inputStream * stream);
char peek(const inputStream * stream);
int isEndOfStream(inputStream * stream);
void readNext(inputStream * stream, token * tokenList);
void resetStream(inputStream * stream);
inputStream * createInputStream();

token * readSlash(inputStream * stream);
token * readNumber(inputStream * stream);
token * readIdent(inputStream * stream);
token * readPeriod(inputStream * stream);
token * readSpecial(inputStream * stream);

void scanFile(char * filename)
{
	FILE * fp;
	fp = fopen(filename, "r");
	if(!fp)
	{
		printf("ERROR: Error reading file \"%s\"", filename);
		exit(-1);
	}
	
	inputStream * stream = createInputStream();
	token * tokenList = (token *) malloc(sizeof(token));
	size_t len = 0;
	
	// Scan file to begin parsing
	fscanf(fp, "%[^\0]", stream->input);
	printf("\ninput: \n%s\n", stream->input); // Debug
	strcat(stream->input, "\0");
	
	while(!stream->isEndOfStream(stream))
	{
		readNext(stream, tokenList);
	}
	
	fclose(fp);
	free(stream);
	free(tokenList);
}

void readNext(inputStream * stream, token * tokenList)
{
	//printf("%c", stream->next(stream)); Debug
	char ch = stream->peek(stream);
	printf("char %d\n", ch);
	if(ch == '\0')
	{
		printf("end read");
		return NULL;
	}
	else if(ch == ' ')	// Change this into a throwing loop that throws whitespace
	{
		stream->next(stream);
		readNext(stream);
	}
	else if(ch == '/')
	{
		readSlash(stream);
	}
	else if(isdigit(ch))
	{
		readNumber(stream);
	}
	else if(isalpha(ch))
	{
		readIdent(stream);
	}
	else if(ch == '.')
	{
		readPeriod(stream);
	}
	else 
	{
		readSpecial(stream);
	}
}

token * readSlash(inputStream * stream)
{
	char ch = stream->next(stream);
	//printf("char %d\n", ch);
	if(stream->peek(stream) == '/')
	{
		printf("Skipping1...\n");
		while(ch != '\n')
		{
			ch = stream->next(stream);
		}
		
		return null;
	}
	
	else if(stream->peek(stream) == '*')
	{
		ch = stream->next(stream);
		ch = stream->next(stream);
		while(ch != '*')
		{
			printf("Skipping2...\n");
			if(stream->peek(stream) == '*')
			{
				stream->next(stream);
				if(stream->peek(stream) == '/')
					return null;
			}
			if(stream->peek(stream) == '\0')
			{
				// Does not end
				return null;
			}
			ch = stream->next(stream);
		}
	}
}

token * readNumber(inputStream * stream)
{
}

token * readIdent(inputStream * stream)
{
}

token * readPeriod(inputStream * stream)
{
}

token * readSpecial(inputStream * stream)
{
}

char next(inputStream * stream)
{
	char retChar = stream->input[(stream->pos)++];
	if(retChar != '\n')
	{
		(stream->col)++;
		return retChar;
	}
	stream->col = 0;
	(stream->line)++;
	printf("returning: %d\n", retChar);
	return retChar;
}

char peek(const inputStream * stream)
{
	char retChar = stream->input[stream->pos];
	printf("peeking: %c\n", retChar);
	return retChar;
}

int isEndOfStream(inputStream * stream)
{
	return stream->pos >= strlen(stream->input);
}

void resetStream(inputStream * stream)
{
	stream->pos = 0;
}

inputStream * createInputStream()
{
	inputStream * stream = (inputStream *)malloc(sizeof(inputStream));
	stream->pos = 0;
	stream->line = 1;
	stream->col = 0;
	stream->input = (char *)malloc(1024*sizeof(char));
	stream->toClose = (char *)malloc(1024*sizeof(char));
	
	// Functions
	stream->next = next;
	stream->isEndOfStream = isEndOfStream;
	stream->peek = peek; 
	stream->resetStream = resetStream;
	return stream;
}

#endif