CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = util.o premiers.o suivants.o analyseur_syntaxique.o analyseur_lexical.o syntabs.o dico.o parcours_mips.o

all: test_yylex

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c $^

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c $^
	
premiers.o: premiers.c
	$(CC) $(CCFLAGS) -c $^

suivants.o: suivants.c
	$(CC) $(CCFLAGS) -c $^

syntabs.o: syntabs.c
	$(CC) $(CCFLAGS) -c $^

parcours_mips.o: parcours_mips.c
	$(CC) $(CCFLAGS) -c $^

dico.o: dico.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
