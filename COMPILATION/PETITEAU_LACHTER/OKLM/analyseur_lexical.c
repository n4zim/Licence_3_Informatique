#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "util.h"

#include "premiers.h"
#include "suivants.h"

#define YYTEXT_MAX 100
#define is_num(c)(('0' <= (c)) && ((c) <= '9'))
#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c) || (c) == '_' || (c) == '$')
#define is_alphanum(c)(is_num((c)) || is_alpha((c)))
 
extern FILE *yyin;

char tableSymbole[] = {
    ';', '+', '-', '*', '/', '(', ')',
    '[', ']', '{', '}', '=', '<',
    '&', '|', '!', ',', '\0'
};

int codeSymbole[] = {
    POINT_VIRGULE, PLUS, MOINS, FOIS, DIVISE, PARENTHESE_OUVRANTE, PARENTHESE_FERMANTE,
    CROCHET_OUVRANT, CROCHET_FERMANT, ACCOLADE_OUVRANTE, ACCOLADE_FERMANTE, EGAL, INFERIEUR,
    ET, OU, NON, VIRGULE
};

char *tableMotsClefs[] = {
    "si", "alors", "sinon", "tantque", "faire", "entier", "retour", "lire", "ecrire", '\0'
};

int codeMotClefs[] = { 
    SI, ALORS, SINON, TANTQUE, FAIRE, ENTIER, RETOUR, LIRE, ECRIRE
};

char yytext[YYTEXT_MAX];
int yyleng;
int yylval;
int nbMotsClefs = 1;
/* Compter les lignes pour afficher les messages d'erreur avec numero ligne */
int nb_ligne = 1;

//int operateur_multichar( char c1, char c2, int token_multichar );

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
int lireCar(void)
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
 * renvoie les tokens sous forme d'entier.
 * Pour les tokens de type ID_FCT, ID_VAR et NOMBRE la
 * valeur du token est dans yytext, visible dans l'analyseur syntaxique.
 ******************************************************************************/


int yylex(void)
{
    char c;
    yytext[yyleng = 0] = '\0';

    mangeEspaces();
    if( feof( yyin ) ) return FIN;

    c = lireCar();

    // Symbole simple
    for( int i = 0; tableSymbole[i] != '\0'; ++i )
    {
    if( c != tableSymbole[i] ) continue;
     
    char tmp[2] = { 0 };
    tmp[0] = c;
    affiche_element( "symbole", tmp, 1 );
    return codeSymbole[i];
    }

    // Nombre
    if( isdigit( c ) )
    {
    do
    {
      lireCar();
    } 
    while( isdigit( yytext[yyleng - 1] ) );
    delireCar();

    affiche_element( "nombre", yytext, 1 );
    return NOMBRE;
    }

    // Mot clefs
    do
    {
    lireCar();
    }
    while( !isspace( yytext[ yyleng - 1 ] ) );
    delireCar();

    for( int i = 0; tableMotsClefs[i] != '\0'; ++i )
    {
    if( strcmp( tableMotsClefs[i], yytext ) != 0 ) continue;

    affiche_element( "mot-clef", yytext, 1 );
    return codeMotClefs[i];
    }

    for(; yyleng != 1; delireCar() );


    //id_var
    if( c == '$' )
    {
    do
    {
      lireCar();
    }
    while( !isspace( yytext[ yyleng - 1 ] ) && yytext[ yyleng - 1 ] != ';' && yytext[ yyleng - 1] != ',' && yytext[ yyleng - 1 ] != '[' );
    delireCar();
    
    affiche_element( "id_variable", yytext, 1 );
    return ID_VAR;
    }

    //id_fct
    if( isalpha( c ) || c == '_' )
    {
    do
    {
      lireCar();
    }
    while( yytext[ yyleng - 1 ] != '(' );
    delireCar();

    affiche_element( "id_fonction", yytext, 1 );
    return ID_FCT;
    }
    
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
    else if(token == ET) strcpy(valeur, "ET");
    else if(token == OU) strcpy(valeur, "OU");
    else if(token == NON) strcpy(valeur, "NON");
    else if(token == SI) strcpy(valeur, "SI");
    else if(token == ALORS) strcpy(valeur, "ALORS");
    else if(token == SINON) strcpy(valeur, "SINON");
    else if(token == TANTQUE) strcpy(valeur, "TANTQUE");
    else if(token == FAIRE) strcpy(valeur, "FAIRE");
    else if(token == ENTIER) strcpy(valeur, "ENTIER");
    else if(token == RETOUR) strcpy(valeur, "RETOUR");
    else if(token == LIRE) strcpy(valeur, "LIRE");
    else if(token == ECRIRE) strcpy(valeur, "ECRIRE");
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
    uniteCourante = yylex();
    while (uniteCourante != FIN) {
    printf("%s %d\n", yytext, uniteCourante);
    uniteCourante = yylex();
    }
    printf("%s %d\n", yytext, uniteCourante);
}

void error(const char * s) {
    printf("%s\n", s);
    exit(1);
}

void syntaxError() {
    printf("Erreur dans la syntaxe !\n");
    exit(1);
}

void PG() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(est_premier(_optDecVariables_, yylval)) {
      ODV();
      if(est_premier(_listeDecFonctions_, yylval)) LDF();
    }
    else if(est_premier(_listeDecFonctions_, yylval)) LDF();
    else syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void ODV() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(est_premier(_listeDecVariables_, yylval)) {
        LDV();
        if(yylval == POINT_VIRGULE) yylval = yylex();
          else error("';' été attendu");
    }
    if(!est_suivant(_optDecVariables_, yylval)) syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void LDV() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(est_premier(_declarationVariable_, yylval)) {
        DV();
        if(est_premier(_listeDecVariablesBis_, yylval)) LDVB();
    } else syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void LDVB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(yylval == VIRGULE) {
        yylval = yylex();
        if(est_premier(_declarationVariable_, yylval)) {
            DV();
            if(est_premier(_listeDecVariablesBis_, yylval)) LDVB();
        } else syntaxError();
    }
    if(!est_suivant(_listeDecVariablesBis_, yylval)) syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void DV() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(yylval == ENTIER) {
        yylval = yylex();
        if(yylval == ID_VAR) {
            yylval = yylex();
            if(est_premier(_optTailleTableau_, yylval)) OTT();
        } else error("Un identificateur de variable été attendu");
    } else error("'ENTIER' été attendu");
    affiche_balise_fermante(__FUNCTION__, 1);
}

void OTT() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(yylval == CROCHET_OUVRANT) {
        yylval = yylex();
        if(yylval == NOMBRE) {
            yylval = yylex();
            if(yylval == CROCHET_FERMANT) yylval = yylex();
                else error("']' été attendu");
        } else error("Un nombre été attendu");
    }
    if(!est_suivant(_optTailleTableau_, yylval)) syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void LDF() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(est_premier(_declarationFonction_, yylval)) {
        DF();
        if(est_premier(_listeDecFonctions_, yylval)) LDF();
    } else syntaxError();
    if(!est_suivant(_listeDecFonctions_, yylval)) syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void DF() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(yylval == ID_FCT) {
        yylval = yylex();
        if(est_premier(_listeParam_, yylval)) {
            LP();
            if(est_premier(_optDecVariables_, yylval)) ODV();
            if(est_premier(_instructionBloc_, yylval)) IB();
                else syntaxError();
        } else syntaxError();
    } else error("Un identificateur de fonction été attendu");
    affiche_balise_fermante(__FUNCTION__, 1);
}

void LP() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(yylval == PARENTHESE_OUVRANTE) {
        yylval = yylex();
        if(est_premier(_optListeDecVariables_, yylval)) OLDV();
        if(yylval == PARENTHESE_FERMANTE) yylval = yylex();
            else  error("')' été attendu");
    } else error("'(' été attendu");
    affiche_balise_fermante(__FUNCTION__, 1);
}

void OLDV() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(est_premier(_listeDecVariables_, yylval)) LDV();
    if(!est_suivant(_optListeDecVariables_, yylval)) syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void I() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    if(est_premier(_instructionAffect_, yylval)) IAFF();
        else if(est_premier(_instructionBloc_, yylval)) IB();
        else if(est_premier(_instructionSi_, yylval)) ISI();
        else if(est_premier(_instructionTantque_, yylval)) ITQ();
        else if(est_premier(_instructionAppel_, yylval)) IAPP();
        else if(est_premier(_instructionRetour_, yylval)) IRET();
        else if(est_premier(_instructionEcriture_, yylval)) IECR();
        else if(est_premier(_instructionVide_, yylval)) IVIDE();
        else syntaxError();
    affiche_balise_fermante(__FUNCTION__, 1);
}

void IAFF() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_var_, yylval)) {
    VAR();

    if(yylval == EGAL) {
      yylval = yylex();

      if(est_premier(_expression_, yylval)) {
        EXP();

        if(yylval == POINT_VIRGULE)
        {
        yylval = yylex();
      }
      else
      {
        error("';' été attendu");
      }
    }
    else
    {
    syntaxError();
}
} else {
    error("'=' été attendu");
}
} else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void IB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == ACCOLADE_OUVRANTE) {
    yylval = yylex();

    if(est_premier(_listeInstructions_, yylval)) {
      LI();
    }
    if(yylval == ACCOLADE_FERMANTE) {
      yylval = yylex();
    } else {
      error("'}' été attendu");
    }
} else {
    error("'{' été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void LI() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_instruction_, yylval)) {
    I();

    if(est_premier(_listeInstructions_, yylval)) {
      LI();
    }
}

if(!est_suivant(_listeInstructions_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void ISI() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == SI) {
        yylval = yylex();

        if(est_premier(_expression_, yylval)) {
        EXP();
      } else {
        syntaxError();
      }

      if(yylval == ALORS) {
        yylval = yylex();

        if(est_premier(_instructionBloc_, yylval)) {
          IB();

          if(est_premier(_optSinon_, yylval))
          {
            OSINON();
        }
      }
      else
      {
        syntaxError();
    }
} else {
    error("'alors' été attendu");
}
} else {
    error("'si' été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void OSINON() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == SINON) {
    yylval = yylex();

    if(est_premier(_instructionBloc_, yylval)) {
      IB();
    }
}

if(!est_suivant(_optSinon_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void ITQ() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == TANTQUE) {
    yylval = yylex();

    if(est_premier(_expression_, yylval)) {
      EXP();

      if(yylval == FAIRE) {
        yylval = yylex();
        
        if(est_premier(_instructionBloc_, yylval))
        {
        IB();
      }
      else
      {
        syntaxError();
      }
    }
    else
    {
    error("'faire' été attendu");
}
} else {
    syntaxError();
}
} else {
    error("'tantque' été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void IAPP() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_instructionAppel_, yylval)) {
    APPF();

    if(yylval == POINT_VIRGULE) {
      yylval = yylex();
    } else {
      error("';' été attendu");
    }
} else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void IRET() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == RETOUR) {
    yylval = yylex();

    if(est_premier(_expression_, yylval)) {
      EXP();

      if(yylval == POINT_VIRGULE) {
        yylval = yylex();
    }
    else
    {
        error("';' été attendu");
    }
} else {
    syntaxError();
}
} else {
    error("'retour' été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void IECR() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == ECRIRE) {
        yylval = yylex();

        if(yylval == PARENTHESE_OUVRANTE) {
        yylval = yylex();

        if(est_premier(_expression_, yylval)) {
          EXP();

          if(yylval == PARENTHESE_FERMANTE)
          {
            yylval = yylex();
        } else {
            error("')' été attendu");
        }
      }
      else
      {
        syntaxError();
    }
} else {
    error("'(' été attendu");
}
} else {
    error("'ecrire' été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void IVIDE() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == POINT_VIRGULE) {
    yylval = yylex();
} else {
    error("';' été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void EXP() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_conjonction_, yylval)) {
        CONJ();

        if(est_premier(_expressionBis_, yylval)) {
        EXPB();
      }
    } else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void EXPB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == OU) {
    yylval = yylex();

    if(est_premier(_conjonction_, yylval)) {
      CONJ();

      if(est_premier(_expressionBis_, yylval)) {
        EXPB();
    }
} else {
    syntaxError();
}
}

if(!est_suivant(_expressionBis_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void CONJ() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_negation_, yylval)) {
        NEG();

        if(est_premier(_conjonctionBis_, yylval)) {
        CONJB();
      }
    } else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void CONJB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == ET) {
    yylval = yylex();

    if(est_premier(_negation_, yylval)) {
      NEG();
      
      if(est_premier(_conjonctionBis_, yylval)) {
        CONJB();
    }
} else {
    syntaxError();
}
}

if(!est_suivant(_conjonctionBis_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void NEG() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == NON) {
        yylval = yylex();

        if(est_premier(_comparaison_, yylval)) {
        COMP();
      } else {
        syntaxError();
      }
    }
    else if(est_premier(_comparaison_, yylval)) {
    COMP();
} else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void COMP() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_expression_, yylval)) {
        E();

        if(est_premier(_comparaisonBis_, yylval)) {
        COMPB();
      }
    } else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void COMPB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == EGAL) {
        yylval = yylex();

        if(est_premier(_expression_, yylval)) {
        E();

        if(est_premier(_comparaisonBis_, yylval)) {
          COMPB();
        }
    } else syntaxError();
}
else if(yylval == INFERIEUR) {
    yylval = yylex();

    if(est_premier(_expression_, yylval)) {
      E();
      
      if(est_premier(_comparaisonBis_, yylval)) {
        COMPB();
    }
} else {
    syntaxError();
}
}

if(!est_suivant(_comparaisonBis_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void E() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_terme_, yylval)) {
        T();

        if(est_premier(_expArithBis_, yylval)) {
        EB();
      }
    } else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void EB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == PLUS) {
        yylval = yylex();

        if(est_premier(_terme_, yylval)) {
        T();

        if(est_premier(_expArithBis_, yylval)) {
          EB();
        }
    } else syntaxError();
}
else if(yylval == MOINS) {
    yylval = yylex();

    if(est_premier(_terme_, yylval)) {
      T();

      if(est_premier(_expArithBis_, yylval)) {
        EB();
    }
} else {
    syntaxError();
}
}

if(!est_suivant(_expArithBis_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void T() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_facteur_, yylval)) {
        F();

        if(est_premier(_termeBis_, yylval)) {
        TB();
      }
    } else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void TB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == FOIS) {
        yylval = yylex();

        if(est_premier(_facteur_, yylval)) {
        F();

        if(est_premier(_termeBis_, yylval)) {
          TB();
        }
    } else syntaxError();
}
else if(yylval == DIVISE) {
    yylval = yylex();

    if(est_premier(_facteur_, yylval)) {
      F();

      if(est_premier(_termeBis_, yylval)) {
        TB();
    }
} else {
    syntaxError();
}
}

if(!est_suivant(_termeBis_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void F() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == PARENTHESE_OUVRANTE) {
        yylval = yylex();

        if(est_premier(_expression_, yylval)) {
        EXP();

        if(yylval == PARENTHESE_FERMANTE) {
          yylval = yylex();
        } else error("')' été attendu");
    } else syntaxError();
}
else if(yylval == NOMBRE) {
    yylval = yylex();
}
else if(est_premier(_appelFct_, yylval)) {
    APPF();
}
else if(est_premier(_var_, yylval)) {
    VAR();
}
else if(yylval == LIRE) {
    yylval = yylex();

    if(yylval == PARENTHESE_OUVRANTE) {
      yylval = yylex();

      if(yylval == PARENTHESE_FERMANTE) {
        yylval = yylex();
    }
    else
    {
        error("')' été attendu");
    }
} else {
    error("'(' été attendu");
}
} else {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void VAR() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == ID_VAR) {
        yylval = yylex();

        if(est_premier(_optIndice_, yylval)) {
        OIND();
      }
    } else {
    error("Indice de variable été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void OIND() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == CROCHET_OUVRANT) {
        yylval = yylex();

        if(est_premier(_expression_, yylval)) {
        EXP();

        if(yylval == CROCHET_FERMANT) {
          yylval = yylex();
        }
        else error("']' été attendu");
    } else syntaxError();
}

if(!est_suivant(_optIndice_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void APPF() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == ID_FCT) {
        yylval = yylex();

        if(yylval == PARENTHESE_OUVRANTE) {
        yylval = yylex();

        if(est_premier(_listeExpressions_, yylval)) {
          LEXP();
        }

        if(yylval == PARENTHESE_FERMANTE) {
          yylval = yylex();
        } else error("')' été attendu");
    } else {
      error("'(' été attendu");
    }
} else {
    error("Identificateur de fonction été attendu");
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void LEXP() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(est_premier(_expression_, yylval)) {
        EXP();

        if(est_premier(_listeExpressionsBis_, yylval)) {
        LEXPB();
      }
    }

    if(!est_suivant(_listeExpressions_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}

void LEXPB() {
    affiche_balise_ouvrante(__FUNCTION__, 1);
    
    if(yylval == VIRGULE) {
    yylval = yylex();
    
    if(est_premier(_expression_, yylval)) {
      EXP();

      if(est_premier(_listeExpressionsBis_, yylval)) {
        LEXPB();
    }
} else {
    syntaxError();
}
}

if(!est_suivant(_listeExpressionsBis_, yylval)) {
    syntaxError();
}

    affiche_balise_fermante(__FUNCTION__, 1);
}