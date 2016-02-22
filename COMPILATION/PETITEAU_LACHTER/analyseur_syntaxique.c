#include <stdio.h>
#include <stdlib.h>
#include "analyseur_syntaxique.h"
#include "analyseur_lexical.h"
#include "symboles.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"

extern FILE *yyin;
extern int nb_ligne;

int uniteCourante;

void analyseur_syntaxique(void) {
	initialise_premiers();
	initialise_suivants();
	uniteCourante = yylex();
	PG();
	printf("La syntaxe gère un max. #CestCool #Pèse #YOLO\n");
}

void error(const char * f, const char * s) {
    printf("LIGNE %d (%s) : %s\n", nb_ligne, f, s);
    exit(1);
}

void syntaxError(const char * f) {
    printf("LIGNE %d (%s) : Erreur dans la syntaxe avec l'unité \"%d\"!\n", nb_ligne, f, uniteCourante);
    exit(1);
}

void PG() {
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_optDecVariables_, uniteCourante)) {
      ODV();
      if(est_premier(_listeDecFonctions_, uniteCourante)) LDF();
    }
    else if(est_premier(_listeDecFonctions_, uniteCourante)) LDF();
    else syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void ODV() {
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_listeDecVariables_, uniteCourante)) {
        LDV();
        if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
          else error(__func__, "';' était attendu");
    }
    if(!est_suivant(_optDecVariables_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void LDV() {
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_declarationVariable_, uniteCourante)) {
        DV();
        if(est_premier(_listeDecVariablesBis_, uniteCourante)) LDVB();
    } else syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void LDVB() {
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == VIRGULE) {
        uniteCourante = yylex();
        if(est_premier(_declarationVariable_, uniteCourante)) {
            DV();
            if(est_premier(_listeDecVariablesBis_, uniteCourante)) LDVB();
        } else syntaxError(__func__);
    }
    if(!est_suivant(_listeDecVariablesBis_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void DV() {
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == ENTIER) {
        uniteCourante = yylex();
        if(uniteCourante == ID_VAR) {
            uniteCourante = yylex();
            if(est_premier(_optTailleTableau_, uniteCourante)) OTT();
        } else error(__func__, "Un identificateur de variable était attendu");
    } else error(__func__, "'ENTIER' était attendu");
    affiche_balise_fermante(__func__, 1);
}

void OTT() {
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == CROCHET_OUVRANT) {
        uniteCourante = yylex();
        if(uniteCourante == NOMBRE) {
            uniteCourante = yylex();
            if(uniteCourante == CROCHET_FERMANT) uniteCourante = yylex();
                else error(__func__, "']' était attendu");
        } else error(__func__, "Un nombre était attendu");
    }
    if(!est_suivant(_optTailleTableau_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void LDF() {
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_declarationFonction_, uniteCourante)) {
        DF();
        if(est_premier(_listeDecFonctions_, uniteCourante)) LDF();
    } else syntaxError(__func__);
    if(!est_suivant(_listeDecFonctions_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void DF() {
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == ID_FCT) {
        uniteCourante = yylex();
        if(est_premier(_listeParam_, uniteCourante)) {
            LP();
            if(est_premier(_optDecVariables_, uniteCourante)) ODV();
            if(est_premier(_instructionBloc_, uniteCourante)) IB();
                else syntaxError(__func__);
        } else syntaxError(__func__);
    } else error(__func__, "Un identificateur de fonction était attendu");
    affiche_balise_fermante(__func__, 1);
}

void LP() {
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == PARENTHESE_OUVRANTE) {
        uniteCourante = yylex();
        if(est_premier(_optListeDecVariables_, uniteCourante)) OLDV();
        if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
            else error(__func__, "')' était attendu");
    } else error(__func__, "'(' était attendu");
    affiche_balise_fermante(__func__, 1);
}

void OLDV() {
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_listeDecVariables_, uniteCourante)) LDV();
    if(!est_suivant(_optListeDecVariables_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void I() {
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_instructionAffect_, uniteCourante)) IAFF();
        else if(est_premier(_instructionBloc_, uniteCourante)) IB();
        else if(est_premier(_instructionSi_, uniteCourante)) ISI();
        else if(est_premier(_instructionTantque_, uniteCourante)) ITQ();
        else if(est_premier(_instructionAppel_, uniteCourante)) IAPP();
        else if(est_premier(_instructionRetour_, uniteCourante)) IRET();
        else if(est_premier(_instructionEcriture_, uniteCourante)) IECR();
        else if(est_premier(_instructionVide_, uniteCourante)) IVIDE();
        else syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
}

void IAFF() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_var_, uniteCourante)) {
		VAR();
		if(uniteCourante == EGAL) {
			uniteCourante = yylex();
			if(est_premier(_expression_, uniteCourante)) {
				EXP();
				if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
					else error(__func__, "';' était attendu");
			} else syntaxError(__func__);
		} else error(__func__, "'=' était attendu");
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void IB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ACCOLADE_OUVRANTE) {
		uniteCourante = yylex();
		if(est_premier(_listeInstructions_, uniteCourante)) LI();
		if(uniteCourante == ACCOLADE_FERMANTE) uniteCourante = yylex();
			else error(__func__, "'}' était attendu");
	} else error(__func__, "'{' était attendu");
	affiche_balise_fermante(__func__, 1);
}

void LI() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_instruction_, uniteCourante)) {
		I();
		if(est_premier(_listeInstructions_, uniteCourante)) LI();
	}
	if(!est_suivant(_listeInstructions_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void ISI() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == SI) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) EXP();
			else syntaxError(__func__);
		if(uniteCourante == ALORS) {
			uniteCourante = yylex();
			if(est_premier(_instructionBloc_, uniteCourante)) {
				IB();
				if(est_premier(_optSinon_, uniteCourante)) OSINON();
			} else syntaxError(__func__);
		} else error(__func__, "'alors' était attendu");
	} else error(__func__, "'si' était attendu");
	affiche_balise_fermante(__func__, 1);
}

void OSINON() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == SINON) {
		uniteCourante = yylex();
		if(est_premier(_instructionBloc_, uniteCourante)) IB();
	}
	if(!est_suivant(_optSinon_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void ITQ() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == TANTQUE) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			EXP();
			if(uniteCourante == FAIRE) {
				uniteCourante = yylex();
				if(est_premier(_instructionBloc_, uniteCourante)) IB();
					else syntaxError(__func__);
			}
			else error(__func__, "'faire' était attendu");
		} else syntaxError(__func__);
	} else error(__func__, "'tantque' était attendu");
	affiche_balise_fermante(__func__, 1);
}

void IAPP() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_instructionAppel_, uniteCourante)) {
		APPF();
		if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
			else error(__func__, "';' était attendu");
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void IRET() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == RETOUR) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			EXP();
			if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
				else error(__func__, "';' était attendu");
		} else syntaxError(__func__);
	} else error(__func__, "'retour' était attendu");
	affiche_balise_fermante(__func__, 1);
}

void IECR() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ECRIRE) {
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			if(est_premier(_expression_, uniteCourante)) {
				EXP();
				if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
					else error(__func__, "')' était attendu");
			} else syntaxError(__func__);
		} else error(__func__, "'(' était attendu");
	} else error(__func__, "'ecrire' était attendu");
	affiche_balise_fermante(__func__, 1);
}

void IVIDE() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
		else error(__func__, "';' était attendu");
	affiche_balise_fermante(__func__, 1);
}

void EXP() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_conjonction_, uniteCourante)) {
		CONJ();
		if(est_premier(_expressionBis_, uniteCourante)) EXPB();
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void EXPB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == OU) {
		uniteCourante = yylex();
		if(est_premier(_conjonction_, uniteCourante)) {
			CONJ();
			if(est_premier(_expressionBis_, uniteCourante)) EXPB();
		} else syntaxError(__func__);
	}
	if(!est_suivant(_expressionBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void CONJ() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_negation_, uniteCourante)) {
		NEG();
		if(est_premier(_conjonctionBis_, uniteCourante)) CONJB();
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void CONJB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ET) {
		uniteCourante = yylex();
		if(est_premier(_negation_, uniteCourante)) {
			NEG();
			if(est_premier(_conjonctionBis_, uniteCourante)) CONJB();
		} else syntaxError(__func__);
	}
	if(!est_suivant(_conjonctionBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void NEG() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == NON) {
		uniteCourante = yylex();
		if(est_premier(_comparaison_, uniteCourante)) COMP();
			else syntaxError(__func__);
	} else if(est_premier(_comparaison_, uniteCourante)) COMP();
		else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void COMP() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_expression_, uniteCourante)) {
		E();
		if(est_premier(_comparaisonBis_, uniteCourante)) COMPB();
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void COMPB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == EGAL) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			E();
			if(est_premier(_comparaisonBis_, uniteCourante)) COMPB();
		} else syntaxError(__func__);
	} else if(uniteCourante == INFERIEUR) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			E();
			if(est_premier(_comparaisonBis_, uniteCourante)) COMPB();
		} else syntaxError(__func__);
	}
	if(!est_suivant(_comparaisonBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void E() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_terme_, uniteCourante)) {
		T();
		if(est_premier(_expArithBis_, uniteCourante)) EB();
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void EB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == PLUS) {
		uniteCourante = yylex();
		if(est_premier(_terme_, uniteCourante)) {
			T();
			if(est_premier(_expArithBis_, uniteCourante)) EB();
		} else syntaxError(__func__);
	} else if(uniteCourante == MOINS) {
		uniteCourante = yylex();
		if(est_premier(_terme_, uniteCourante)) {
			T();
			if(est_premier(_expArithBis_, uniteCourante)) EB();
		} else syntaxError(__func__);
	}
	if(!est_suivant(_expArithBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void T() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_facteur_, uniteCourante)) {
		F();
		if(est_premier(_termeBis_, uniteCourante)) TB();
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void TB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == FOIS) {
		uniteCourante = yylex();
		if(est_premier(_facteur_, uniteCourante)) {
			F();
			if(est_premier(_termeBis_, uniteCourante)) TB();
		} else syntaxError(__func__);
	} else if(uniteCourante == DIVISE) {
		uniteCourante = yylex();
		if(est_premier(_facteur_, uniteCourante)) {
			F();
			if(est_premier(_termeBis_, uniteCourante)) TB();
		} else syntaxError(__func__);
	}
	if(!est_suivant(_termeBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void F() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			EXP();
			if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
				else error(__func__, "')' était attendu");
		} else syntaxError(__func__);
	} else if(uniteCourante == NOMBRE) uniteCourante = yylex();
		else if(est_premier(_appelFct_, uniteCourante)) APPF();
		else if(est_premier(_var_, uniteCourante)) VAR();
	else if(uniteCourante == LIRE) {
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
				else error(__func__, "')' était attendu");
		} else error(__func__, "'(' était attendu");
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void VAR() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ID_VAR) {
		uniteCourante = yylex();
		if(est_premier(_optIndice_, uniteCourante)) OIND();
	} else error(__func__, "Indice de variable était attendu");
	affiche_balise_fermante(__func__, 1);
}

void OIND() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == CROCHET_OUVRANT) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			EXP();
			if(uniteCourante == CROCHET_FERMANT) uniteCourante = yylex();
				else error(__func__, "']' était attendu");
		} else syntaxError(__func__);
	}
	if(!est_suivant(_optIndice_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void APPF() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ID_FCT) {
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			if(est_premier(_listeExpressions_, uniteCourante)) LEXP();
			if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
				else error(__func__, "')' était attendu");
		} else error(__func__, "'(' était attendu");
	} else error(__func__, "Identificateur de fonction était attendu");
	affiche_balise_fermante(__func__, 1);
}

void LEXP() {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_expression_, uniteCourante)) {
		EXP();
		if(est_premier(_listeExpressionsBis_, uniteCourante)) LEXPB();
	}
	if(!est_suivant(_listeExpressions_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}

void LEXPB() {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == VIRGULE) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			EXP();
			if(est_premier(_listeExpressionsBis_, uniteCourante)) LEXPB();
		} else syntaxError(__func__);
	}
	if(!est_suivant(_listeExpressionsBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
}
