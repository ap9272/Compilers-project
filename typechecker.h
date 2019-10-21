/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include"parser.h"

#ifndef TYPECHECKER
#define TYPECHECKER

void typeCheck(ParseTreeNode * ptNode);

ParamList* checkFuncCall(ParseTreeNode * ptNode,ParamList * l);

void checkBool(ParseTreeNode * ptNode);

void checkArith1(ParseTreeNode * ptNode,char * type);

char * checkArith2(ParseTreeNode * ptNode,char * type);

#endif