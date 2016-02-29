#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include "stdio.h"
#include "syntabs.h"

void analyseur_syntaxique(void);

void syntaxError(const char * f);
void error(const char * f, const char * s);

n_prog * PG();
n_l_dec * ODV();
n_l_dec * LDV();
n_l_dec * LDVB();
n_dec * DV();
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
