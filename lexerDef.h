/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>

#ifndef LEXERDEF
#define LEXERDEF

struct token{
	char tokenName[20] ;
	char lexeme[40];
};


typedef struct tokenlist{
	struct token t;
	int lineno;
	struct tokenlist * next;
}tokenlist;

int counter,bytesread;
extern char keywrds[25][25]; 
extern char tk_keywrds[25][25];

char * buff, * block;
FILE * fp;

#endif