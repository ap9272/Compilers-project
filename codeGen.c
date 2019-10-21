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
#include"codeGen.h"


int lblcount=1;

int ArithmeticExpressionGenerator(ParseTreeNode *ptNode,FILE *fp)
{
	int t1,t2;
	if(!strcmp(ptNode->NodeSymbol,"<arithmeticExpression>"))
	{
		t1 = ArithmeticExpressionGenerator(ptNode->child, fp);
		// fprintf(fp,"t1:%d\n",t1);
		t2 = ArithmeticExpressionGenerator(ptNode->child->sibling,fp);
		// fprintf(fp,"t2:%d\n",t2);
		if(t2)
		{
			while(t1--)
			{
				fprintf(fp,"POP\tBX\n");
				fprintf(fp,"ADD\tSP,%d\n",2*(t2-1));
				fprintf(fp,"POP\tAX\n");
				if(!strcmp(ptNode->child->sibling->child->child->token,"TK_PLUS"))
				{
					fprintf(fp,"ADD\tAX,BX\n");
				}
				else
				{
					// printf("1%s\n", ptNode->child->sibling->child->child->token);
					fprintf(fp,"SUB\tAX,BX\n");
				}
				fprintf(fp,"PUSH\tAX\n");
				fprintf(fp,"SUB\tSP,%d\n",2*(t2-1));
			}
			return t2;
		}
		return t1;
	}
	else if(!strcmp(ptNode->NodeSymbol,"<term>"))
	{
		t1 = ArithmeticExpressionGenerator(ptNode->child,fp);
		t2 = ArithmeticExpressionGenerator(ptNode->child->sibling,fp);
		if(t2)
		{
			if(t1 == 1)
			{
				fprintf(fp,"ADD\tSP,%d\n", 2*t2);
				fprintf(fp,"POP\tCX\n");
				fprintf(fp,"SUB\tSP,%d\n", 2*(t2+1));
				fprintf(fp,"POP\tBX\n");
				t2--;
				while(t2--)
				{
					fprintf(fp,"MOV\tAX,CX\n");
					if(!strcmp(ptNode->child->child->token,"TK_MUL"))
					{
						fprintf(fp,"MUL\tBX\n");
					}
					else
					{
						fprintf(fp,"MOV\tDX,0\n");
						fprintf(fp,"DIV\tBX\n");
					}
					fprintf(fp,"POP\tBX\n");
					fprintf(fp,"PUSH\tAX\n");
					fprintf(fp,"ADD\tSP,2\n");
				}
				fprintf(fp,"MOV\tAX,CX\n");
				if(!strcmp(ptNode->child->child->token,"TK_MUL"))
				{
					fprintf(fp,"MUL\tBX\n");
				}
				else
				{
					fprintf(fp,"MOV\tDX,0\n");
					fprintf(fp,"DIV\tBX\n");
				}
				fprintf(fp,"POP\tBX\n");
				fprintf(fp,"PUSH\tAX\n");
			}
			else
			{
				while(t1--)
				{
					fprintf(fp,"POP\tCX\n");
					fprintf(fp,"POP\tBX\n");
					fprintf(fp,"MOV\tAX,CX\n");
					if(!strcmp(ptNode->child->sibling->child->child->token,"TK_MUL"))
					{
						fprintf(fp,"MUL\tBX\n");
					}
					else
					{
						fprintf(fp,"MOV\tDX,0\n");
						fprintf(fp,"DIV\tBX\n");
					}
					fprintf(fp,"PUSH\tAX\n");
					fprintf(fp,"ADD\tSP,2\n");
				}
				fprintf(fp,"SUB\tSP,%d\n", 2*t1);
			}
		}
		if(t1 > t2)
		{
			return t1;
		}
		else
		{
			return t2;
		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<expPrime>"))
	{
		if(ptNode->child == NULL)
		{
			return 0;
		}
		t1 = ArithmeticExpressionGenerator(ptNode->child->sibling,fp);
		// fprintf(fp,"expt1:%d\n",t1);
		t2 = ArithmeticExpressionGenerator(ptNode->child->sibling->sibling,fp);
		// fprintf(fp,"expt1:%d\n",t1);
		if(t2)
		{
			while(t1--)
			{
				fprintf(fp,"POP\tBX\n");
				fprintf(fp,"ADD\tSP,%d\n",2*(t2-1));
				fprintf(fp,"POP\tAX\n");
				if(!strcmp(ptNode->child->child->token,"TK_PLUS"))
				{
					fprintf(fp,"ADD\tAX,BX\n");
				}
				else
				{
					// printf("2%s\n", ptNode->child->child->token);
					fprintf(fp,"SUB\tAX,BX\n");
				}
				fprintf(fp,"PUSH\tAX\n");
				fprintf(fp,"SUB\tSP,%d\n",2*(t2-1));
			}
			return t2;
		}
		return t1;
	}
	else if(!strcmp(ptNode->NodeSymbol,"<factor>"))
	{
		if(!strcmp(ptNode->child->token,"TK_ID"))
		{
			if(ptNode->child->sibling->child == NULL)
			{
				Identifiers *id = ptNode->child->identifierPtr;
				if(!strcmp(id->type,"int"))
				{
					fprintf(fp,"MOV\tAX,[%s]\n", ptNode->child->lexemeCurrentNode);
					fprintf(fp,"PUSH\tAX\n");
					return 1;	
				}
				else
				{
					int noOfFields = 0;
					char fullVarName[30];
					Records *records = id->records;
					while(records != NULL)
					{
						strcpy(fullVarName,ptNode->child->lexemeCurrentNode);
						strcat(fullVarName,".");
						strcat(fullVarName,records->IDname);
						fprintf(fp,"MOV\tAX,[%s]\n", fullVarName);
						fprintf(fp,"PUSH\tAX\n");
						noOfFields++;

						records = records->next;
					}

					return noOfFields;
				}
			}
			else
			{
				char fullVarName[30];
				strcpy(fullVarName,ptNode->child->lexemeCurrentNode);
				strcat(fullVarName,".");
				strcat(fullVarName,ptNode->child->sibling->child->lexemeCurrentNode);
				fprintf(fp,"MOV\tAX,[%s]\n", fullVarName);
				fprintf(fp,"PUSH\tAX\n");
				return 1;
			}
		}
		else if(!strcmp(ptNode->child->token,"TK_NUM"))
		{
			fprintf(fp,"MOV\tAX,%s\n", ptNode->child->valuelfNumber);
			fprintf(fp,"PUSH\tAX\n");
			return 1;
		}
		else if(!strcmp(ptNode->child->NodeSymbol,"<term>"))
		{
			t1 = ArithmeticExpressionGenerator(ptNode->child,fp);
			t2 = ArithmeticExpressionGenerator(ptNode->child->sibling,fp);
			if(t2)
			{
				while(t1--)
				{
					fprintf(fp,"POP\tBX\n");
					fprintf(fp,"ADD\tSP,%d\n",2*(t2-1));
					fprintf(fp,"POP\tAX\n");
					if(!strcmp(ptNode->child->sibling->child->child->token,"TK_PLUS"))
					{
						fprintf(fp,"ADD\tAX,BX\n");
					}
					else
					{
						// printf("3%s\n", ptNode->child->child->token);
						fprintf(fp,"SUB\tAX,BX\n");
					}
					fprintf(fp,"PUSH\tAX\n");
					fprintf(fp,"SUB\tSP,%d\n",2*(t2-1));
				}
				return t2;
			}
			return t1;
		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<termPrime>"))
	{
		if(ptNode->child == NULL)
		{
			return 0;
		}
		t1 = ArithmeticExpressionGenerator(ptNode->child->sibling,fp);
		t2 = ArithmeticExpressionGenerator(ptNode->child->sibling->sibling,fp);
		if(t2)
		{
			if(t1 == 1)
			{
				fprintf(fp,"ADD\tSP,%d\n", 2*t2);
				fprintf(fp,"POP\tCX\n");
				fprintf(fp,"SUB\tSP,%d\n", 2*(t2+1));
				fprintf(fp,"POP\tBX\n");
				t2--;
				while(t2--)
				{
					fprintf(fp,"MOV\tAX,CX\n");
					if(!strcmp(ptNode->child->child->token,"TK_MUL"))
					{
						fprintf(fp,"MUL\tBX\n");
					}
					else
					{
						fprintf(fp,"MOV\tDX,0\n");
						fprintf(fp,"DIV\tBX\n");
					}
					fprintf(fp,"POP\tBX\n");
					fprintf(fp,"PUSH\tAX\n");
					fprintf(fp,"ADD\tSP,2\n");
				}
				fprintf(fp,"MOV\tAX,CX\n");
				if(!strcmp(ptNode->child->child->token,"TK_MUL"))
				{
					fprintf(fp,"MUL\tBX\n");
				}
				else
				{
					fprintf(fp,"MOV\tDX,0\n");
					fprintf(fp,"DIV\tBX\n");
				}
				fprintf(fp,"POP\tBX\n");
				fprintf(fp,"PUSH\tAX\n");
			}
			else
			{
				while(t1--)
				{
					fprintf(fp,"POP\tCX\n");
					fprintf(fp,"POP\tBX\n");
					fprintf(fp,"MOV\tAX,CX\n");
					if(!strcmp(ptNode->child->child->token,"TK_MUL"))
					{
						fprintf(fp,"MUL\tBX\n");
					}
					else
					{
						fprintf(fp,"MOV\tDX,0\n");
						fprintf(fp,"DIV\tBX\n");
					}
					fprintf(fp,"PUSH\tAX\n");
					fprintf(fp,"ADD\tSP,2\n");
				}
				fprintf(fp,"SUB\tSP,%d\n", 2*t1);
			}
		}
		if(t1 > t2)
		{
			return t1;
		}
		else
		{
			return t2;
		}
	}

	return -1;
}

void AssignmentGenerator(ParseTreeNode *ptNode,FILE *fp)
{
	if(!strcmp(ptNode->NodeSymbol,"<assignmentStmt>"))
	{
		ptNode = ptNode->child;
		fprintf(fp,"PUSHA\n");
		if(ptNode->child->sibling->child == NULL)
		{
			Identifiers *id = ptNode->child->identifierPtr;
			if(!strcmp(id->type,"int"))
			{
				if(ArithmeticExpressionGenerator(ptNode->sibling->sibling,fp) != 1)
				{
					fprintf(fp,"Error\n");
				}
				fprintf(fp,"POP\tAX\n");
				fprintf(fp,"MOV\t[%s],AX\n", id->IDname);
			}
			else
			{
				int noOfFields = ArithmeticExpressionGenerator(ptNode->sibling->sibling,fp);
				char fullVarName[30];
				Records *records = id->records;
				fprintf(fp,"ADD\tSP,%d\n", 2*noOfFields);
				while(records != NULL)
				{
					strcpy(fullVarName,ptNode->child->lexemeCurrentNode);
					strcat(fullVarName,".");
					strcat(fullVarName,records->IDname);
					fprintf(fp,"SUB\tSP,2\n");
					fprintf(fp,"POP\tAX\n");
					fprintf(fp,"MOV\t[%s],AX\n", fullVarName);
					fprintf(fp,"SUB\tSP,2\n");

					records = records->next;
				}
				fprintf(fp,"ADD\tSP,%d\n", 2*noOfFields);
			}
		}
		else
		{
			if(ArithmeticExpressionGenerator(ptNode->sibling->sibling,fp) != 1)
			{
				fprintf(fp,"Error\n");
			}
			char fullVarName[30];
			strcpy(fullVarName,ptNode->child->lexemeCurrentNode);
			strcat(fullVarName,".");
			strcat(fullVarName,ptNode->child->sibling->child->lexemeCurrentNode);
			fprintf(fp,"POP\tAX\n");
			fprintf(fp,"MOV\t[%s],AX\n", fullVarName);
		}
		fprintf(fp,"POPA\n");
	}
}



void boolCodeGen(ParseTreeNode * ptNode,FILE *fp)
{
	if(!strcmp(ptNode->child->NodeSymbol,"<var>") ){
		if(!strcmp(ptNode->child->child->token,"TK_ID") ){
			fprintf(fp,"mov ax, [%s]\n",ptNode->child->child->lexemeCurrentNode);
		}
		else{
			fprintf(fp,"mov ax, %sd\n",ptNode->child->child->lexemeCurrentNode);	
		}

		if(!strcmp(ptNode->child->sibling->sibling->child->token,"TK_ID") ){
			fprintf(fp,"mov dx, [%s]\n",ptNode->child->sibling->sibling->child->lexemeCurrentNode);
		}
		else{
			fprintf(fp,"mov dx, %sd\n",ptNode->child->sibling->sibling->child->valuelfNumber);	
		}
		char a[][6]={"TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE"};
		char b[][4]={"jge","jg","jne","jle","jl","je"};
		int i;

		fprintf(fp,"sub ax, dx\n");
		fprintf(fp,"mov ax, 0d\n");

		for(i=0;i<6;i++){
			if(!strcmp(a[i],ptNode->child->sibling->child->token)){
				fprintf(fp,"%s lbl%d\n",b[i],lblcount);
				break;
			}
		}
		fprintf(fp,"mov ax, 1d\n");
		fprintf(fp,"lbl%d: push ax\n",lblcount);lblcount++;


	}

	else if(!strcmp(ptNode->child->token,"TK_NOT") ){
		boolCodeGen(ptNode->child->sibling,fp);
		fprintf(fp,"pop dx\n");
		fprintf(fp,"mov ax, 1d\n");
		fprintf(fp,"sub ax, dx\n");
		fprintf(fp,"push ax\n");
	}

	else if(!strcmp(ptNode->child->sibling->child->token,"TK_AND") ){
		boolCodeGen(ptNode->child,fp);
		boolCodeGen(ptNode->child->sibling->sibling,fp);
		fprintf(fp,"pop dx\n");
		fprintf(fp,"pop ax\n");
		fprintf(fp,"and ax,dx\n");
		fprintf(fp,"push ax\n");
	}
	else if(!strcmp(ptNode->child->sibling->child->token,"TK_OR") ){
		boolCodeGen(ptNode->child,fp);
		boolCodeGen(ptNode->child->sibling->sibling,fp);
		fprintf(fp,"pop dx\n");
		fprintf(fp,"pop ax\n");
		fprintf(fp,"or ax,dx\n");
		fprintf(fp,"push ax\n");	
	}
}

void codegen(ParseTreeNode *ptNode,FILE *fp)
{
	//fprintf(fp,".data\n");
	if(!strcmp(ptNode->NodeSymbol,"<declaration>") ){
		ParseTreeNode * name=ptNode->child->sibling->sibling;
		if(!strcmp(ptNode->child->sibling->child->token,"TK_INT") ){
			fprintf(fp,"%s\tdd\t0\n",name->lexemeCurrentNode);
		}
		else {

			Identifiers * ids=name->identifierPtr;
			Records * rec=ids->records;

			while(rec != NULL)
			{
				fprintf(fp,"%s.%s\tdd\t0\n",name->lexemeCurrentNode,rec->IDname);
				rec = rec->next;
			}

		}
	}
	else if(!strcmp(ptNode->NodeSymbol,"<declarations>")&&ptNode->child==NULL ){
		fprintf(fp,"Input\tdb\t\"%%d\",0\n");
		fprintf(fp,"Output\tdb\t\"%%d\",10,0\n");
		fprintf(fp, "temp\tdd\t0\n");
		fprintf(fp,"\nSECTION .text\n");
		fprintf(fp,"global  main\n");
		fprintf(fp,"\nmain:\n\n");

	}
	else if(!strcmp(ptNode->NodeSymbol,"<conditionalStmt>") ){
		boolCodeGen(ptNode->child->sibling,fp);
		fprintf(fp,"pop ax\n");
		fprintf(fp,"cmp ax,0d\n");
		fprintf(fp,"je lbl%d\n",lblcount);
		int x=lblcount;lblcount++;
		ParseTreeNode * temp=ptNode->child->sibling->sibling;
		while(strcmp(temp->NodeSymbol,"<elsePart>") ){
			codegen(temp,fp);
			temp=temp->sibling;
		}
		fprintf(fp,"jmp lbl%d\n", lblcount);
		int y = lblcount;lblcount++;
		fprintf(fp,"lbl%d: ",x);
		codegen(temp,fp);
		fprintf(fp,"lbl%d: ",y);
	}
	else if(!strcmp(ptNode->NodeSymbol,"<iterativeStmt>") ){
		fprintf(fp,"lbl%d: ",lblcount);
		int y=lblcount;lblcount++;
		boolCodeGen(ptNode->child->sibling,fp);
		fprintf(fp,"pop ax\n");
		fprintf(fp,"cmp ax,0d\n");
		fprintf(fp,"je lbl%d\n",lblcount);
		int x=lblcount;lblcount++;
		ParseTreeNode * temp=ptNode->child->sibling->sibling;
		while(temp!=NULL ){
			codegen(temp,fp);
			temp=temp->sibling;
		}
		fprintf(fp,"jmp lbl%d\n",y);
		fprintf(fp,"lbl%d: ",x);
	}
	else if(!strcmp(ptNode->NodeSymbol,"<assignmentStmt>")){
		AssignmentGenerator(ptNode,fp);
	}
	else if(!strcmp(ptNode->NodeSymbol,"<ioStmt>")){
		ParseTreeNode * temp=ptNode->child->sibling;
		if(!strcmp(ptNode->child->token,"TK_READ") ){
			if(temp->child->sibling->child==NULL){
				Identifiers *ids = temp->child->identifierPtr;
				if(!strcmp(ids->type,"int"))
				{
					fprintf(fp,"mov ecx, %s\n",temp->child->lexemeCurrentNode);
					fprintf(fp,"push ecx\n");
					fprintf(fp,"push dword Input\n");
					fprintf(fp,"call scanf\n");
				}
				else
				{
					char fullVarName[30];
					Records *records = ids->records;

					while(records != NULL)
					{
						strcpy(fullVarName,ids->IDname);
						strcat(fullVarName,".");
						strcat(fullVarName,records->IDname);
						fprintf(fp,"mov ecx, %s\n", fullVarName);
						fprintf(fp,"push ecx\n");
						fprintf(fp,"push dword Input\n");
						fprintf(fp,"call scanf\n");

						records = records->next;
					}
				}
			}
			else{
				fprintf(fp,"mov ecx, %s.%s\n",temp->child->lexemeCurrentNode,temp->child->sibling->child->lexemeCurrentNode);
				fprintf(fp,"push ecx\n");
				fprintf(fp,"push dword Input\n");
				fprintf(fp,"call scanf\n");
			}
		}
		else{
			if(!strcmp(temp->child->token,"TK_ID")){
    			if(temp->child->sibling->child==NULL){
    				Identifiers *ids = temp->child->identifierPtr;
					if(!strcmp(ids->type,"int"))
					{
						fprintf(fp,"mov eax, 0d\n");
						fprintf(fp,"mov ax, [%s]\n",temp->child->lexemeCurrentNode);
						fprintf(fp,"mov bx, ax\n");
						fprintf(fp,"and ax, 08000h\n");
						fprintf(fp,"jz lbl%d\n",lblcount);
						fprintf(fp,"mov eax, 0ffffffffh\n");
						fprintf(fp,"lbl%d: mov ax, bx\n",lblcount);lblcount++;
						fprintf(fp,"push eax\npush dword Output\ncall printf\n");
						/*fprintf(fp,"mov ecx, dword [%s]\n",temp->child->lexemeCurrentNode);
						fprintf(fp,"push ecx\n");
						fprintf(fp,"push dword Output\n");
						fprintf(fp,"call printf\n");*/
					}
					else
					{
						char fullVarName[30];
						Records *records = ids->records;

						while(records != NULL)
						{
							strcpy(fullVarName,ids->IDname);
							strcat(fullVarName,".");
							strcat(fullVarName,records->IDname);
							fprintf(fp,"mov eax, 0d\n");
							fprintf(fp,"mov ax, [%s]\n",fullVarName);
							fprintf(fp,"mov bx, ax\n");
							fprintf(fp,"and ax, 08000h\n");
							fprintf(fp,"jz lbl%d\n",lblcount);
							fprintf(fp,"mov eax, 0ffffffffh\n");
							fprintf(fp,"lbl%d: mov ax, bx\n",lblcount);lblcount++;
							fprintf(fp,"push eax\npush dword Output\ncall printf\n");
							/*fprintf(fp,"mov ecx, dword [%s]\n", fullVarName);
							fprintf(fp,"push ecx\n");
							fprintf(fp,"push dword Output\n");
							fprintf(fp,"call printf\n");*/

							records = records->next;
						}
					}
				}
				else{
					fprintf(fp,"mov eax, 0d\n");
					fprintf(fp,"mov ax, [%s.%s]\n",temp->child->lexemeCurrentNode,temp->child->sibling->child->lexemeCurrentNode);
					fprintf(fp,"mov bx, ax\n");	
					fprintf(fp,"and ax, 08000h\n");
					fprintf(fp,"jz lbl%d\n",lblcount);
					fprintf(fp,"mov eax, 0ffffffffh\n");
					fprintf(fp,"lbl%d: mov ax, bx\n",lblcount);lblcount++;
					fprintf(fp,"push eax\npush dword Output\ncall printf\n");
					/*fprintf(fp,"mov ecx, dword [%s.%s]\n",temp->child->lexemeCurrentNode,temp->child->sibling->child->lexemeCurrentNode);
					fprintf(fp,"push ecx\n");
					fprintf(fp,"push dword Output\n");
					fprintf(fp,"call printf\n");*/
				}
			}
			else{
				fprintf(fp,"mov ecx, %s\n",temp->child->valuelfNumber);
				fprintf(fp,"push ecx\n");
				fprintf(fp,"push dword Output\n");
				fprintf(fp,"call printf\n");
			}
    	}
	}
	else{
		ParseTreeNode *temp=ptNode->child;
		while(temp!=NULL){
			codegen(temp,fp);
			temp=temp->sibling;
		}
	}
}