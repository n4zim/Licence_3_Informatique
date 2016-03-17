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
int OTT();
n_l_dec * LDF();
n_dec * DF();
n_l_dec * LP();
n_l_dec * OLDV();
n_instr * I();
n_instr * IAFF();
n_instr * IB();
n_l_instr * LI();
n_instr * ISI();
n_instr * OSINON();
n_instr * ITQ();
n_instr * IAPP();
n_instr * IRET();
n_instr * IECR();
n_instr * IVIDE();
n_exp * EXP();
n_exp * EXPB();
n_exp * CONJ();
n_exp * CONJB();
n_exp * NEG();
n_exp * COMP();
n_exp * COMPB();
n_exp * E();
n_exp * EB();
n_exp * T();
n_exp * TB();
n_exp * F();
n_var * VAR();
n_exp * OIND();
n_appel * APPF();
n_l_exp * LEXP();
n_l_exp * LEXPB();

#endif
