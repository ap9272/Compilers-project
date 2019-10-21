/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"firstfollow.h"
#include"helperfunctions.h"
#include"parser.h"
#include"lexer.h"

char a[][30]={"TK_COMMENT","TK_WITH","TK_PARAMETERS","TK_END", "TK_COMMA",\
"TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT","TK_SEM","TK_COLON",\
"TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_THEN","TK_ENDIF","TK_RETURN","TK_CALL","TK_ENDRECORD","eps"};

int syntactic_error;

void createAST(ParseTreeNode *ptNode){
 	ParseTreeNode *prev = ptNode, *curr = ptNode->child, *temp;
 	int i;
 	while(curr!=NULL)
 	{
 		for( i = 0; i < 22; i++)
 		{
 			if(!strcmp(a[i],curr->token))
 			{
 				if(prev != ptNode)
 				{
 					prev->sibling = temp = curr->sibling;
 				}
 				else
 				{
 					prev->child = temp = curr->sibling;
 				}
 				free(curr);
 				curr=temp;
 				break;
 			}
 		}
 		if(i==22)
 		{
 			createAST(curr);
 			prev = curr;
 			curr = curr->sibling;
 		}
 	}
 	if(ptNode->child == prev && prev->child != NULL && strcmp(ptNode->NodeSymbol,"<stmt>"))
 	{
 		ptNode->child = prev->child;
 		prev->child->parent = ptNode;
 		strcpy(prev->child->parentNodeSymbol,ptNode->NodeSymbol);

 		ParseTreeNode *sib_trav = prev->child->sibling;

		while (sib_trav != NULL)
		{
			sib_trav->parent = ptNode;
			strcpy(sib_trav->parentNodeSymbol,ptNode->NodeSymbol);
			sib_trav = sib_trav->sibling;
		}

 		free(prev);
 	}
 }

void createParseTable(Variables* G[], int T[][no_of_terminals])
{
	int i;
	extern int no_of_rules;

	for (i=0; i<no_of_rules; ++i)
	{
		int non_terminal_idx = find_index_of(G[i]->value,non_terminals);
		Variables *head = NULL;
		Variables **ptr = &head;
		int eps = 0;

		Variables *traverse = G[i]->next;
		while(traverse != NULL)
		{
			eps = 0;
			head = NULL;
			first(traverse->value, ptr);
			Variables *first_set = *ptr;
			while (first_set != NULL)
			{
				if(strcmp(first_set->value,"eps") != 0)
				{
					int terminal_idx = find_index_of(first_set->value, terminals);
					T[non_terminal_idx][terminal_idx] = i;
				}
				else
				{
					eps = 1;
				}
				first_set = first_set->next;
			}

			if (eps == 0)
			{
				break;
			}

			traverse = traverse->next;
		}
		if (eps == 1)
		{
			head = NULL;
			begin = 0;
			follow(G[i]->value, ptr, G[i]->value);

			Variables *follow_set = *ptr;
			while (follow_set != NULL)
			{
				int terminal_idx = find_index_of(follow_set->value, terminals);
				T[non_terminal_idx][terminal_idx] = i;

				follow_set = follow_set->next;
			}
		}
	}
}


int addToStack(char stack[][25],int sp,int ruleNo)
{
	Variables * temp=rules[ruleNo]->next;
	if ( !strcmp(temp->value,"eps"))
	{
		return sp-1;
	}
	int i=0,k;
	while(temp!=NULL)
	{
		i++;temp=temp->next;
	}
	temp=rules[ruleNo]->next;
	sp=sp+i;k=sp;
	while(i--)
	{
		strcpy(stack[--sp],temp->value);
		temp=temp->next;
	}
	return k-1;
}

ParseTreeNode* getNextTreeNode(ParseTreeNode *ptreeNode)
{
	if (ptreeNode == NULL)
		return NULL;
	if (ptreeNode->sibling == NULL)
	{
		return getNextTreeNode(ptreeNode->parent);
	}
	else if (strcmp(ptreeNode->sibling->token,"eps") == 0)
	{
		return getNextTreeNode(ptreeNode->sibling);
	}
	else
	{
		return ptreeNode->sibling;
	}
}

ParseTreeNode* addToTree(ParseTreeNode *ptreeNode, int ruleno, tokenlist *token)
{
	ptreeNode->child = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
	ptreeNode->child->parent = ptreeNode;
	ptreeNode->child->child = NULL;

	Variables *traverse = rules[ruleno]->next;
	ParseTreeNode *sib_trav = ptreeNode->child;

	if(traverse->value[0] == '<')
	{
		strcpy(ptreeNode->child->lexemeCurrentNode,"---");
		strcpy(ptreeNode->child->token,"---");
		strcpy(ptreeNode->child->parentNodeSymbol, ptreeNode->NodeSymbol);
		ptreeNode->child->isLeafNode = 0;
		strcpy(ptreeNode->child->NodeSymbol,traverse->value);
	}
	else if(traverse->value[0] == 'T')
	{
		ptreeNode->child->lineno = token->lineno;
		strcpy(ptreeNode->child->token,token->t.tokenName);
		if(strcmp(token->t.tokenName,"TK_NUM") == 0 || strcmp(token->t.tokenName,"TK_RNUM") == 0)
		{
			strcpy(ptreeNode->child->lexemeCurrentNode,"---");
			strcpy(ptreeNode->child->valuelfNumber, token->t.lexeme);			
		}
		else
		{
			strcpy(ptreeNode->child->lexemeCurrentNode,token->t.lexeme);
			strcpy(ptreeNode->child->valuelfNumber,"---");
		}

		strcpy(ptreeNode->child->parentNodeSymbol, ptreeNode->NodeSymbol);
		ptreeNode->child->isLeafNode = 1;
	}
	else
	{
		strcpy(ptreeNode->child->lexemeCurrentNode,"---");
		strcpy(ptreeNode->child->token,"eps");
		strcpy(ptreeNode->child->parentNodeSymbol, ptreeNode->NodeSymbol);
		ptreeNode->child->isLeafNode = 1;
	}

	traverse = traverse->next;

	while (traverse != NULL)
	{
		sib_trav->sibling = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
		sib_trav = sib_trav->sibling;

		if(traverse->value[0] == '<')
		{
			strcpy(sib_trav->lexemeCurrentNode,"---");
			strcpy(sib_trav->token,"---");
			strcpy(sib_trav->parentNodeSymbol, ptreeNode->NodeSymbol);
			sib_trav->isLeafNode = 0;
			strcpy(sib_trav->NodeSymbol,traverse->value);
		}
		else if(traverse->value[0] == 'T')
		{
			sib_trav->lineno = token->lineno;
			strcpy(sib_trav->token,token->t.tokenName);
			if(strcmp(token->t.tokenName,"TK_NUM") == 0 || strcmp(token->t.tokenName,"TK_RNUM") == 0)
			{
				strcpy(sib_trav->lexemeCurrentNode,"---");
				strcpy(sib_trav->valuelfNumber, ptreeNode->token);			
			}
			else
			{
				strcpy(sib_trav->lexemeCurrentNode,ptreeNode->token);
				strcpy(sib_trav->valuelfNumber,"---");
			}

			strcpy(sib_trav->parentNodeSymbol, ptreeNode->NodeSymbol);
			sib_trav->isLeafNode = 1;
		}


		sib_trav->parent = ptreeNode;
		sib_trav->child = sib_trav->sibling = NULL;
		
		traverse = traverse->next;
	}

	if (ptreeNode->child->token[0] == 'e')
		return getNextTreeNode(ptreeNode->child);

	return ptreeNode->child;
}

void printParseTreeNode(ParseTreeNode *ptNode, FILE *fp)
{
	fprintf(fp, "%s,", ptNode->lexemeCurrentNode);
	
	if(ptNode->token[0] == '-' || ptNode->token[0] == 'e')
		fprintf(fp, "---,");
	else
		fprintf(fp, "%d,", ptNode->lineno);
	
	fprintf(fp, "%s,", ptNode->token);
	
	if(ptNode->token[0] == '-' || ptNode->token[0] == 'e')
		fprintf(fp, "---,");
	else
		fprintf(fp, "%s,", ptNode->valuelfNumber);
	
	fprintf(fp, "%s,", ptNode->parentNodeSymbol);
	
	if(ptNode->isLeafNode == 0)
		fprintf(fp, "no,");
	else
		fprintf(fp, "yes,");
	
	if(ptNode->token[0] == '-')
		fprintf(fp, "%s\n", ptNode->NodeSymbol);
	else
		fprintf(fp, "---\n");

	ptNode->functionPtr = NULL;
	ptNode->identifierPtr = NULL;
	ptNode->recordDefPtr = NULL;
}


void printParseTree(ParseTreeNode *PT, char *outfile)
{
	FILE *fp = stdout; //fopen(outfile,"w");

	ParseTreeNode* stack[1000];
	int sp = 0, ss = 0;

	printParseTreeNode(PT,fp);

	int children_no = 0;
	ParseTreeNode *traverse = PT->child;
	while(traverse != NULL)
	{
		children_no++;
		traverse = traverse->sibling;
	}

	ss = sp+children_no;

	traverse = PT->child;
	while(traverse != NULL)
	{
		stack[ss] = traverse;
		ss--;
		traverse = traverse->sibling;
	}

	sp = sp + children_no;

	while( sp > 0 )
	{
		printParseTreeNode(stack[sp],fp);
		if (stack[sp] != NULL && stack[sp]->child != NULL)
		{
			children_no = 0;
			ParseTreeNode *traverse = stack[sp]->child;
			while(traverse != NULL)
			{

				children_no++;
				traverse = traverse->sibling;
			}

			ss = sp+children_no-1;

			traverse = stack[sp]->child;


			sp--;
			while(traverse != NULL)
			{				
				stack[ss] = traverse;
				ss--;
				traverse = traverse->sibling;
			}
			sp = sp + children_no;
		}
		else
		{
			sp--;
		}
	}

	printf("Parse tree printed to file!!\n");
	fclose(fp);

}

ParseTreeNode*  parseInputSourceCode(char *testcaseFile, int parseTable[][no_of_terminals])
{
	FILE *temp_open = fopen(testcaseFile, "ra");
	if(fseek(temp_open, -1, SEEK_END) == 0)
	{
		if(fgetc(temp_open) == '\n')
		{
			fclose(temp_open);
		}
		else
		{
			fputc('\n',temp_open);
			fclose(temp_open);	
		}
	}


	fp=fopen (testcaseFile, "rb");
	block=(char *)malloc(1024);
	buff=(char *)malloc(1024);
	bytesread = -1;

	int sp=0,i,j,error_found = 0;
	char stack[500][25];
	ParseTreeNode *Ptree, *ptreeNode;


	tokenlist *temp = (tokenlist *)malloc(sizeof(tokenlist));
	temp=getNextToken(block,temp);

	strcpy(stack[sp++],"$");
	strcpy(stack[sp],"<program>");

	Ptree = ptreeNode = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));

	strcpy(Ptree->lexemeCurrentNode,"---");
	strcpy(Ptree->token,"---");
	strcpy(Ptree->parentNodeSymbol,"ROOT");
	Ptree->isLeafNode = 0;
	strcpy(Ptree->NodeSymbol,rules[0]->value);
	Ptree->parent = Ptree->child = Ptree->sibling = NULL;
	
	while ( sp > 0 && temp->t.tokenName[0] != '\0') 
	{
		if (!strcmp(temp->t.tokenName,"TK_COMMENT"))
		{
			temp = getNextToken(block,temp);
			continue;
		}
		if(!strcmp(temp->t.tokenName,stack[sp]))
		{
			sp--;
			ptreeNode->lineno = temp->lineno;
			strcpy(ptreeNode->token,temp->t.tokenName);
			if(strcmp(temp->t.tokenName,"TK_NUM") == 0 || strcmp(temp->t.tokenName,"TK_RNUM") == 0)
			{
				strcpy(ptreeNode->lexemeCurrentNode,"---");
				strcpy(ptreeNode->valuelfNumber, temp->t.lexeme);			
			}
			else
			{
				strcpy(ptreeNode->lexemeCurrentNode,temp->t.lexeme);
				strcpy(ptreeNode->valuelfNumber,"---");
			}

			strcpy(ptreeNode->parentNodeSymbol, ptreeNode->parent->NodeSymbol);
			ptreeNode->isLeafNode = 1;


			ptreeNode = getNextTreeNode(ptreeNode);
			temp=getNextToken(block,temp);
			continue;
		}
		else if(stack[sp][0]=='<')
		{
			i = find_index_of(stack[sp], non_terminals);
			j = find_index_of(temp->t.tokenName, terminals);
			if(parseTable[i][j] != -1)
			{
				sp = addToStack(stack,sp,parseTable[i][j]);
				// printParseTreeNode(ptreeNode, stdout);
				ptreeNode = addToTree(ptreeNode,parseTable[i][j], temp);
			}
			else
			{
				//error
				error_found = 1;
				if(strcmp(temp->t.tokenName,"TK_ERROR"))
				{	
					printf("ERROR_5: The token <%s> for lexeme <%s> does not match at line <%d>. The expected token here is <%s>\n", temp->t.tokenName, temp->t.lexeme, temp->lineno, stack[sp]);
				}
				else
				{
					temp=getNextToken(block,temp);				
				}
				sp--;
				if(stack[sp][0] == '<')
				{
					Variables *head = NULL;
					Variables **ptr = &head;

					first(stack[sp], ptr);
					Variables *traverse= *ptr;
					Variables* temp_trav;
					while(temp->t.tokenName[0] != '\0')
					{
						
						if(strcmp(temp->t.tokenName,"TK_COMMENT"))
						{
							temp_trav = traverse;
							while(temp_trav != NULL)
							{
								if(strcmp(temp_trav->value, temp->t.tokenName) == 0)
								{
									break;
								}
								temp_trav = temp_trav->next;
							}
						}
						if( temp_trav != NULL)
						{
							break;
						}
						temp = getNextToken(block,temp);
					}
				}
			}
		}
		else
		{
			//error
			printf("b");
			error_found = 1;
			if(strcmp(temp->t.tokenName,"TK_ERROR"))
			{	
				printf("ERROR_5: The token <%s> for lexeme <%s> does not match at line <%d>. The expected token here is <%s>\n", temp->t.tokenName, temp->t.lexeme, temp->lineno, stack[sp]);
			}
			else
			{
				temp=getNextToken(block,temp);				
			}
			sp--;
			ptreeNode->lineno = temp->lineno;
			strcpy(ptreeNode->token,temp->t.tokenName);
			if(strcmp(temp->t.tokenName,"TK_NUM") == 0 || strcmp(temp->t.tokenName,"TK_RNUM") == 0)
			{
				strcpy(ptreeNode->lexemeCurrentNode,"---");
				strcpy(ptreeNode->valuelfNumber, temp->t.lexeme);			
			}
			else
			{
				strcpy(ptreeNode->lexemeCurrentNode,temp->t.lexeme);
				strcpy(ptreeNode->valuelfNumber,"---");
			}

			strcpy(ptreeNode->parentNodeSymbol, ptreeNode->parent->NodeSymbol);
			ptreeNode->isLeafNode = 1;
			ptreeNode = getNextTreeNode(ptreeNode);

			continue;
		}
	}
	temp=getNextToken(block,temp);
	while(temp->t.tokenName[0] != '\0' && strcmp(temp->t.tokenName,"TK_COMMENT") == 0)
	{
		temp=getNextToken(block,temp);
	}
	if ( (temp->t.tokenName[0] != '\0' || sp > 0) || error_found)
	{
		//error
		printf("Compilation failed: Errors found in code!!\n");
	}
	else
	{
		printf("Compiled Successfully: Input source code is syntactically correct!!\n");
	}

	if(error_found)
	{
		syntactic_error = 1;
	}

	return Ptree;
}

