# Batch number: 36
# Pratik Satish (2013A7PS057P)
# Anurag Prakash (2013A7PS061P)

CC = gcc

CFLAGS = -ggdb3 -O0 -std=c99 -Wall -Werror

EXE = toycompiler

HDRS = firstfollow.h helperfunctions.h lexer.h lexerDef.h parser.h parserDef.h symbolTableDef.h symbolTable.h typechecker.h semanticAnalysis.h codeGen.h

SRCS = 	driver.c firstfollow.c helperfunctions.c lexer.c parser.c symbolTable.c typechecker.c semanticAnalysis.c codeGen.c

OBJS = $(SRCS:.c=.o)

$(EXE): $(OBJS) $(HDRS) makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJS): $(HDRS) makefile

clean:
	rm -f $(EXE) *.o