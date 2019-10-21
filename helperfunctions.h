/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include"lexerDef.h"
#include"parserDef.h"

#ifndef HELPERFUNCTIONS
#define HELPERFUNCTIONS

void read_grammer();

void print_ptr(Variables *ptr);

void add_variable(Variables **head_ptr, char *value);

int find_index_of(char *string, Variables *linked_list);

void print_Parse_Table(int T[][no_of_terminals]);

#endif