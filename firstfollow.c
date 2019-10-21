/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/
 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"helperfunctions.h"
#include"lexerDef.h"
#include"parserDef.h"

extern int no_of_rules;

void first(char* variable, Variables **result)
{
	int i;
	if(variable[0] != '<')
	{
		add_variable(result, variable);
	}
	else
	{
		for ( i = 0; i < no_of_rules; ++i)
		{
			if(strcmp(rules[i]->value,variable) == 0)
			{
				if(strcmp(rules[i]->next->value,"eps") == 0)
				{
					add_variable(result, "eps");
				}
				else
				{
					Variables *ptr = rules[i]->next;
					Variables *head_ptr = NULL;
					Variables **subRes = &head_ptr;
					int eps = 0;					
					while( ptr != NULL )
					{
						eps = 0;
						head_ptr = NULL;
						first(ptr->value, subRes);
						Variables *subres = *subRes;
						while (subres != NULL)
						{
							if(strcmp(subres->value,"eps") != 0)
							{
								add_variable(result, subres->value);
							}
							else
							{
								eps = 1;
							}
							subres = subres->next;
						}

						if (eps == 0)
						{
							break;
						}

						ptr = ptr->next;
					}
				}
			}
		}
	}
}

void follow(char* variable, Variables **result, char* start)
{
	int i;
	if (begin == 1 && strcmp(variable,start) == 0)
	{
		return;
	}
	begin = 1;
	if(strcmp(variable,rules[0]->value) == 0)
	{
		add_variable(result,"$");
	}
	for ( i=0; i<no_of_rules; ++i)
	{
		Variables *ptr = rules[i]->next;
		while ( ptr != NULL )
		{
			if( strcmp(ptr->value,variable) == 0 )
			{
				int eps=0;
				while ( ptr->next != NULL)
				{
					eps = 0;
					Variables *head_ptr = NULL;
					Variables **subRes = &head_ptr;
					first(ptr->next->value, subRes);
					Variables *subres = *subRes;
					while( subres != NULL)
					{
						if(strcmp(subres->value,"eps") != 0)
						{
							add_variable(result, subres->value);
						}
						else
						{
							eps = 1;
						}
						subres = subres->next;
					}
					if (eps == 0)
					{
						break;
					}
					ptr = ptr->next;
				}
				if ( ptr->next == NULL && strcmp(rules[i]->value,variable) != 0)
				{
					Variables *head_ptr = NULL;
					Variables **subRes = &head_ptr;
					follow(rules[i]->value, subRes, start);
					Variables *subres = *subRes;
					while( subres != NULL)
					{
						if(strcmp(subres->value,"eps") != 0)
						{
							add_variable(result, subres->value);
						}
						subres = subres->next;
					}
				}
			}
			ptr = ptr->next;
		}
	}
}