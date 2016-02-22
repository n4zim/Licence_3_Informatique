#include <stdio.h>
#include <stdlib.h>
#include "premiers.h"
#include "suivants.h"
#include "analyseur_lexical.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {
  int uniteCourante;
//  int yylval;
  char nom[100];
  char valeur[100];
  
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }

/* uniteCourante = yylex();
  while (uniteCourante != FIN) {
    nom_token( uniteCourante, nom, valeur );
    printf( "%s %d\n", yytext, uniteCourante );
    //printf("%s\t%s\t%s\n", yytext, nom, valeur);
    uniteCourante = yylex();
  }

  return 0;
*/
  initialise_premiers();
  initialise_suivants();

  yylval = yylex();

  PG();

  return 0;
}
