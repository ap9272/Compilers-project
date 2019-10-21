/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#ifndef SYMBOLTABLEDEF
#define SYMBOLTABLEDEF

typedef struct NumList
{
	int lineno;
	struct NumList *next;
} NumList;

typedef struct ParamList
{
	char type[30];
	struct ParamList *next;
} ParamList;

typedef struct Records
{
	char IDname[30];
	char type[30];
	int offset;
	int width;
	int lineInit;
	NumList *occurance;
	char scope[30];
	struct Records *next;
} Records;

typedef struct RecordDef
{
	int filled;
	Records *records;
	int lineno;
	char recordID[30];
	int totaloffset;
	int totalwidth;
} RecordDef;

typedef struct Identifiers
{
	int filled;
	char IDname[30];
	//IDRecord IDinfo;
	char type[30];
	int lineInit;
	NumList *occurance;
	int offset;
	int width;
	char scope[30];
	Records *records;
} Identifiers;

typedef struct FunctionScope
{
	int filled;
	char funcName[30];
	int lineInit;
	NumList *occurance;
	int noOfInPars;
	int noOfOutPars;
	Identifiers *ids;
	RecordDef *recordDefs;
	ParamList *inputParams;
	ParamList *outputParams;
	int hashsize;
} FunctionScope;

typedef struct symbolTable
{
	FunctionScope *funcScope;
	int hashsize;
} symbolTable;

#endif