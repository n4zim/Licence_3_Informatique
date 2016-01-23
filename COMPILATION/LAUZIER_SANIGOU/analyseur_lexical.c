#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "util.h"
#include <regex.h>

#define YYTEXT_MAX 100
#define is_num(c)(('0' <= (c)) && ((c) <= '9'))
#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c) || (c) == '_' || (c) == '$')
#define is_alphanum(c)(is_num((c)) || is_alpha((c)))
 
extern FILE *yyin;

char *tableMotsClefs[] = {
  "si", 
  "entier",
  "retour",
  "lire",
  "ecrire",

};

int codeMotClefs[] = { 
  SI, 
  ENTIER,
  RETOUR,
  LIRE,
  ECRIRE,
};

char yytext[YYTEXT_MAX];
int yyleng;
int nbMotsClefs = 9;
/* Compter les lignes pour afficher les messages d'erreur avec numero ligne */
int nb_ligne = 1;

/*******************************************************************************
 * Fonction qui ignore les espaces et commentaires. 
 * Renvoie -1 si arrivé à la fin du fichier, 0 si tout va bien 
 ******************************************************************************/
int mangeEspaces()
{ 
  char c = fgetc(yyin);
  int comment = 0;
  while( comment || (c == ' ') || (c == '\n') || (c == '\t') || (c == '#' ) ) {
    if( c == '#' ) {
        comment = 1;
    }
    if( c == '\n' ) {
      nb_ligne++;
      comment = 0;
    }
    c = fgetc(yyin);    
  }
  if ( feof(yyin) ) {
    return -1;
  }    
  ungetc(c, yyin);
  return 0;
}

/*******************************************************************************
 * Lit un caractère et le stocke dans le buffer yytext 
 ******************************************************************************/
char lireCar(void)
{
  yytext[yyleng++] = fgetc(yyin);
  yytext[yyleng] = '\0';
  return yytext[yyleng - 1];
}

/*******************************************************************************
 * Remet le dernier caractère lu au buffer clavier et enlève du buffer yytext 
 ******************************************************************************/
void delireCar()
{
  char c;
  c = yytext[yyleng - 1];
  yytext[--yyleng] = '\0';
  ungetc(c, yyin);
}
/*******************************************************************************
 * Fonction principale de l'analyseur lexical, lit les caractères de yyin et
 * renvoie les tokens sous forme d'entier. Le code de chaque unité est défini 
 * dans symboles.h sinon (mot clé, idententifiant, etc.). Pour les tokens de 
 * type ID_FCT, ID_VAR et NOMBRE la valeur du token est dans yytext, visible 
 * dans l'analyseur syntaxique.
 ******************************************************************************/
int yylex(void)
{
  char c;
  //int i;
  yytext[yyleng = 0] = '\0';

  if(mangeEspaces() == -1)  return 29;
  
  c = lireCar();

  if(c == ';')  return 1;

  //Opérateurs
  if(c == '+')  return 2;
  if(c == '-')  return 3;
  if(c == '*')  return 4;
  if(c == '/')  return 5;

  //Structure
  if(c == '(')  return 6;
  if(c == ')')  return 7;
  if(c == '[')  return 8;
  if(c == ']')  return 9;
  if(c == '{')  return 10;
  if(c == '}')  return 11;

  //Comparateurs
  if(c == '=')  return 12;
  if(c == '<')  return 13;
  if(c == '&')  return 14;
  if(c == '|')  return 15;
  if(c == '!')  return 16;

  regex_t regex;
  int retour;

  retour = regcomp(&regex, "^[a-zA-Z_$][0-9a-zA-Z_]*$", REG_NOSUB | REG_EXTENDED);
  if(retour)
  {
    fprintf(stderr, "Impossible de compiler la regex\n");
    exit(1);
  }

  int boucle = 0;
  int match = regexec(&regex, yytext, 0, NULL, 0);
  
  while(match != REG_NOMATCH)
  {
    lireCar();
    match = regexec(&regex, yytext, 0, NULL, 0);
    boucle = 1;
  }

  regfree(&regex);

  if(boucle == 1)
  {	
    delireCar();
  }  

  if(strcmp(yytext,"si") == 0)  return 17;
  if(strcmp(yytext,"alors") == 0) return 18;
  if(strcmp(yytext,"sinon") == 0) return 19;
  if(strcmp(yytext,"tantque") == 0) return 20;
  if(strcmp(yytext,"faire") == 0) return 21;
  if(strcmp(yytext,"entier") == 0)  return 22;
  if(strcmp(yytext,"retour") == 0) return 23;
  if(strcmp(yytext,"lire") == 0)  return 24;
  if(strcmp(yytext,"ecrire") == 0)  return 25;


  if(yytext[0] == '$') return 26;

  if(boucle && yytext[0] != '$') return 27;

   retour = regcomp(&regex, "[0-9]+$", REG_NOSUB | REG_EXTENDED);
   if(retour)
   {
     fprintf(stderr, "Impossible de compiler la regex\n");
     exit(1);
   }

   boucle = 0;
   match = regexec(&regex, yytext, 0, NULL, 0);
   while(match != REG_NOMATCH)
   {
     lireCar();
     match = regexec(&regex, yytext, 0, NULL, 0);
     boucle = 1;
   }

   regfree(&regex);

   if(boucle)
   {
     delireCar();
     return 28;
   }

  if(c == ',')  return 30;
  if(c == '>')  return 31;

  return -1;
}

/*******************************************************************************
 * Fonction auxiliaire appelée par l'analyseur syntaxique tout simplement pour 
 * afficher des messages d'erreur et l'arbre XML 
 ******************************************************************************/
void nom_token( int token, char *nom, char *valeur ) {
  int i;
  
  strcpy( nom, "symbole" );
  if(token == POINT_VIRGULE) strcpy( valeur, "POINT_VIRGULE");
  else if(token == PLUS) strcpy(valeur, "PLUS");
  else if(token == MOINS) strcpy(valeur, "MOINS");
  else if(token == FOIS) strcpy(valeur, "FOIS");
  else if(token == DIVISE) strcpy(valeur, "DIVISE");
  else if(token == PARENTHESE_OUVRANTE) strcpy(valeur, "PARENTHESE_OUVRANTE");
  else if(token == PARENTHESE_FERMANTE) strcpy(valeur, "PARENTHESE_FERMANTE");
  else if(token == CROCHET_OUVRANT) strcpy(valeur, "CROCHET_OUVRANT");
  else if(token == CROCHET_FERMANT) strcpy(valeur, "CROCHET_FERMANT");
  else if(token == ACCOLADE_OUVRANTE) strcpy(valeur, "ACCOLADE_OUVRANTE");
  else if(token == ACCOLADE_FERMANTE) strcpy(valeur, "ACCOLADE_FERMANTE");
  else if(token == EGAL) strcpy(valeur, "EGAL");
  else if(token == INFERIEUR) strcpy(valeur, "INFERIEUR");
  else if(token == SUPERIEUR) strcpy(valeur, "SUPERIEUR");
  else if(token == ET) strcpy(valeur, "ET");
  else if(token == OU) strcpy(valeur, "OU");
  else if(token == NON) strcpy(valeur, "NON");
  else if(token == FIN) strcpy(valeur, "FIN");
  else if(token == VIRGULE) strcpy(valeur, "VIRGULE");

  else if( token == ID_VAR ) {
    strcpy( nom, "id_variable" );  
    strcpy( valeur, yytext );        
  }
  else if( token == ID_FCT ) {
    strcpy( nom, "id_fonction" );    
    strcpy( valeur, yytext );    
  }
  else if( token == NOMBRE ) {
    strcpy( nom, "nombre" );
    strcpy( valeur, yytext ); 
  }
  else {
    strcpy( nom, "mot_clef" );
    for(i=0; i < nbMotsClefs; i++){
      if( token ==  codeMotClefs[i] ){
        strcpy( valeur, tableMotsClefs[i] );
        break;
      }
    }
  }  
}
/*******************************************************************************
 * Fonction auxiliaire appelée par le compilo en mode -l, pour tester l'analyseur
 * lexical et, étant donné un programme en entrée, afficher la liste des tokens.
 ******************************************************************************/

void test_yylex_internal(FILE *yyin) {
  int uniteCourante;
  char nom[100];
  char valeur[100];  
  do {
    uniteCourante = yylex();  
    nom_token( uniteCourante, nom, valeur );
    printf("%s\t%s\t%s\n", yytext, nom, valeur);
  } while (uniteCourante != FIN);
}
