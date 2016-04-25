#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {  
    //int uniteCourante;
    //char nom[100];
    //char valeur[100];

    if(argc <= 1) {
        fprintf(stderr, "Il manque un argument : le programme doit connaitre le fichier à analyser (./test_yylex Tests/[...])\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if(yyin == NULL){
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
        exit(1);
    }
	
    analyseur_syntaxique();
    
    /*do {
        uniteCourante = yylex();
        nom_token(uniteCourante, nom, valeur);
        printf("%s\t%s\t%s\n", yytext, nom, valeur);
    } while (uniteCourante != FIN);*/

    //test_yylex_internal(yyin); 
    return 0;
}
