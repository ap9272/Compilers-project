/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include"parser.h"

#ifndef SEMANTICANALYSIS
#define SEMANTICANALYSIS

void Semantic5(ParseTreeNode *ptNode, char idName[30]);

void findVar(ParseTreeNode *ptNode, char idName[30]);

void Semantic15(ParseTreeNode *ptNode, ParseTreeNode *stmts);

void semanticAnalysis(ParseTreeNode *ptNode);

#endif