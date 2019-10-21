/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.h"
#include"symbolTable.h"
#include"typechecker.h"

int typechecker_error = 0;

char * checkArith2(ParseTreeNode * ptNode,char * type){
	if(ptNode->isLeafNode){

		char * t=(char *)malloc(30*sizeof(char));

		if(strcmp(ptNode->token,"TK_ID")){
			if(!strcmp(ptNode->token,"TK_NUM") &&strcmp(type,"int")){
				printf("Error: Type 'int' cant be used in RHS of Expression at line %d\n",ptNode->lineno);
				typechecker_error = 1;
			}
			else if(!strcmp(ptNode->token,"TK_RNUM") && strcmp(type,"real") ){
				printf("Error: Type 'real' cant be used in RHS of Expression at line %d\n",ptNode->lineno);
				typechecker_error = 1;
			}
			strcpy(t,type);
		}
		else if(!strcmp(ptNode->token,"TK_ID")){
			ParseTreeNode * temp=ptNode;
			if(temp->sibling->child==NULL){
				if(!strcmp(ptNode->identifierPtr->type,"int") &&strcmp(type,"int")){
					printf("Error: %s of type 'int' cant be used in RHS of Expression at line %d \n",\
						ptNode->lexemeCurrentNode,ptNode->lineno);
					typechecker_error = 1;
					strcpy(t,type);
				}
				else if(!strcmp(ptNode->identifierPtr->type,"real") &&strcmp(type,"real")){
					printf("Error: %s of type 'real' cant be used in RHS of Expression at line %d \n",\
						ptNode->lexemeCurrentNode,ptNode->lineno);
					typechecker_error = 1;
					strcpy(t,type);
				}
				else{
					strcpy(t,ptNode->identifierPtr->type);
					// typechecker_error = 1;
				}
			}
			else{
				char fieldid[30],fieldtype[30];
				strcpy(fieldid,temp->sibling->child->lexemeCurrentNode);

				Identifiers * ids=temp->identifierPtr;
				Records * rec=ids->records;

				while(rec != NULL)
				{
					if(!strcmp(rec->IDname, fieldid))
					{
						strcpy(fieldtype,rec->type);
						break;
					}

					rec = rec->next;
				}
				if(rec == NULL)
				{
					printf("Error: No fieldid %s for record of type %s at line %d\n",\
						temp->sibling->child->lexemeCurrentNode,temp->identifierPtr->type,temp->lineno );
					typechecker_error = 1;
				}
				else if(strcmp(fieldtype,type) ){
					printf("Error: %s.%s of type '%s' cant be used in RHS of Expression at line %d \n",\
						ptNode->lexemeCurrentNode,ptNode->sibling->child->lexemeCurrentNode,fieldtype,ptNode->lineno);
					typechecker_error = 1;
				}

				strcpy(t,type);
			}
		}
		return t;
	}
	else if(!strcmp(ptNode->child->NodeSymbol,"<term>") ){
		ParseTreeNode * temp=ptNode->child;
		char * t1,* t2;
		t1=checkArith2(temp,type);

		if(temp->sibling->child==NULL){

			return t1;
		}
		t2=checkArith2(temp->sibling,type);
		if(strcmp(t1,t2)){
			printf("Type mismatch at line %d: incompatible operation between %s and %s\n",\
				temp->sibling->child->lineno,t1,t2);
			typechecker_error = 1;
		}
		free(t2);
		return t1;
	}
	else if(!strcmp(ptNode->child->NodeSymbol,"<lowPrecedenceOperators>") ){
		ParseTreeNode * temp=ptNode->child;
		char * t1,* t2;
		t1=checkArith2(temp->sibling,type);
		if(temp->sibling->sibling->child==NULL){
			return t1;
		}
		t2=checkArith2(temp->sibling->sibling,type);
		if(strcmp(t1,t2)){
			printf("Type mismatch at line %d: incompatible operation between %s and %s\n",\
				ptNode->child->child->lineno,t1,t2);
			typechecker_error = 1;
		}
		free(t2);
		return t1;

	}
	else if(!strcmp(ptNode->child->NodeSymbol,"<factor>") ){
		ParseTreeNode * temp=ptNode->child;
		char * t1, * t2;
		t1=checkArith2(temp,type);
		if(temp->sibling->child==NULL){
			return t1;
		}
		t2=checkArith2(temp->sibling,type);
		if(strcmp(t1,"int")&&strcmp(t1,"real")&&strcmp(t2,"int")&&strcmp(t2,"real") ){
			printf("Type mismatch at line %d: incompatible operation between %s and %s\n",\
				temp->sibling->child->lineno,t1,t2);
			typechecker_error = 1;
		}
		else if(strcmp(t1,"int")&&strcmp(t1,"real")){
			free(t2);
			return t1;
		}
		else if(strcmp(t2,"int")&&strcmp(t2,"real")){
			free(t1);
			return t2;
		}
		else if(strcmp(t1,t2)){
			printf("Type mismatch at line %d: %s and %s\n",\
				temp->sibling->child->lineno,t1,t2);
			typechecker_error = 1;
		}
		free(t1);
		return t2;
	}

	else if(!strcmp(ptNode->child->NodeSymbol,"<highPrecedenceOperators>") ){
		ParseTreeNode * temp=ptNode->child;
		char * t1,* t2;

		t1=checkArith2(temp->sibling,type);
		if(temp->sibling->sibling->child==NULL){

			if(!strcmp(temp->child->token,"TK_DIV") ){
				if(strcmp(t1,"int")&&strcmp(t1,"real") ){
					printf("Division by record type not defined at line: %d\n",\
					temp->child->lineno);
					typechecker_error = 1;
					return t1;
				}
			}




			return t1;
		}
		
		t2=checkArith2(temp->sibling->sibling,type);
		if(!strcmp(temp->child->token,"TK_DIV") ){
			if(strcmp(t1,"int")&&strcmp(t1,"real") ){
				printf("Division by record type not defined at line: %d\n",\
					temp->child->lineno);
				typechecker_error = 1;
				free(t2);
				return t1;
			}
			else if(strcmp(t2,"int")&&strcmp(t2,"real")){
				free(t1);
				return t2;
			}
			else if(strcmp(t1,t2) ){
				printf("Type mismatch at line %d: %s and %s\n",\
					temp->child->lineno,t1,t2);
				typechecker_error = 1;
			}

			free(t2);
			return t1;
		}
		if(strcmp(t1,"int")&&strcmp(t1,"real")&&strcmp(t2,"int")&&strcmp(t2,"real")){
			printf("Type mismatch at line :%d, %s * %s not defined\n",\
				ptNode->child->child->lineno,t1,t2);
			typechecker_error = 1;
			free(t2);return(t1);
		}
		else if(strcmp(t1,"int")&&strcmp(t1,"real")){
			//"check if record t1 has ints || reals"
			free(t2);
			return t1;
		}
		else if(strcmp(t2,"int")&&strcmp(t2,"real")){
			//"check if record t2 has ints || reals"
			free(t1);
			return t2;
		}
		else if(strcmp(t1,t2) ){
			printf("Type mismatch at line %d: %s and %s\n",\
				ptNode->child->child->lineno,t1,t2);
			typechecker_error = 1;
			free(t1);
			return t2;
		}

	}
	else{
		return checkArith2(ptNode->child,type);
	}	
	return NULL;
}


void checkArith1(ParseTreeNode * ptNode,char * type){
	if(type==NULL){
		type=(char *)malloc(30*sizeof(char));
		ParseTreeNode * temp=ptNode->child->child;
		if(temp->sibling->child==NULL){
			strcpy(type,temp->identifierPtr->type);
		}
		else{
			char fieldid[30];
			strcpy(fieldid,temp->sibling->child->lexemeCurrentNode);

			Identifiers * ids=temp->identifierPtr;
			Records * rec=ids->records;

			while(rec != NULL)
			{
				if(!strcmp(rec->IDname, fieldid))
				{
					strcpy(type,rec->type);
					break;
				}

				rec = rec->next;
			}
			if(rec == NULL)
			{
				printf("Error: No fieldid of this type\n");
				typechecker_error = 1;
			}
		}

		temp=ptNode->child->sibling->sibling;
		while(temp!=NULL){
			checkArith1(temp,type);
			temp=temp->sibling;
		}
	}
	else if(ptNode->isLeafNode){
		char a[][20]={"TK_MINUS","TK_PLUS","TK_DIV","TK_MUL","TK_FIELDID"};
		int i;
		for(i=0;i<4;i++){
			if(!strcmp(ptNode->token,a[i])){
				return;
			}
		}
		if(strcmp(ptNode->token,"TK_ID")){
			if(!strcmp(ptNode->token,"TK_NUM")&&strcmp(type,"int")){
				printf("Type mismatch at line : %d Expected %s but got 'int'\n",ptNode->lineno,type);
				typechecker_error = 1;
			}
			else if(!strcmp(ptNode->token,"TK_RNUM")&&strcmp(type,"real")){
				printf("Type mismatch at line : %d Expected %s but got 'real'\n",ptNode->lineno,type);
				typechecker_error = 1;
			}
		}
		else if(!strcmp(ptNode->token,"TK_ID")){
			ParseTreeNode * temp=ptNode;
			if(temp->sibling->child==NULL){
				if(strcmp(type,temp->identifierPtr->type))
				{
					printf("Type mismatch at line : %d Expected %s but got %s\n",ptNode->lineno,type,temp->identifierPtr->type);
					typechecker_error = 1;
				}

			}
			else{
				char fieldid[30],fieldtype[30];
				strcpy(fieldid,temp->sibling->child->lexemeCurrentNode);

				Identifiers * ids=temp->identifierPtr;
				Records * rec=ids->records;

				while(rec != NULL)
				{
					if(!strcmp(rec->IDname, fieldid))
					{
						strcpy(fieldtype,rec->type);
						break;
					}

					rec = rec->next;
				}
				if(rec == NULL)
				{
					printf("Error: No fieldid of this type\n");
					typechecker_error = 1;
				}

				if(strcmp(type,fieldtype))
				{
					printf("Type mismatch at line : %d Expected %s but got %s\n",ptNode->lineno,type,fieldtype);
					typechecker_error = 1;
				}
			}
		}
	}
	else{
		ParseTreeNode * temp = ptNode->child;
		while(temp!=NULL){
			checkArith1(temp,type);
			temp=temp->sibling;
		}
	}
}


//No such thing as type in Parsetreenode-- 
void checkBool(ParseTreeNode * ptNode){
	if(!strcmp(ptNode->child->NodeSymbol,"<var>")){
		ParseTreeNode * temp1=ptNode->child->sibling, * temp2=ptNode->child->child;
		temp1=temp1->sibling->child;
		if(!strcmp(temp1->token,"TK_ID")&&!strcmp(temp2->token,"TK_ID")&&strcmp(temp1->identifierPtr->type,temp2->identifierPtr->type)){
			printf("Type mismatch in boolean expression at line: %d %s and %s\n", temp1->lineno,temp1->identifierPtr->type,temp2->identifierPtr->type);
			typechecker_error = 1;
		}
		else if(strcmp(temp1->token,temp2->token)&&strcmp(temp1->token,"TK_ID")&&strcmp(temp2->token,"TK_ID")){
			printf("Type mismatch in boolean expression at line: %d %s and %s\n", temp1->lineno,temp1->token,temp2->identifierPtr->type);
			typechecker_error = 1;
		}
		else if(!strcmp(temp1->token,"TK_ID")&&strcmp(temp2->token,"TK_ID")&&strcmp(temp1->identifierPtr->type,temp2->token)){
			if(!strcmp(temp1->identifierPtr->type,"int")&&strcmp(temp2->token,"TK_NUM")){
				printf("Type mismatch in boolean expression at line: %d, %s:int and real\n", temp2->lineno,temp1->lexemeCurrentNode);
				typechecker_error = 1;
			}
			else if(!strcmp(temp2->identifierPtr->type,"real")&&strcmp(temp1->token,"real")){
				printf("Type mismatch in boolean expression at line: %d,%s:real and int\n", temp2->lineno,temp1->lexemeCurrentNode);
				typechecker_error = 1;
			}
		}
		else if(!strcmp(temp2->token,"TK_ID")&&strcmp(temp1->token,"TK_ID")){
			if(!strcmp(temp2->identifierPtr->type,"int")&&strcmp(temp1->token,"TK_NUM")){
				printf("Type mismatch in boolean expression at line: %d, real%s and %s:int\n", temp1->lineno,temp1->token,temp2->lexemeCurrentNode);
				typechecker_error = 1;
			}
			else if(!strcmp(temp2->identifierPtr->type,"real")&&strcmp(temp1->token,"TK_RNUM")){
				printf("Type mismatch in boolean expression at line: %d, int and %s:real\n", temp1->lineno,temp2->lexemeCurrentNode);
				typechecker_error = 1;
			}
		}
	}
	else if(!strcmp(ptNode->child->token,"TK_NOT")){
		checkBool(ptNode->child->sibling);
	}
	else {
		checkBool(ptNode->child);
		checkBool(ptNode->child->sibling->sibling);
	}
}


ParamList* checkFuncCall(ParseTreeNode * ptNode,ParamList * l){

	if(ptNode->isLeafNode){
		if(!strcmp(ptNode->lexemeCurrentNode,"<---"))
		{

		}
		else if(l==NULL){
			printf("Too many parameters at line :%d %s\n", ptNode->lineno, ptNode->lexemeCurrentNode);
			typechecker_error = 1;
		}
		else if(strcmp(ptNode->identifierPtr->type,l->type)){
			printf("Parameter type mismatch at line: %d , expected %s , received %s for %s\n",\
				ptNode->lineno,l->type,ptNode->identifierPtr->type,ptNode->lexemeCurrentNode);
			typechecker_error = 1;
		}
		else{
			l=l->next;
		}
	}
	else{
		ParseTreeNode * temp=ptNode->child;
		while(temp!=NULL){
			l = checkFuncCall(temp,l);
			temp=temp->sibling;
		}
	}

	return l;
}

void typeCheck(ParseTreeNode * ptNode){
	if(strcmp(ptNode->NodeSymbol,"<assignmentStmt>")==0){

		char type[30];
		ParseTreeNode * temp=ptNode->child->child;
		if(temp->sibling->child==NULL){
			strcpy(type,temp->identifierPtr->type);
			if(temp->identifierPtr->records!=NULL){
				int flag = 0;
				char prevType[30];
				Records *records = temp->identifierPtr->records;
				strcpy(prevType,records->type);
				records = records->next;
				while(records != NULL)
				{
					if(strcmp(records->type,prevType))
					{
						flag = 1;
						break;
					}
					records = records->next;
				}
				if(flag)
				{
					strcpy(type,"garbage");
				}
				else
				{
					strcpy(type,prevType);
				}
				char * t=checkArith2(ptNode->child->sibling->sibling,type);
				if(strcmp(t,temp->identifierPtr->type)){
					printf("Type mismatch at line %d: %s and %s\n",\
						temp->lineno,type,t);
					typechecker_error = 1;
				}
			}
			else{
				//printf("****HELLO %d\n",temp->lineno);
				checkArith1(ptNode->child->sibling->sibling,type);
			}
		}
		else{
			char fieldid[30];
			strcpy(fieldid,temp->sibling->child->lexemeCurrentNode);

			Identifiers * ids=temp->identifierPtr;
			Records * rec=ids->records;

			while(rec != NULL)
			{
				if(!strcmp(rec->IDname, fieldid))
				{
					strcpy(type,rec->type);
					break;
				}

				rec = rec->next;
			}
			if(rec == NULL)
			{
				printf("Error: No fieldid of this type\n");
				typechecker_error = 1;
			}

			temp=ptNode->child->sibling->sibling;
			while(temp!=NULL){
				checkArith1(temp,type);
				temp=temp->sibling;
			}

		}
		return;
	}
	else if(strcmp(ptNode->NodeSymbol,"<booleanExpression>")==0){
		checkBool(ptNode);
		return;
	}
	else if(strcmp(ptNode->NodeSymbol,"<funCallStmt>")==0){
		ParseTreeNode * temp=ptNode->child;
		ParamList * l=temp->sibling->functionPtr->outputParams;
		l=checkFuncCall(temp,l);
		if(l!=NULL){
			printf("Too few parameters at line: %d\n",temp->sibling->lineno);
			typechecker_error = 1;
		}
		l=temp->sibling->functionPtr->inputParams;
		temp=temp->sibling->sibling;
		l=checkFuncCall(temp,l);
		if(l!=NULL){
			printf("Too few parameters at line: %d\n",temp->sibling->lineno);
			typechecker_error = 1;
		}
		return;
	}
	ParseTreeNode * temp=ptNode->child;
	while(temp!=NULL){
		//printf("**%s\n", temp->NodeSymbol);
		typeCheck(temp);
		temp=temp->sibling;
	}
}