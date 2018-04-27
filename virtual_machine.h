/**
	COP3402 - Spring 2018

	System Software Assignment 3
	PL/0 Parser/Code Generator

	Authors - Daquaris Chadwick, Marcus Sooter
*/

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <stdlib.h>
#include <stdio.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_LEXI_LEVELS 3

int BP = 1;
int SP = 0;
int PC = 0;
int halt = 0;
int IR = 0;
int stack[MAX_STACK_HEIGHT];
int reg[8];
int LEX = 0;

typedef struct instruction {
    int op; // Op Code
    int r;  // reg
    int l;  // L
    int m;  // M
    int size;
} instruction;

instruction ** readInstructions(char * fname);
void printStack(int sp, int bp, int* stack, int lex);
void vmRun(int options, char * filename);
char * opToString(int op);
int fetchInstruction(instruction * inst, int reg[16], int * PC, int * BP, int * SP);
int base(int l, int base);

void vmRun(int options, char * filename)
{
	int i = 0, j = 0;
    int * reg = (int *)malloc(16 * sizeof(int));
	int print = options & v_num;

    // Set Up registers with zeroes
    for(i = 0; i < MAX_STACK_HEIGHT; i++) { stack[i] = 0; }
    for(i = 0; i < 8; i++) { reg[i] = 0; }

    instruction ** code = readInstructions(filename);
	
	if(print)
		printf("OP   Rg Lx Vl[ PC BP SP] \n");

    for(i = 0; i < code[0]->size;)
    {
        halt = fetchInstruction(code[i], reg, &PC, &BP, &SP);

		if(print)
		{
			// Print code info
			printf("%d\t%-4s%3d%3d%3d[%3d%3d%3d] ", i, opToString(code[i]->op), code[i]->r, code[i]->l, code[i]->m, PC, BP, SP);
			printStack(SP, BP, stack, LEX);
			printf("\n");

			// Print Registers
			printf("Registers:[");
			for(j = 0; j < 8; j++)
			{
				printf("%3d", reg[j]);
			}
			printf("]\n");
		}

        if(halt == -1) break;
        i = PC;
    }
}

instruction ** readInstructions(char * fname)
{
    FILE * f = fopen(fname , "r");
    int * codeArray = (int *) malloc(MAX_CODE_LENGTH * sizeof(int));
    instruction ** code = (instruction **)malloc(sizeof(instruction*));

    int i = 0, j = 0, k = 0;
    int rows = 0;
    if(f)
    {
        while(fscanf(f, "%d", &codeArray[i]) != EOF)
        {
            i += 1;
        }
    }

    // Create an array of instructions
    rows = i/4;
    code = (instruction **) realloc(code, rows * sizeof(instruction*));
    for(j = 0; j < rows; j++)
    {
        code[j] = (instruction*)malloc(sizeof(instruction));
    }
    for(j = 0; j < i; j+=4)
    {
        code[k]->op = codeArray[j];
        code[k]->r = codeArray[j+1];
        code[k]->l = codeArray[j+2];
        code[k]->m = codeArray[j+3];
        code[k]->size = rows;
        k++;
    }

    return code;

    fclose(f);
    free(codeArray);
}

int base(int l, int base) // l stand for L in the instruction format
{  int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}

char * opToString(int op)
{
    switch(op)
    {
        case 1:
            return "LIT";
        break;
        case 2:
            return "RTN";
        break;
        case 3:
            return "LOD";
        break;
        case 4:
            return "STO";
        break;
        case 5:
            return "CAL";
        break;
        case 6:
            return "INC";
        break;
        case 7:
            return "JMP";
        break;
        case 8:
            return "JPC";
        break;
        case 9:
            return "SIO";
        break;
        case 10:
            return "NEG";
        break;
        case 11:
            return "ADD";
        break;
        case 12:
            return "SUB";
        break;
        case 13:
            return "MUL";
        break;
        case 14:
            return "DIV";
        break;
        case 15:
            return "ODD";
        break;
        case 16:
            return "MOD";
        break;
        case 17:
            return "EQL";
        break;
        case 18:
            return "NEQ";
        break;
        case 19:
            return "LSS";
        break;
        case 20:
            return "LEQ";
        break;
        case 21:
            return "GTR";
        break;
        case 22:
            return "GEQ";
        break;
    }
}

int fetchInstruction(instruction * inst, int reg[8], int * PC, int * BP, int * SP)
{
    *PC += 1;
    switch(inst->op)
    {
        // LIT
        case 1:
            reg[inst->r] = inst->m;
        break;

        // RTN
        case 2:
            *SP = *BP -1;
            *BP = stack[*SP + 3];
            *PC = stack[*SP + 4];
            LEX -= 1;
        break;

        // LOD
        case 3:
            reg[inst->r] = stack[base(inst->l, *BP) + inst->m - 1];
        break;

        // STO
        case 4:
            stack[base(inst->l, *BP) + inst->m - 1] = reg[inst->r];
        break;

        // CAL
        case 5:
            stack[*SP + 1] = 0;
            stack[*SP + 2] = base(inst->l, *BP);
            stack[*SP + 3] = *BP;
            stack[*SP + 4] = *PC;

            *BP = *SP + 1;
            *SP = *SP + 4;
            *PC = inst->m;
            LEX += 1;
            return 2;
            // Increment Lex

        break;

        // INC
        case 6:
            *SP += inst->m;
        break;

        // JMP
        case 7:
            *PC = inst->m;
        break;

        // JPC
        case 8:
            if(!reg[inst->r])
                *PC = inst->m;
        break;

        // SIO
        case 9:
            switch(inst->m)
            {
                case 1:
                    printf("%d\n", reg[inst->r]);
                break;

                case 2:
                    scanf("%d", &reg[inst->r]);
                break;

                case 3:
                    return -1;
                break;
            }
        break;

        // NEG
        case 10:
            reg[inst->r] = -reg[inst->m];
        break;

        // ADD
        case 11:
            reg[inst->r] = reg[inst->l] + reg[inst->m];
        break;

        // SUB
        case 12:
            reg[inst->r] = reg[inst->l] - reg[inst->m];
        break;

        // MUL
        case 13:
            reg[inst->r] = reg[inst->l] * reg[inst->m];
        break;

        // DIV
        case 14:
            reg[inst->r] = reg[inst->l] / reg[inst->m];
        break;

        // ODD
        case 15:
            reg[inst->r] = reg[inst->r] % 2;
        break;

        // MOD
        case 16:
            reg[inst->r] = reg[inst->l] % reg[inst->m];
        break;

        // EQL
        case 17:
            reg[inst->r] = reg[inst->l] == reg[inst->m];
        break;

        // NEQ
        case 18:
            reg[inst->r] = reg[inst->l] != reg[inst->m];
        break;

        // LSS
        case 19:
            reg[inst->r] = reg[inst->l] < reg[inst->m];
        break;

        // LEQ
        case 20:
            reg[inst->r] = reg[inst->l] <= reg[inst->m];
        break;

        // GTR
        case 21:
            reg[inst->r] = reg[inst->l] > reg[inst->m];
        break;

        // GEQ
        case 22:
            reg[inst->r] = reg[inst->l] >= reg[inst->m];
        break;
    }

    return 0;
}

void printStack(int sp, int bp, int* stack, int lex){
     int i;
     if (bp == 1) {
     	if (lex > 0) {
	   printf("|");
	   }
     }
     else {
     	  //Print the lesser lexical level
     	  printStack(bp-1, stack[bp + 2], stack, lex-1);
	  printf("|");
     }
     //Print the stack contents - at the current level
     for (i = bp; i <= sp; i++) {
     	 printf("%3d ", stack[i]);
     }
}

#endif