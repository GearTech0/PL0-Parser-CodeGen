#ifndef SCANNER_H
#define SCANNER_H

typedef struct inputStream 
{
	int pos;
	char * input;
	int (*isEndOfStream)(struct inputStream *);
	char (*next)(struct inputStream *);
	char (*peek)(const struct inputStream *);
	void (*resetStream)(struct inputStream *);
} inputStream;

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

char next(inputStream * stream);
char peek(const inputStream * stream);
int isEndOfStream(inputStream * stream);
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

char next(inputStream * stream)
{
	char retChar = stream->input[(stream->pos)++];
	if(retChar != '\n')
		return retChar;
	return '\0';
}

char peek(const inputStream * stream)
{
	return stream->input[stream->pos];
}

int isEndOfStream(inputStream * stream)
{
	return stream->pos < strlen(stream->input);
}

void resetStream(inputStream * stream)
{
	stream->pos = 0;
}

inputStream * createInputStream()
{
	inputStream * stream = (inputStream *)malloc(sizeof(inputStream));
	stream->pos = 0;
	stream->input = (char *)malloc(1024*sizeof(char));
	stream->next = next;
	stream->isEndOfStream = isEndOfStream;
	stream->peek = peek; 
	stream->resetStream = resetStream;
	return stream;
}

#endif