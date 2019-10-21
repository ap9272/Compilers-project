/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include"lexerDef.h"
#include"parserDef.h"

#ifndef FIRSTFOLLOW
#define FIRSTFOLLOW

void first(char* variable, Variables **result);

void follow(char* variable, Variables **result, char* start);

#endif