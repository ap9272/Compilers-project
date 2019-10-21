/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include"symbolTableDef.h"
#include"parser.h"

#ifndef SYMBOLTABLE
#define SYMBOLTABLE

int hash(char s[], int size);

symbolTable* initializeSymbolTable(int hashsize);

FunctionScope* insertFunction(symbolTable *ST, char fname[30], int lineno, int initialize);

Identifiers* insertVariable(FunctionScope *FS, FunctionScope *globalfunc, char IDname[30], char Type[30], char Scope[30], int lineno);

RecordDef* addrecord(ParseTreeNode *typedefination, FunctionScope *globalfunc);

void findTK(ParseTreeNode *AST, FunctionScope *FS, FunctionScope *globalfunc, symbolTable *ST);

void makeFunctionScope(ParseTreeNode *AST, symbolTable *ST, FunctionScope *globalfunc);

void findDataStructures(ParseTreeNode *AST, symbolTable *ST, FunctionScope *globalfunc);

symbolTable* fillSymbolTable(ParseTreeNode* AST, int hashsize);

void printSymbolTable(symbolTable *ST);

#endif