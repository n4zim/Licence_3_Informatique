CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = util.o analyseur_syntaxique.o analyseur_lexical.o

all: test_yylex

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c $^

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
