/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include"parser.h"

#ifndef CODEGEN
#define CODEGEN

int ArithmeticExpressionGenerator(ParseTreeNode *ptNode,FILE *fp);

void AssignmentGenerator(ParseTreeNode *ptNode,FILE *fp);

void boolCodeGen(ParseTreeNode * ptNode,FILE *fp);

void codegen(ParseTreeNode *ptNode,FILE *fp);

#endif