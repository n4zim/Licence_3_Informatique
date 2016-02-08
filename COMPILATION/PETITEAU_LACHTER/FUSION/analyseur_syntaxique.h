#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include "stdio.h"

void analyseur_syntaxique(void);

private void syntaxError();
private void error( const char * msg );

private void PG();
private void ODV();
private void LDV();
private void LDVB();
private void DV();
private void OTT();
private void LDF();
private void DF();
private void LP();
private void OLDV();
private void I();
private void IAFF();
private void IB();
private void LI();
private void ISI();
private void OSINON();
private void ITQ();
private void IAPP();
private void IRET();
private void IECR();
private void IVIDE();
private void EXP();
private void EXPB();
private void CONJ();
private void CONJB();
private void NEG();
private void COMP();
private void COMPB();
private void E();
private void EB();
private void T();
private void TB();
private void F();
private void VAR();
private void OIND();
private void APPF();
private void LEXP();
private void LEXPB();

#endif