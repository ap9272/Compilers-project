/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"lexerDef.h"
#include"parserDef.h"
#include"firstfollow.h"

extern int no_of_rules;

void print_ptr(Variables *ptr)
{
		while(ptr != NULL)
		{
			printf("%s, ",ptr->value);
			ptr = ptr->next;
		}
		printf("\n");
}

int add_variable(Variables **head_ptr, char *value)
{
	int i=0;
	Variables *ptr = *head_ptr;
	if (*head_ptr == NULL)
	{
		*head_ptr = (Variables*)malloc(sizeof(Variables));
		strcpy((*head_ptr)->value,value);
		(*head_ptr)->next = NULL;
		return 1;
	}
	while(ptr->next != NULL)
	{
		if (strcmp(ptr->value,value) == 0)
		{
			return i+1;
		}
		i++;
		ptr = ptr->next;
	}
	if (strcmp(ptr->value,value) == 0)
	{
			return ++i;
	}
	ptr->next = (Variables*)malloc(sizeof(Variables));
	ptr = ptr->next;
	strcpy(ptr->value,value);
	ptr->next = NULL;
	return i+2;
}

void read_grammer(char *grammer_file)
{
	char t[5];
	//getline(&t,&len,stdin);

	FILE *fp = fopen(grammer_file,"r");
	fgets(t,5,fp);

	no_of_rules = atoi(t);
	no_of_terminals = 0;
	no_of_non_terminals = 0;


	int i;
	for ( i=0; i<MAX_RULES; ++i)
	{
		rules[i] = NULL;
	}
	terminals = NULL;
	non_terminals = NULL;
	Variables **terminals_ptr = &terminals;
	Variables **non_terminals_ptr = &non_terminals;

	for( i=0; i<no_of_rules; ++i)
	{
		char rule[200];
		if(fgets(rule, 200, fp) == NULL)
			break;


		int j,l,x=0,flag = 1;
		Variables *ptr;
		rules[i] = (Variables*)malloc(sizeof(Variables));
		ptr = rules[i];
		for (	j=0,l=strlen(rule); j<l; j++)
		{
			if (rule[j] == '\n')
			{
				ptr->value[x]='\0';
				ptr->next = NULL;

				if (ptr->value[0] == '<')
				{
					if(flag)
					{
						no_of_non_terminals = add_variable(non_terminals_ptr,ptr->value);
						flag = 0;
					}
				}
				else if (strcmp(ptr->value, "eps") != 0)
				{
					add_variable(terminals_ptr,ptr->value);
				}
				break;	
			}
			else if (rule[j] == ' ')
			{
				ptr->value[x]='\0';
				ptr->next = (Variables*)malloc(sizeof(Variables));
				
				if (ptr->value[0] == '<')
				{
					if(flag)
					{
						no_of_non_terminals = add_variable(non_terminals_ptr,ptr->value);
						flag = 0;
					}
				}
				else if (strcmp(ptr->value, "eps") != 0)
				{
					add_variable(terminals_ptr,ptr->value);
				}
				
				ptr = ptr->next;
				x=0;
			}
			else if (rule[j] == '=' && rule[j+1] == '=' && rule[j+2] == '=' && rule[j+3] == '>')
				j += 4;
			else
			{
				ptr->value[x++] = rule[j];
			}

		}
	}

	no_of_terminals = add_variable(terminals_ptr, "$");
}

int find_index_of(char *string, Variables *linked_list)
{
	int i = 0;
	Variables *traverse = linked_list;
	
	while(traverse != NULL)
	{
		if(strcmp(traverse->value,string) == 0)
		{
			return i;
		}
		i++;
		traverse = traverse->next;
	}

	return -1;
}

void print_Parse_Table(int T[][no_of_terminals])
{
	int i,j;
	Variables *traverse = terminals;
	printf("terminals,");
	while(traverse != NULL)
	{
		printf("%s,", traverse->value);
		traverse = traverse->next;
	}
	printf("\n");
	traverse = non_terminals;
	for (i = 0; i < no_of_non_terminals; ++i)
	{
		printf("%s,", traverse->value);
		for (j = 0; j < no_of_terminals; ++j)
		{
			printf("%d,", T[i][j]);
		}
		printf("\n");
		traverse = traverse->next;
	}
}