/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"parser.h"
#include"symbolTable.h"


int VarChanged = 0;

int semanticanalysis_error = 0;

void Semantic5(ParseTreeNode *ptNode, char idName[30])
{
	if(ptNode == NULL || VarChanged == 1)
	{
		return;
	}
	else if(!strcmp(ptNode->token,"TK_READ"))
	{
		if(!strcmp(ptNode->sibling->child->lexemeCurrentNode,idName))
		{
			VarChanged = 1;
		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<assignmentStmt>"))
	{
		if(!strcmp(ptNode->child->child->lexemeCurrentNode,idName))
		{
			VarChanged = 1;
		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<outputParameters>"))
	{
		if(ptNode->child != NULL)
		{
			ParseTreeNode *ids = ptNode->child->child;

			while(ids != NULL)
			{
				if(!strcmp(ids->lexemeCurrentNode,idName))
				{
					VarChanged = 1;
				}

				ids = ids->sibling->child;
			}
		}
	}
	
	ParseTreeNode *temp = ptNode->child;

	while(temp != NULL)
	{
		Semantic5(temp,idName);
		temp = temp->sibling;
	}
}

void findVar(ParseTreeNode *ptNode, char idName[30])
{
	if(ptNode == NULL || VarChanged == 1)
	{
		return;
	}
	else if(!strcmp(ptNode->token,"TK_READ"))
	{
		if(!strcmp(ptNode->sibling->child->lexemeCurrentNode,idName))
		{
			VarChanged = 1;
		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<assignmentStmt>"))
	{
		if(!strcmp(ptNode->child->child->lexemeCurrentNode,idName))
		{
			VarChanged = 1;
		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<outputParameters>"))
	{
		if(ptNode->child != NULL)
		{
			ParseTreeNode *ids = ptNode->child->child;

			while(ids != NULL)
			{
				if(!strcmp(ids->lexemeCurrentNode,idName))
				{
					VarChanged = 1;
				}

				ids = ids->sibling->child;
			}
		}
	}
	
	while(ptNode != NULL)
	{
		ParseTreeNode *temp = ptNode->child;

		while(temp != NULL)
		{
			Semantic5(temp,idName);
			temp = temp->sibling;
		}

		ptNode = ptNode->sibling;
	}
}

void Semantic15(ParseTreeNode *ptNode, ParseTreeNode *stmts)
{
	if(VarChanged == 1)
	{
		return;
	}
	else if(!strcmp(ptNode->child->NodeSymbol,"<var>"))
	{
		if(!strcmp(ptNode->child->child->token,"TK_ID"))
		{
			findVar(stmts,ptNode->child->child->lexemeCurrentNode);
		}
		else if(!strcmp(ptNode->child->sibling->sibling->child->token,"TK_ID"))
		{
			findVar(stmts,ptNode->child->sibling->sibling->child->lexemeCurrentNode);
		}
	}
	else if(!strcmp(ptNode->child->token,"TK_NOT"))
	{
		Semantic15(ptNode->child->sibling,stmts);
	}
	else
	{
		Semantic15(ptNode->child,stmts);
		Semantic15(ptNode->child->sibling->sibling,stmts);
	}

}



void semanticAnalysis(ParseTreeNode *ptNode)
{
	if(!strcmp(ptNode->token,"TK_FUNID") && !strcmp(ptNode->parentNodeSymbol,"<function>"))
	{
		FunctionScope *functionPtr = ptNode->functionPtr;


		if(ptNode->sibling->sibling->child != NULL)
		{
			ParseTreeNode *outPar = ptNode->sibling->sibling->child->sibling->child;
			ParseTreeNode *returnPar = ptNode->sibling->sibling->sibling->child->sibling->sibling->sibling;

			char idName[30];

			while(outPar != NULL)
			{
				VarChanged = 0;
				strcpy(idName,outPar->sibling->lexemeCurrentNode);
				Semantic5(ptNode->sibling->sibling->sibling->child->sibling->sibling, idName);

				if(VarChanged == 0)
				{
					printf("Error: One of the output parameters (%s) of function %s does not change.\n", idName, functionPtr->funcName);
					semanticanalysis_error = 1;
					break;
				}

				if(returnPar->child == NULL || strcmp(outPar->sibling->lexemeCurrentNode, returnPar->child->lexemeCurrentNode))
				{
					printf("Error: Output parameters and returned parameters of function %s do not match.\n", functionPtr->funcName);
					semanticanalysis_error = 1;
					break;
				}

				returnPar = returnPar->child->sibling;
				outPar = outPar->sibling->sibling->child;
			}
			if(semanticanalysis_error != 1 && returnPar->child != NULL)
			{
				printf("Error: Output parameters and returned parameters of function %s do not match.\n", functionPtr->funcName);
				semanticanalysis_error = 1;
			}
		}
	}
	else if(!strcmp(ptNode->token,"TK_WHILE"))
	{
		VarChanged = 0;
		Semantic15(ptNode->sibling,ptNode->sibling->sibling);
		if(VarChanged == 0)
		{
			printf("Error: None of the variables involved in the condition for while at line %d change.\n", ptNode->lineno);
			semanticanalysis_error = 1;
		}
	}

	ParseTreeNode *temp = ptNode->child;
	while(temp!=NULL)
	{
		// printParseTreeNode(temp, stdout);
		semanticAnalysis(temp);
		temp = temp->sibling;
	}
}