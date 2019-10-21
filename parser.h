/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include"parserDef.h"
#include"lexer.h"

#ifndef PARSER
#define PARSER

ParseTreeNode* getNextTreeNode(ParseTreeNode *ptreeNode);

ParseTreeNode*  parseInputSourceCode(char *testcaseFile, int parseTable[][no_of_terminals]);

int addToStack(char stack[][25],int sp,int ruleNo);

void createParseTable(Variables* G[], int T[][MAX_RULES]);

void printParseTree(ParseTreeNode *PT, char *outfile);

void printParseTreeNode(ParseTreeNode *ptNode, FILE *fp);

ParseTreeNode* addToTree(ParseTreeNode *ptreeNode, int ruleno, tokenlist *token);

void createAST(ParseTreeNode * ptNode);

#endif