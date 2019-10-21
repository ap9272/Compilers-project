/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include"symbolTableDef.h"

#ifndef PARSERDEF
#define PARSERDEF
#define MAX_RULES 100

typedef struct Variables
{
	char value[30];
	struct Variables *next;
} Variables;

Variables *rules[MAX_RULES], *terminals, *non_terminals, *follow_set[MAX_RULES];
int no_of_rules ,begin, no_of_terminals,no_of_non_terminals;

typedef struct ParseTreeNode
{
	struct ParseTreeNode *parent;
	struct ParseTreeNode *child;
	struct ParseTreeNode *sibling;
	char lexemeCurrentNode[30];
	int lineno;
	char token[30];
	char valuelfNumber[30];
	char parentNodeSymbol[30];
	int isLeafNode;
	char NodeSymbol[30];
	FunctionScope *functionPtr;
	Identifiers *identifierPtr;
	RecordDef *recordDefPtr;
} ParseTreeNode;

#endif