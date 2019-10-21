/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"symbolTable.h"
#include"parser.h"

int OFFSET = 0;

int symboltable_error = 0;

int hash(char s[], int size)
{
	int sum = 0,i,j;
	for ( i = 0, j = strlen(s); i < j; ++i)
	{
		sum += (int)s[i];
	}
	return sum%size;
}

symbolTable* initializeSymbolTable(int hashsize)
{
	symbolTable *ST;
	FunctionScope *FS;
	Identifiers *IDs;
	RecordDef *RD;
	ST = (symbolTable*)malloc(sizeof(symbolTable));
	ST->hashsize = hashsize;
	ST->funcScope = (FunctionScope*)malloc(hashsize*sizeof(FunctionScope));
	FS = ST->funcScope;
	int i;
	for (i = 0; i < hashsize; ++i)
	{
		(FS+i)->ids = (Identifiers*)malloc(hashsize*sizeof(Identifiers));
		(FS+i)->recordDefs = (RecordDef*)malloc(hashsize*sizeof(RecordDef));
		IDs = (FS+i)->ids;
		RD = (FS+i)->recordDefs;		
		(FS+i)->filled = 0;
		(FS+i)->noOfInPars = (FS+i)->noOfOutPars = (FS+i)->lineInit = 0;
		(FS+i)->hashsize = hashsize;
		(FS+i)->occurance = NULL;
		(FS+i)->inputParams = NULL;
		(FS+i)->outputParams = NULL;
		int j;
		for (j = 0; j < hashsize; ++j)
		{
			(IDs+j)->filled = 0;
			(IDs+j)->offset = (IDs+j)->width = (IDs+j)->lineInit = 0;
			(IDs+j)->records = NULL;
			(IDs+j)->occurance = NULL;
		}
		for (j = 0; j < hashsize; ++j)
		{
			(RD+j)->filled = (RD+j)->totaloffset = (RD+j)->totalwidth = (RD+j)->lineno = 0;
			(RD+j)->records = NULL;
		}
	}
	return ST;
}

FunctionScope* insertFunction(symbolTable *ST, char fname[30], int lineno, int initialize)
{
	int hashval = hash(fname, ST->hashsize);
	FunctionScope *FS = (ST->funcScope)+hashval;
	int i = 0,j = 0;
	while( j < ST->hashsize)
	{
		if( FS->filled == 0 && initialize == 0)
		{
			printf("Error: function %s not defined before using at line %d.", fname, lineno);
			symboltable_error = 1;
			return NULL;
		}
		else if( FS->filled == 0 && initialize == 1)
		{
			FS->filled = 1;
			strcpy(FS->funcName,fname);
			FS->lineInit = lineno;
			return FS;
		}
		else if( !strcmp(FS->funcName, fname) && initialize == 1)
		{
			printf("Error: function %s defined on line %d has already been defined on line %d. Cannot redefine function\n", fname, lineno, FS->lineInit);
			symboltable_error = 1;
			return NULL;
		}
		else if( !strcmp(FS->funcName, fname) && initialize == 0)
		{
			NumList *head = FS->occurance;
			if(head == NULL)
			{
				head = (NumList*)malloc(sizeof(NumList));
				head->lineno = lineno;
				head->next = NULL;
				FS->occurance = head;
			}
			else
			{
				while(head->next != NULL)
				{
					head = head->next;
				}
				head->next = (NumList*)malloc(sizeof(NumList));
				head->next->lineno = lineno;
				head->next->next = NULL;
			}
			return FS;
		}
		else
		{
			i++;j++;
			if( hashval + i == ST->hashsize )
			{
				FS = ST->funcScope;
				i=0;
			}
			else
			{
				FS = (FS + i);
			}
		}
	}
	if( j == ST->hashsize)
	{
		printf("Error: Could not insert function %s.\n", fname);
		symboltable_error = 1;
	}

	return FS;
}

Identifiers* insertVariable(FunctionScope *FS, FunctionScope *globalfunc, char IDname[30], char Type[30], char Scope[30], int lineno)
{
	int hashval = hash(IDname, FS->hashsize);
	Identifiers *IDs = (FS->ids)+hashval;
	int i = 0,j = 0;
	while( j < FS->hashsize)
	{
		if( IDs->filled == 0 && !strcmp(Type, "UNKNOWN"))
		{
			if(FS == globalfunc)
			{
				return NULL;
			}
			Identifiers *IDs = insertVariable(globalfunc, globalfunc, IDname, Type, Scope, lineno);
			if( IDs == NULL)
			{
				printf("Error: variable %s not defined before using at line %d\n", IDname, lineno);
				symboltable_error = 1;
			}
			return IDs;
		}
		if( IDs->filled == 0 && strcmp(Type, "UNKNOWN"))
		{
			IDs->filled = 1;
			strcpy(IDs->IDname, IDname);
			strcpy(IDs->type, Type);
			IDs->lineInit = lineno;
			strcpy(IDs->scope,Scope);
			if(!strcmp(Type,"int"))
			{
				IDs->offset = OFFSET;
				OFFSET += 2;
				IDs->width = 2;
			}
			else if(!strcmp(Type,"real"))
			{
				IDs->offset = OFFSET;
				OFFSET += 4;
				IDs->width = 4;
			}
			else
			{
				int recordtypehash = hash(Type, globalfunc->hashsize);
				RecordDef *recorddef = (globalfunc->recordDefs)+recordtypehash;
				int i=0,j=0;
				while( j < globalfunc->hashsize)
				{
					if( recorddef->filled == 0 )
					{
						printf("Error: Type %s is unknown could not create variable %s at line %d\n", Type, IDname, lineno);
						symboltable_error = 1;
						return NULL;
					}
					else if( !strcmp(recorddef->recordID,Type))
					{
						Records *records = (Records*)malloc(sizeof(Records));
						IDs->records = records;
						IDs->offset = OFFSET;
						strcpy(records->IDname,recorddef->records->IDname);
						strcpy(records->type,recorddef->records->type);
						strcpy(records->scope,IDname);
						if(!strcmp(records->type,"int"))
						{
							records->offset = OFFSET;
							OFFSET += 2;
							records->width = 2;
						}
						else if(!strcmp(records->type,"real"))
						{
							records->offset = OFFSET;
							OFFSET += 4;
							records->width = 4;
						}
						records->lineInit = 0;	
						records->occurance = NULL;
						records->next = NULL;
						Records *varRecord = recorddef->records->next;
						while(varRecord != NULL)
						{			
							records->next = (Records*)malloc(sizeof(Records));
							records = records->next;
							strcpy(records->IDname,varRecord->IDname);
							strcpy(records->type,varRecord->type);
							strcpy(records->scope,IDname);
							if(!strcmp(records->type,"int"))
							{
								records->offset = OFFSET;
								OFFSET += 2;
								records->width = 2;
							}
							else if(!strcmp(records->type,"real"))
							{
								records->offset = OFFSET;
								OFFSET += 4;
								records->width = 4;
							}
							records->lineInit = 0;
							records->occurance = NULL;
							records->next = NULL;
							varRecord = varRecord->next;
						}
						IDs->width = recorddef->totalwidth;
						return IDs;
					}
					else
					{
						i++;j++;
						if( recordtypehash + i == globalfunc->hashsize )
						{
							recorddef = globalfunc->recordDefs;
							i=0;
						}
						else
						{
							recorddef = (recorddef + i);
						}
					}
				}
			}
			return IDs;
		}
		else if(!strcmp(IDs->IDname,IDname) && strcmp(Type, "UNKNOWN"))
		{
			printf("Error: variable %s defined on line %d has already been defined on line %d. Cannot redefine variable.\n", IDname, lineno, IDs->lineInit);
			symboltable_error = 1;
			return NULL;
		}
		else if(!strcmp(IDs->IDname,IDname) && !strcmp(Type, "UNKNOWN"))
		{
			if(strcmp(Scope,"UNKNOWN"))
			{
				char fieldid[30];
				strcpy(fieldid,Scope);
				Records *records = IDs->records;
				while(records != NULL)
				{
					if(!strcmp(records->IDname,fieldid))
					{
						if(records->lineInit == 0)
						{
							records->lineInit = lineno;
						}
						else
						{
							NumList *head = records->occurance;
							if(head == NULL)
							{
								head = (NumList*)malloc(sizeof(NumList));
								head->lineno = lineno;
								head->next = NULL;
								records->occurance = head;
							}
							else
							{
								while(head->next != NULL)
								{
									head = head->next;
								}
								head->next = (NumList*)malloc(sizeof(NumList));
								head->next->lineno = lineno;
								head->next->next = NULL;
							}
						}
						return IDs;
					}
					records = records->next;
				}
				printf("Error: No such field %s in record of type %s at line %d.\n", fieldid, IDs->type, lineno);
				symboltable_error = 1;
				return NULL;
			}
			NumList *head = IDs->occurance;
			if(head == NULL)
			{
				head = (NumList*)malloc(sizeof(NumList));
				head->lineno = lineno;
				head->next = NULL;
				IDs->occurance = head;
			}
			else
			{
				while(head->next != NULL)
				{
					head = head->next;
				}
				head->next = (NumList*)malloc(sizeof(NumList));
				head->next->lineno = lineno;
				head->next->next = NULL;
			}
			return IDs;
		}
		else
		{
			i++;j++;
			if( hashval + i == FS->hashsize )
			{
				IDs = FS->ids;
				i=0;
			}
			else
			{
				IDs = (IDs + i);
			}
		}
	}
	if( j == FS->hashsize)
	{
		printf("Error: Could not insert variable %s.\n", IDname);
		symboltable_error = 1;
	}

	return IDs;
}

RecordDef* addrecord(ParseTreeNode *typedefination, FunctionScope *globalfunc)
{
	int hashval = hash(typedefination->sibling->lexemeCurrentNode, globalfunc->hashsize);
	RecordDef *RD = (globalfunc->recordDefs)+hashval;
	char recordID[30];
	strcpy(recordID,typedefination->sibling->lexemeCurrentNode);
	int i=0, j=0;
	while( j < globalfunc->hashsize )
	{
		if(RD->filled == 0)
		{
			RD->filled = 1;
			RD->lineno = typedefination->lineno;
			strcpy(RD->recordID,recordID);
			ParseTreeNode *fielddef = typedefination->sibling->sibling->child->child;
			RD->totalwidth = RD->totaloffset = 0;
			Records *records = (Records*)malloc(sizeof(Records));
			RD->records = records;
			strcpy(records->IDname,fielddef->sibling->sibling->lexemeCurrentNode);
			strcpy(records->type,fielddef->sibling->child->lexemeCurrentNode);
			strcpy(records->scope,RD->recordID);
			if(!strcmp(records->type,"int"))
			{
				records->width = 2;
				RD->totalwidth += 2;
			}
			else if(!strcmp(records->type,"real"))
			{
				records->width = 4;
				RD->totalwidth += 4;
			}
			records->offset = records->lineInit = 0;
			records->occurance = NULL;
			records->next = NULL;
			ParseTreeNode *fielddefination = typedefination->sibling->sibling->child->sibling;
			while(fielddefination != NULL)
			{
				fielddef = fielddefination->child;
				records->next = (Records*)malloc(sizeof(Records));
				records = records->next;
				strcpy(records->IDname,fielddef->sibling->sibling->lexemeCurrentNode);
				strcpy(records->type,fielddef->sibling->child->lexemeCurrentNode);
				strcpy(records->scope,RD->recordID);
				if(!strcmp(records->type,"int"))
				{
					records->width = 2;
					RD->totalwidth += 2;
				}
				else if(!strcmp(records->type,"real"))
				{
					records->width = 4;
					RD->totalwidth += 4;
				}
				records->offset = records->lineInit = 0;
				records->occurance = NULL;
				records->next = NULL;
				fielddefination = fielddefination->sibling->child;
			}
			return RD;
		}
		else if(!strcmp(RD->recordID,recordID))
		{
			printf("Error: RecordType %s defined at line %d is already defined at line %d.\n", recordID, typedefination->lineno, RD->lineno);
			symboltable_error = 1;
			return NULL;
		}
		else
		{
			i++;j++;
			if( hashval + i == globalfunc->hashsize)
			{
				RD = globalfunc->recordDefs;
				i=0;
			}
			else
			{
				RD = (RD+i);
			}
		}
	}
	if( j == globalfunc->hashsize)
	{
		printf("Error: Could not insert record type %s.\n", recordID);
		symboltable_error = 1;
	}
	return RD;
}

void findTK(ParseTreeNode *AST, FunctionScope *FS, FunctionScope *globalfunc, symbolTable *ST)
{
	while(AST != NULL)
	{
		if(AST->child != NULL)
		{
			findTK(AST->child, FS, globalfunc, ST);
		}
		if(!strcmp(AST->token,"TK_ID"))
		{
			if(AST->sibling != NULL && !strcmp(AST->sibling->NodeSymbol,"<new_24>") && AST->sibling->child != NULL)
			{
				AST->identifierPtr = insertVariable(FS, globalfunc, AST->lexemeCurrentNode, "UNKNOWN", AST->sibling->child->lexemeCurrentNode, AST->lineno);
			}	
			else
			{
				AST->identifierPtr = insertVariable(FS, globalfunc, AST->lexemeCurrentNode, "UNKNOWN", "UNKNOWN", AST->lineno);
			}
		}
		else if(!strcmp(AST->token,"TK_FUNID"))
		{
			if(!strcmp(FS->funcName,AST->token))
			{
				printf("Error: Recursion at line %d. recursion not allowed\n", AST->lineno);
				symboltable_error = 1;
			}
			else
			{
				AST->functionPtr = insertFunction(ST, AST->lexemeCurrentNode, AST->lineno,0);
			}
		}
		AST = AST->sibling;
	}
}

void makeFunctionScope(ParseTreeNode *AST, symbolTable *ST, FunctionScope *globalfunc)
{
	ParseTreeNode *ptNode = AST;
	char Scope[30];
	ptNode = ptNode->child;
	while (ptNode != NULL)
	{
		if(!strcmp(ptNode->NodeSymbol,"<otherFunctions>"))
		{
			ParseTreeNode *functions = ptNode->child;
			while( functions != NULL)
			{
				if( !strcmp(functions->NodeSymbol,"<otherFunctions>"))
				{
					functions = functions->child;
					if(functions == NULL)
					{
						break;
					}
				}
				if( !strcmp(functions->NodeSymbol,"<function>") )
				{
					strcpy(Scope, functions->child->lexemeCurrentNode);
					int lineno = functions->child->lineno;
					FunctionScope *FS = insertFunction(ST, Scope, lineno, 1);
					functions->child->functionPtr = FS;
					OFFSET = 0;
					ParseTreeNode *funcChildren = functions->child;
					while( strcmp(funcChildren->NodeSymbol,"<input_par>"))
					{
						funcChildren = funcChildren->sibling;
					}
					ParseTreeNode *input_par = funcChildren->child;
					ParamList *Iparams;
					if(input_par != NULL)
					{
						Iparams = (ParamList*)malloc(sizeof(ParamList));
						Iparams->next = NULL;
						FS->inputParams = Iparams;
					}
					while( input_par != NULL)
					{
						ParseTreeNode *data_type = input_par->child;
						if( !strcmp(data_type->token, "TK_RECORD"))
						{
							input_par->sibling->identifierPtr = insertVariable(FS, globalfunc, input_par->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, FS->funcName, input_par->sibling->lineno);
							strcpy(Iparams->type,data_type->sibling->lexemeCurrentNode);
						}
						else
						{	
							input_par->sibling->identifierPtr = insertVariable(FS, globalfunc, input_par->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, FS->funcName, input_par->sibling->lineno);
							strcpy(Iparams->type,data_type->lexemeCurrentNode);
						}
						FS->noOfInPars++;
						input_par = input_par->sibling->sibling->child;
						if(input_par != NULL)
						{
							Iparams->next = (ParamList*)malloc(sizeof(ParamList));
							Iparams = Iparams->next;
							Iparams->next = NULL;
						}
					}
					while( strcmp(funcChildren->NodeSymbol,"<output_par>"))
					{
						funcChildren = funcChildren->sibling;
					}
					if( funcChildren->child != NULL)
					{
						ParseTreeNode *output_par = funcChildren->child->sibling;
						ParamList *Oparams;
						Oparams = (ParamList*)malloc(sizeof(ParamList));
						FS->outputParams = Oparams;
						ParseTreeNode *data_type = output_par->child->child;
						if( !strcmp(data_type->token, "TK_RECORD"))
						{
							output_par->child->sibling->identifierPtr = insertVariable(FS, globalfunc, output_par->child->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, FS->funcName, output_par->child->sibling->lineno);
							strcpy(Oparams->type,data_type->sibling->lexemeCurrentNode);
						}
						else
						{
							output_par->child->sibling->identifierPtr = insertVariable(FS, globalfunc, output_par->child->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, FS->funcName, output_par->child->sibling->lineno);
							strcpy(Oparams->type,data_type->lexemeCurrentNode);
						}
						FS->noOfOutPars++;
						Oparams->next = NULL;
						output_par = output_par->child->sibling->sibling->child;
						while( output_par != NULL)
						{
							Oparams->next = (ParamList*)malloc(sizeof(ParamList));
							Oparams = Oparams->next;
							ParseTreeNode *data_type = output_par->child;
							if( !strcmp(data_type->token, "TK_RECORD"))
							{
								output_par->sibling->identifierPtr = insertVariable(FS, globalfunc, output_par->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, FS->funcName, output_par->sibling->lineno);
								strcpy(Oparams->type,data_type->sibling->lexemeCurrentNode);
							}
							else
							{
								output_par->sibling->identifierPtr = insertVariable(FS, globalfunc, output_par->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, FS->funcName, output_par->sibling->lineno);
								strcpy(Oparams->type,data_type->lexemeCurrentNode);
							}
							FS->noOfOutPars++;
							output_par = output_par->sibling->child;
							Oparams = Oparams->next;
						}
					}
					while( strcmp(funcChildren->NodeSymbol,"<stmts>"))
					{
						funcChildren = funcChildren->sibling;
					}
					ParseTreeNode *stmts = funcChildren->child;
					stmts = stmts->sibling;
					if( stmts->child != NULL)
					{
						ParseTreeNode *declarations = stmts->child;
						while(declarations != NULL)
						{
							ParseTreeNode *declaration = declarations->child;
							ParseTreeNode *data_type = declaration->sibling->child;
							if( !strcmp(data_type->token, "TK_RECORD"))
							{
								if(declaration->sibling->sibling->child == NULL)
								{
									declaration->sibling->identifierPtr = insertVariable(FS, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, FS->funcName, declaration->sibling->sibling->lineno);
								}
								else
								{
									declaration->sibling->identifierPtr = insertVariable(globalfunc, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, "global", declaration->sibling->sibling->lineno);
								}					
							}
							else
							{
								if(declaration->sibling->sibling->child == NULL)
								{
									declaration->sibling->identifierPtr = insertVariable(FS, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, FS->funcName, declaration->sibling->sibling->lineno);
								}
								else
								{
									declaration->sibling->identifierPtr = insertVariable(globalfunc, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, "global", declaration->sibling->sibling->lineno);
								}
							}
							declarations = declarations->sibling->child;
						}
					}
					stmts = stmts->sibling;
					while( stmts != NULL)
					{
						findTK(stmts->child, FS, globalfunc, ST);
						stmts = stmts->sibling;
					}
				}
				functions = functions->sibling;
			}
		}
		else if (!strcmp(ptNode->NodeSymbol,"<mainFunction>"))
		{
			ParseTreeNode *functions = ptNode;
			strcpy(Scope, functions->child->lexemeCurrentNode);
			int lineno = functions->child->lineno;
			FunctionScope *FS = insertFunction(ST, Scope, lineno, 1);
			OFFSET = 0;
			functions->child->functionPtr = FS;
			ParseTreeNode *funcChildren = ptNode->child;
			while( strcmp(funcChildren->NodeSymbol,"<stmts>"))
			{
				funcChildren = funcChildren->sibling;
			}
			ParseTreeNode *stmts = funcChildren->child;
			stmts = stmts->sibling;
			if( stmts->child != NULL)
			{
				ParseTreeNode *declarations = stmts->child;
				while(declarations != NULL)
				{
					ParseTreeNode *declaration = declarations->child;
					ParseTreeNode *data_type = declaration->sibling->child;
					if( !strcmp(data_type->token, "TK_RECORD"))
					{
						if(declaration->sibling->sibling->child == NULL)
						{
							declaration->sibling->sibling->identifierPtr = insertVariable(FS, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, FS->funcName, declaration->sibling->sibling->lineno);
						}
						else
						{
							declaration->sibling->sibling->identifierPtr = insertVariable(FS, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->sibling->lexemeCurrentNode, "global", declaration->sibling->sibling->lineno);	
						}	
					}
					else
					{
						if(declaration->sibling->sibling->child == NULL)
						{
							declaration->sibling->sibling->identifierPtr = insertVariable(FS, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, FS->funcName, declaration->sibling->sibling->lineno);
						}
						else
						{
							declaration->sibling->sibling->identifierPtr = insertVariable(globalfunc, globalfunc, declaration->sibling->sibling->lexemeCurrentNode, data_type->lexemeCurrentNode, "global", declaration->sibling->sibling->lineno);
						}
					}
					declarations = declarations->sibling->child;
				}
			}
			stmts = stmts->sibling;
			while( stmts != NULL)
			{
				findTK(stmts->child, FS, globalfunc, ST);
				stmts = stmts->sibling;
			}
		}
		ptNode = ptNode->sibling;
	}
}

void findDataStructures(ParseTreeNode *AST, symbolTable *ST, FunctionScope *globalfunc)
{
	ParseTreeNode *ptNode = AST;
	ptNode = ptNode->child;
	while (ptNode != NULL)
	{
		if(!strcmp(ptNode->NodeSymbol,"<otherFunctions>"))
		{
			ParseTreeNode *functions = ptNode->child;
			while( functions != NULL)
			{
				if( !strcmp(functions->NodeSymbol,"<otherFunctions>"))
				{
					functions = functions->child;
					if(functions == NULL)
					{
						break;
					}
				}
				if( !strcmp(functions->NodeSymbol,"<function>") )
				{
					ParseTreeNode *funcChildren = functions->child;
					while( strcmp(funcChildren->NodeSymbol,"<stmts>"))
					{
						funcChildren = funcChildren->sibling;
					}
					ParseTreeNode *stmts = funcChildren->child;
					if( stmts->child != NULL)
					{
						ParseTreeNode *typedefination = stmts->child->child;
						while(typedefination != NULL)
						{
							typedefination->sibling->recordDefPtr = addrecord(typedefination, globalfunc);
							typedefination = typedefination->sibling->child;
						}
					}
				}
				functions = functions->sibling;
			}
		}
		else if (!strcmp(ptNode->NodeSymbol,"<mainFunction>"))
		{
			ParseTreeNode *funcChildren = ptNode->child;
			while( strcmp(funcChildren->NodeSymbol,"<stmts>"))
			{
				funcChildren = funcChildren->sibling;
			}
			ParseTreeNode *stmts = funcChildren;
			if( stmts->child != NULL)
			{
				ParseTreeNode *typedefination = stmts->child->child;

				while(typedefination != NULL)
				{
					typedefination->child->sibling->recordDefPtr = addrecord(typedefination->child, globalfunc);
					typedefination = typedefination->sibling->child;
				}
			}
		}
		ptNode = ptNode->sibling;
	}
}


symbolTable* fillSymbolTable(ParseTreeNode* AST, int hashsize)
{
	symbolTable *ST;
	FunctionScope *globalfunc;
	ST = initializeSymbolTable(hashsize);
	OFFSET = 0;
	globalfunc = insertFunction(ST, "global",0, 1);
	findDataStructures(AST, ST, globalfunc);
	makeFunctionScope(AST, ST, globalfunc);
	return ST;
}

void printSymbolTable(symbolTable *ST)
{
	int i,j;
	FunctionScope *FS = ST->funcScope;
	printf("%-30s %-30s %-30s %s\n","Lexeme","type","scope","offset");
	for( i = 0; i < ST->hashsize; ++i)
	{
		if( (FS + i)->filled == 1)
		{
			Identifiers *IDs = (FS+i)->ids;
			for (j = 0; j < (FS+i)->hashsize; ++j)
			{
				if( (IDs+j)->filled == 1)
				{
					printf("%-30s ", (IDs+j)->IDname);
					if((IDs+j)->records != NULL)
					{
						Records *records = (IDs+j)->records;
						int i,no_of_chars = 0;
						while(records != NULL)
						{
							printf("%s ",records->type);
							no_of_chars += strlen(records->type) + 1;
							records = records->next;
						}
						for( i=0; i<(31-no_of_chars); ++i)
						{
							printf(" ");
						}
					}
					else
					{
						printf("%-30s ", (IDs+j)->type);
					}
					printf("%-30s %d\n", (IDs+j)->scope, (IDs+j)->offset);
				}
			}
		}
	}
}