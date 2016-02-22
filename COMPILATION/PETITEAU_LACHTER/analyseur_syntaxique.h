#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include "stdio.h"

void analyseur_syntaxique(void);

void syntaxError(const char * f);
void error(const char * f, const char * s);

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
