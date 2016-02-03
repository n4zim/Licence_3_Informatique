#ifndef __ANALYSEUR_LEXICAL__
#define __ANALYSEUR_LEXICAL__

#include "stdio.h"

int yylval;

int yylex(void);
void nom_token( int token, char *nom, char *valeur );
void test_yylex_internal( FILE *yyin );

void syntaxError();
void syntaxErrorMsg( const char * msg );
void openSection( const char * section );
void closeSection( const char * section );

void PG();
void ODV();
void LDV();
void LDVB();
void DV();
void OTT();
void LDF();
void DF();
void LP();
void OLDV();
void I();
void IAFF();
void IB();
void LI();
void ISI();
void OSINON();
void ITQ();
void IAPP();
void IRET();
void IECR();
void IVIDE();
void EXP();
void EXPB();
void CONJ();
void CONJB();
void NEG();
void COMP();
void COMPB();
void E();
void EB();
void T();
void TB();
void F();
void VAR();
void OIND();
void APPF();
void LEXP();
void LEXPB();

#endif

