/*
 Batch number: 36
 Pratik Satish (2013A7PS057P)
 Anurag Prakash (2013A7PS061P)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

extern int counter;
char keywrds[25][25]={"with","parameters","end","while","int","real","type", \
"global","parameter","list","input","output","int","endwhile","if","then","endif","read","write","return","call","record","endrecord","else"};
char tk_keywrds[25][25]={"TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_INT","TK_REAL","TK_TYPE", "TK_GLOBAL","TK_PARAMETER", "TK_LIST","TK_INPUT",\
"TK_OUTPUT","TK_INT","TK_ENDWHILE","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_CALL","TK_RECORD","TK_ENDRECORD","TK_ELSE"};

int syntactic_error = 0;

extern char * buff, * block;
extern FILE * fp;

int getStream(FILE *fp, char * block, int k){
	counter=0;
	memset(block,'\0',1024);
	return fread(block,1,k,fp);
}



tokenlist *  getNextToken( char * block,tokenlist * tl){
	int state=0;
	static int i=0,lineNo=1;
	tl->t.tokenName[0]=tl->t.lexeme[0]='\0';

    if(bytesread==-1){
		bytesread=getStream(fp,block,1024);		
	}

	while(bytesread!=0){
	while(bytesread!=counter){


	buff[i++]=block[counter];
	switch(state){

		case 0:
			if(block[counter]>='0'&&block[counter]<='9'){
				state=1;counter++;
			}
			else if(block[counter]>='b'&&block[counter]<='d'){
				state=31;counter++;
			}
			else if(block[counter]>='a'&&block[counter]<='z'){
				state=35;counter++;
			}
			else if(block[counter]=='@'){
				state=5;counter++;
			}
			else if(block[counter]=='&'){
				state=6;counter++;
			}
			else if(block[counter]=='~'){
				//tk_not
				strcpy(tl->t.tokenName,"TK_NOT");
				strcpy(tl->t.lexeme,"~");
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='>'){
				state=8;counter++;
			}
			else if(block[counter]=='='){
				state=9;counter++;
			}
			else if(block[counter]=='!'){
				state=10;counter++;
			}
			else if(block[counter]=='+'){
				//tk_plus
				strcpy(tl->t.tokenName,"TK_PLUS");
				strcpy(tl->t.lexeme,"+");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='-'){
				//tk_minus
				strcpy(tl->t.tokenName,"TK_MINUS");
				strcpy(tl->t.lexeme,"-");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='*'){
				//tk_mul
				strcpy(tl->t.tokenName,"TK_MUL");
				strcpy(tl->t.lexeme,"*");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='/'){
				//tk_div
				strcpy(tl->t.tokenName,"TK_DIV");
				strcpy(tl->t.lexeme,"/");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='%'){
				//tk_comment
				while(block[counter]!='\n'&&counter!=bytesread){
                    counter++;
				}
				lineNo++;
				strcpy(tl->t.tokenName,"TK_COMMENT");
				strcpy(tl->t.lexeme,"\%");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='['){
				//tk_sql
				strcpy(tl->t.tokenName,"TK_SQL");
				strcpy(tl->t.lexeme,"[");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]==']'){
				//tk_sqr
				strcpy(tl->t.tokenName,"TK_SQR");
				strcpy(tl->t.lexeme,"]");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]==';'){
				//tk_sem
				strcpy(tl->t.tokenName,"TK_SEM");
				strcpy(tl->t.lexeme,";");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]==':'){
				//tk_colon
				strcpy(tl->t.tokenName,"TK_COLON");
				strcpy(tl->t.lexeme,":");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]==','){
				//tk_comma
				strcpy(tl->t.tokenName,"TK_COMMA");
				strcpy(tl->t.lexeme,",");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='_'){
				state=20;counter++;
			}
			else if(block[counter]=='<'){
				state=21;counter++;
			}
			else if(block[counter]=='#'){
				state=22;counter++;
			}
			else if(block[counter]=='('){
				//tk_op
				strcpy(tl->t.tokenName,"TK_OP");
				strcpy(tl->t.lexeme,"(");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]==')'){
				//tk_cl
				strcpy(tl->t.tokenName,"TK_CL");
				strcpy(tl->t.lexeme,")");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]=='.'){
				//tk_dot
				strcpy(tl->t.tokenName,"TK_DOT");
				strcpy(tl->t.lexeme,".");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else if(block[counter]==' '||block[counter]=='\t'){
				i=0;counter++;
			}
			else if(block[counter]=='\r'){
				lineNo++;
				//printf("%d\n",lineNo);
				i=0;counter+=2;
			}
			else if(block[counter]=='\n'){
				lineNo++;counter++;i=0;
			}
			else{
				printf("ERROR_2: Unknown Symbol <%c> at line:%d\n",block[counter],lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno = lineNo;
				//error
				i=0;counter++;
				syntactic_error = 1;
				return tl;
			}
		break;


		case 1:
			if(block[counter]>='0'&&block[counter]<='9'){
				state=1;counter++;
			}
			else if(block[counter]=='.'){
				state=36;counter++;
			}
			else{
				buff[i-1]='\0';
				strcpy(tl->t.tokenName,"TK_NUM");
				strcpy(tl->t.lexeme,buff);
				
				tl->lineno=lineNo;
				state=0;i=0;
				return tl;
				//TK_NUM
			}
		break;
		case 36:
			if(block[counter]>='0'&&block[counter]<='9'){
				state=50;counter++;
			}
			else {
				//error
				buff[i-1]='0';
				printf("ERROR_4: Invalid number format <%s> at line:<%d>\n",buff,lineNo);
				buff[i]='0';
				buff[i+1]='\0';
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;
		case 50:
		    buff[i-1]='\0';
			if(block[counter]>='0'&&block[counter]<='9'){
			    //tk_rnum
				strcpy(tl->t.tokenName,"TK_RNUM");
				strcpy(tl->t.lexeme,buff);
				
				tl->lineno=lineNo;
				counter++;i=0;
				return tl;
			}
			else{
				//error
				printf("ERROR_4: Invalid number format <%s> at line:<%d>\n",buff,lineNo);
				buff[i-1]='0';
				buff[i]='\0';
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
			state=0;i=0;
		break;


		case 31:
			if(block[counter]>='2'&&block[counter]<='7'){
				state=63;counter++;
			}
			else{
				state=35;i--;//tk_fieldid
			}
		break;
		case 63:
			if(block[counter]>='a'&&block[counter]<='z'){
				state=63;counter++;
			}
			else {
				state=64;i--;
			}
		break;
		case 64:
			if(block[counter]>='2'&&block[counter]<='7'){
				state=64;counter++;
			}
			else{
				//tk_id
				buff[i-1]='\0';
				if(i>=21){
					buff[20] = '\0';
					printf("ERROR_1: Identifier at line <%d> is longer than the prescribed length of 20 characters\n",lineNo);
					strcpy(tl->t.tokenName,"TK_ERROR");
					strcpy(tl->t.lexeme,"error");
					syntactic_error = 1;
				}	
				else
				{
				strcpy(tl->t.tokenName,"TK_ID");
				strcpy(tl->t.lexeme,buff);
				}
				tl->lineno=lineNo;
				i=0;
				return tl;
					

				state=0;
			}
		break;


		case 35:
			if(block[counter]>='a'&&block[counter]<='z'){
				state=35;counter++;
			}
			else{
				int j;buff[i-1]='\0';
				for(j=0;j<25;j++){
					if(strcmp(keywrds[j],buff)==0){
						strcpy(tl->t.tokenName,tk_keywrds[j]);
						strcpy(tl->t.lexeme,keywrds[j]);
						
						tl->lineno=lineNo;
						i=0;
						/*if(strcmp("end",buff)==0){
							printf("HI\n");
						}*/
						return tl;
						break;
					}
				}
				//check if keyword
				// else tk_fieldid
				if(j==25){
					buff[i-1]='\0';
					strcpy(tl->t.tokenName,"TK_FIELDID");
					strcpy(tl->t.lexeme,buff);
					
					tl->lineno=lineNo;
					i=0;
					return tl;
				}
				state=0;i=0;
			}
		break;


		case 5:
			if(block[counter]=='@'){
				state=41;counter++;
			}
			else{
				//error;
				printf("ERROR_2: Unknown Symbol <@> at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;
		case 41:
			if(block[counter]=='@'){
				//tk_or
				strcpy(tl->t.tokenName,"TK_OR");
				strcpy(tl->t.lexeme,"@@@");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else{
				//error;
				printf("ERROR_2: Unknown Symbol <@@> at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;


		case 6:
			if(block[counter]=='&'){
				state=42;counter++;
			}
			else{
				//error;
				printf("ERROR_2: Unknown Symbol <&> at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;
		case 42:
			if(block[counter]=='&'){
				//tk_and
				strcpy(tl->t.tokenName,"TK_AND");
				strcpy(tl->t.lexeme,"&&&");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else{
				//error;
				printf("ERROR_2: Unknown Symbol <&&> at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;


		case 8:
			if(block[counter]=='='){
				//tk_ge
				strcpy(tl->t.tokenName,"TK_GE");
				strcpy(tl->t.lexeme,">=");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
			}
			else{
				state=0;i=0;
				//tk_gt
				strcpy(tl->t.tokenName,"TK_GT");
				strcpy(tl->t.lexeme,">");
				
				tl->lineno=lineNo;
			}
			return tl;
		break;


		case 9:
			if(block[counter]=='='){
				//tk_eq
				strcpy(tl->t.tokenName,"TK_EQ");
				strcpy(tl->t.lexeme,"==");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else{
				//error;
				printf("ERROR_2: Unknown Symbol <=> at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;


		case 10:
			if(block[counter]=='='){
				//tk_ne
				strcpy(tl->t.tokenName,"TK_NE");
				strcpy(tl->t.lexeme,"!=");
				
				tl->lineno=lineNo;
				state=0;counter++;i=0;
				return tl;
			}
			else{
				//error;
				printf("ERROR_2: Unknown Symbol <!> at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;


		case 20:
			if((block[counter]>='a'&&block[counter]<='z')||(block[counter]>='A'&&block[counter]<='Z')){
				state=47;counter++;
			}
			else{
				//error
				printf("ERROR_2: Unknown Symbol <_'>at line:<%d>\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;
		case 47:
			if((block[counter]>='a'&&block[counter]<='z')||(block[counter]>='A'&&block[counter]<='Z')||(block[counter]>='0'&&block[counter]<='9')){
				state=47;counter++;
			}
			else{
				//tk_funid
				buff[i-1]='\0';
				if(strcmp(buff,"_main")==0){
					strcpy(tl->t.tokenName,"TK_MAIN");
					strcpy(tl->t.lexeme,"_main");
					
					tl->lineno=lineNo;
					i=0;
					return tl;
				}
				else{
					if(i>=31){
						printf("ERROR_1: Function identifier at line %d is longer than the prescribed length of 30 characters\n",lineNo);
						buff[30]='\0';
						strcpy(tl->t.tokenName,"TK_ERROR");
						strcpy(tl->t.lexeme,"error");
						tl->lineno=lineNo;
						syntactic_error = 1;
						return tl;
					}
					strcpy(tl->t.tokenName,"TK_FUNID");
					strcpy(tl->t.lexeme,buff);
					
					tl->lineno=lineNo;
					return tl;
				}
				state=0;i=0;
				
			}
		break;



		case 21:
			if(block[counter]=='='){
				//tk_le
				strcpy(tl->t.tokenName,"TK_LE");
				strcpy(tl->t.lexeme,"<=");
				
				tl->lineno=lineNo;
				state=0;i=0;counter++;
				return tl;
			}
			else if(block[counter]=='-'){
				state=48;counter++;
			}
			else{
				//tk_lt
				strcpy(tl->t.tokenName,"TK_LT");
				strcpy(tl->t.lexeme,"<");
				
				tl->lineno=lineNo;
				state=0;i=0;
				return tl;
			}
		break;
		case 48:
			if(block[counter]=='-'){
				state=65;counter++;
			}
			else{
				//error
				printf("ERROR_2: Unknown Symbol <<-> at line:%d\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;
		case 65:
			if(block[counter]=='-'){
				//tk_assignop
				strcpy(tl->t.tokenName,"TK_ASSIGNOP");
				strcpy(tl->t.lexeme,"<---");
				
				tl->lineno=lineNo;
				counter++;state=0;i=0;
				return tl;
			}
			else{
				//error
				printf("ERROR_2: Unknown Symbol <<--> at line:%d\n",lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;

		case 22:
			if(block[counter]>='a'&&block[counter]<='z'){
				state=49;counter++;
			}
			else{
				//error
				buff[i-1]='\0';
				printf("ERROR_2: Unknown Symbol <%s> at line:%d\n",buff,lineNo);
				strcpy(tl->t.tokenName,"TK_ERROR");
				strcpy(tl->t.lexeme,"error");
				tl->lineno=lineNo;
				state=0;i=0;
				syntactic_error = 1;
				return tl;
			}
		break;
		case 49:
			if(block[counter]>='a'&&block[counter]<='z'){
				state=49;counter++;
			}
			else{
				//tk_recordid
				buff[i-1]='\0';
				strcpy(tl->t.tokenName,"TK_RECORDID");
				strcpy(tl->t.lexeme,buff);
				
				tl->lineno=lineNo;
				state=0;i=0;
				return tl;
			}
		break;

	}
	}
	bytesread=getStream(fp,block,1024);
	}
    lineNo = 0;
    return tl;
}