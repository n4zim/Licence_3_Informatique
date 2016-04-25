#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "symboles.h"
#include "syntabs.h"
#include "analyseur_syntaxique.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {  
  int uniteCourante;
  //char nom[100];
  //char valeur[100];

  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
	fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
	exit(1);
  }
  //do {
	uniteCourante = yylex();
  analyseur_syntaxique(uniteCourante);
	//nom_token( uniteCourante, nom, valeur );
	//printf("%s\t%s\t%s\n", yytext, nom, valeur);
  //} while (uniteCourante != FIN); return 0;

  //test_yylex_internal(yyin); 
  return 0;
}
	
