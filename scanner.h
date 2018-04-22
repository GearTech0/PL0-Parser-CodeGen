#ifndef SCANNER_H
#define SCANNER_H

#include "lexer.h"

typedef struct inputStream 
{
	int pos;
	int line;
	int col;
	char * input;
	int (*isEndOfStream)(struct inputStream *);
	char (*next)(struct inputStream *);
	char (*peek)(const struct inputStream *);
	void (*resetStream)(struct inputStream *);
} inputStream;

char next(inputStream * stream);
char peek(const inputStream * stream);
int isEndOfStream(inputStream * stream);
int readNext(inputStream * stream);
void resetStream(inputStream * stream);
inputStream * createInputStream();

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
	
	// Scan file to begin parsing
	while(fscanf(fp, "%s", stream->input) != EOF)
	{
		while(stream->isEndOfStream(stream))
		{
			//printf("%c", stream->next(stream)); Debug
		}
		
		// Reset position marker for stream reader
		stream->resetStream(stream);
	}
	
	fclose(fp);
}

int readNext(inputStream * stream)
{
	if(stream->isEndOfStream(stream))
		return -1;
	char ch = stream->peek(stream);
	if(ch == "/")
	{
		return checkComment(ch);
	}
	if(isdigit(ch))
	{
		
	}
	if(isAlpha(ch))
	{
		return check
	}
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
	return '\0';
}

char peek(const inputStream * stream)
{
	return stream->input[stream->pos];
}

int isEndOfStream(inputStream * stream)
{
	return stream->pos => strlen(stream->input);
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
	stream->next = next;
	stream->isEndOfStream = isEndOfStream;
	stream->peek = peek; 
	stream->resetStream = resetStream;
	return stream;
}

#endif