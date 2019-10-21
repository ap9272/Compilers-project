/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include"lexerDef.h"

#ifndef LEXER
#define LEXER

int getStream(FILE *fp, char * block, int k);

tokenlist* getNextToken(char * block,tokenlist * tl);

//tokenlist* getNextToken(tokenlist *token);

#endif