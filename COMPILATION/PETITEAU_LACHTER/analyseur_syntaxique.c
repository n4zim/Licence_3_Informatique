#include <stdio.h>
#include <stdlib.h>
#include "analyseur_syntaxique.h"
#include "analyseur_lexical.h"
#include "symboles.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"
#include "syntabs.h"

extern FILE *yyin;
extern int nb_ligne;
extern yytext;

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

n_prog* PG() {
	n_prog *$$ = NULL;
	n_l_dec *vars = NULL;
	n_l_dec *funcs = NULL;

    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_optDecVariables_, uniteCourante)) {
        vars = ODV();
		if(est_premier(_listeDecFonctions_, uniteCourante)) funcs = LDF();
    }
    else if(est_premier(_listeDecFonctions_, uniteCourante)) funcs = LDF();
    else syntaxError(__func__);
    $$ = creer_n_prog(vars, funcs);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_l_dec* ODV() {
	n_l_dec *$$ = NULL;

    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_listeDecVariables_, uniteCourante)) {
        $$ = LDV();
        if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
        	else error(__func__, "';' était attendu");
    }
    if(!est_suivant(_optDecVariables_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_l_dec* LDV() {
	n_l_dec *$$ = NULL;
    n_dec *tete = NULL;
    n_l_dec *queue = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_declarationVariable_, uniteCourante)) {
        tete = DV();
        if(est_premier(_listeDecVariablesBis_, uniteCourante)) queue = LDVB();
        $$ = cree_n_l_dec(tete, queue);
    } else syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_l_dec* LDVB() {
    n_l_dec *$$ = NULL;
    n_dec *tete = NULL;
    n_l_dec *queue = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == VIRGULE) {
        uniteCourante = yylex();
        if(est_premier(_declarationVariable_, uniteCourante)) {
            tete = DV();
            if(est_premier(_listeDecVariablesBis_, uniteCourante)) queue = LDVB();
        } else syntaxError(__func__);
    }
    $$ = cree_n_l_dec(tete, queue);
    if(!est_suivant(_listeDecVariablesBis_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_dec* DV() {
    n_dec *$$ = NULL;
    char* nomVar;
    int optTailleTableau = 0;
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == ENTIER) {
        uniteCourante = yylex();
        if(uniteCourante == ID_VAR) {
        	strcpy(nomVar, yytext);
            uniteCourante = yylex();
            if(est_premier(_optTailleTableau_, uniteCourante)) optTailleTableau = OTT();
        } else error(__func__, "Un identificateur de variable était attendu");
    } else error(__func__, "'ENTIER' était attendu");
    if(optTailleTableau <= 0) $$ = cree_n_dec_var(nomVar);
    	else $$ = cree_n_dec_tab(nomVar, optTailleTableau);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

int OTT() {
	int optTailleTableau = -1;
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == CROCHET_OUVRANT) {
        uniteCourante = yylex(); 
        if(uniteCourante == NOMBRE) {
        	optTailleTableau = atoi(yytext);
            uniteCourante = yylex();
            if(uniteCourante == CROCHET_FERMANT) uniteCourante = yylex();
                else error(__func__, "']' était attendu");
        } else error(__func__, "Un nombre était attendu");
    }
    if(!est_suivant(_optTailleTableau_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
    return optTailleTableau;
}

n_l_dec* LDF() {
	n_l_dec *$$ = NULL;
	n_dec *declarationFonction = NULL;
	n_l_dec *listeDecFonctions = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_declarationFonction_, uniteCourante)) {
        declarationFonction = DF();
        if(est_premier(_listeDecFonctions_, uniteCourante)) listeDecFonctions = LDF();
    } else syntaxError(__func__);
    if(!est_suivant(_listeDecFonctions_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
    $$ = cree_n_l_dec(declarationFonction, listeDecFonctions);
  	return $$;
}

n_dec* DF() {
	n_dec* $$ = NULL;
	char* nomFonc = NULL;
	n_l_dec *listeParam = NULL;
	n_l_dec *optDecVariables = NULL;
	n_instr *instructionBloc = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == ID_FCT) {
   		strcpy(nomFonc, yytext);
        uniteCourante = yylex();
        if(est_premier(_listeParam_, uniteCourante)) {
            listeParam = LP();
            if(est_premier(_optDecVariables_, uniteCourante)) optDecVariables = ODV();
            if(est_premier(_instructionBloc_, uniteCourante)) instructionBloc = IB();
                else syntaxError(__func__);
            $$ = cree_n_dec_fonc(nomFonc, listeParam, optDecVariables, instructionBloc);
        } else syntaxError(__func__);
    } else error(__func__, "Un identificateur de fonction était attendu");
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_l_dec* LP() {
	n_l_dec *$$ = NULL;
	n_l_dec *optListeDecVariables = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == PARENTHESE_OUVRANTE) {
        uniteCourante = yylex();
        if(est_premier(_optListeDecVariables_, uniteCourante)) optListeDecVariables = OLDV();
        if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
            else error(__func__, "')' était attendu");
        $$ = cree_n_l_dec(optListeDecVariables->tete, optListeDecVariables->queue);
    } else error(__func__, "'(' était attendu");
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_l_dec* OLDV() {
	n_l_dec *$$ = NULL;
	n_l_dec *listeDecVariables = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_listeDecVariables_, uniteCourante)) listeDecVariables = LDV();
    if(!est_suivant(_optListeDecVariables_, uniteCourante)) syntaxError(__func__);
    $$ = cree_n_l_dec(listeDecVariables->tete, listeDecVariables->queue);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_instr* I() {
	n_instr *$$ = NULL;
	n_instr *instruction = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_instructionAffect_, uniteCourante)) {
    	instruction = IAFF();
    	$$ = cree_n_instr_affect(instruction->var, instruction->exp);
    } else if(est_premier(_instructionBloc_, uniteCourante)) {
    	instruction = IB();
    	$$ = cree_n_instr_bloc(instruction->liste);
    } else if(est_premier(_instructionSi_, uniteCourante)) {
    	instruction = ISI();
    	$$ = cree_n_instr_si(instruction->test, instruction->alors, instruction->sinon);
    } else if(est_premier(_instructionTantque_, uniteCourante)) {
    	instruction = ITQ();
    	$$ = cree_n_instr_tantque(instruction->test, instruction->faire);
    } else if(est_premier(_instructionAppel_, uniteCourante)) {
    	instruction = IAPP();
    	$$ = cree_n_instr_appel(instruction->appel);
    } else if(est_premier(_instructionRetour_, uniteCourante)) {
    	instruction = IRET();
    	$$ = cree_n_instr_retour(instruction->expression);
    } else if(est_premier(_instructionEcriture_, uniteCourante)) {
    	instruction = IECR();
    	$$ = cree_n_instr_ecrire(instruction->expression);
    } else if(est_premier(_instructionVide_, uniteCourante)) {
    	IVIDE();
    	$$ = cree_n_instr_vide();
    } else syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
    return $$;
}

n_instr* IAFF() {
	n_instr *$$ = NULL;
	n_var *var = NULL;
	n_exp *exp = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_var_, uniteCourante)) {
		var = VAR();
		if(uniteCourante == EGAL) {
			uniteCourante = yylex();
			if(est_premier(_expression_, uniteCourante)) {
				exp = EXP();
				if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
					else error(__func__, "';' était attendu");
				$$ = cree_n_instr_affect(var, exp);
			} else syntaxError(__func__);
		} else error(__func__, "'=' était attendu");
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr* IB() {
	n_instr *$$ = NULL;
	n_l_instr *listeInstructions = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ACCOLADE_OUVRANTE) {
		uniteCourante = yylex();
		if(est_premier(_listeInstructions_, uniteCourante)) listeInstructions = LI();
		if(uniteCourante == ACCOLADE_FERMANTE) uniteCourante = yylex();
			else error(__func__, "'}' était attendu");
		$$ = cree_n_instr_bloc(listeInstructions);
	} else error(__func__, "'{' était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_l_instr *LI() {
	n_l_instr *$$ = NULL;
	n_instr *instruction = NULL;
	n_l_instr *listeInstructions = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_instruction_, uniteCourante)) {
		instruction = I();
		if(est_premier(_listeInstructions_, uniteCourante)) listeInstructions = LI();
	}
	if(!est_suivant(_listeInstructions_, uniteCourante)) syntaxError(__func__);
	$$ = cree_n_l_instr(instruction, listeInstructions);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *ISI() {
	n_instr *$$ = NULL;
	n_exp *expression = NULL;
	n_instr *instructionBloc = NULL;
	n_instr *instructionSinon = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == SI) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) expression = EXP();
			else syntaxError(__func__);
		if(uniteCourante == ALORS) {
			uniteCourante = yylex();
			if(est_premier(_instructionBloc_, uniteCourante)) {
				instructionBloc = IB();
				if(est_premier(_optSinon_, uniteCourante)) instructionSinon = OSINON();
				$$ = cree_n_instr_si(expression, instructionBloc, instructionSinon);
			} else syntaxError(__func__);
		} else error(__func__, "'alors' était attendu");
	} else error(__func__, "'si' était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *OSINON() {
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == SINON) {
		uniteCourante = yylex();
		if(est_premier(_instructionBloc_, uniteCourante)) $$ = IB();
	}
	if(!est_suivant(_optSinon_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *ITQ() {
	n_instr *$$ = NULL;
	n_exp *expression = NULL;
	n_instr *instructionBloc = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == TANTQUE) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			expression = EXP();
			if(uniteCourante == FAIRE) {
				uniteCourante = yylex();
				if(est_premier(_instructionBloc_, uniteCourante)) instructionBloc = IB();
					else syntaxError(__func__);
				$$ = cree_n_instr_tantque(expression, instructionBloc);
			}
			else error(__func__, "'faire' était attendu");
		} else syntaxError(__func__);
	} else error(__func__, "'tantque' était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *IAPP() {
	n_instr *$$ = NULL;
	n_appel *appelFct = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_instructionAppel_, uniteCourante)) {
		appelFct = APPF();
		if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
			else error(__func__, "';' était attendu");
		$$ = cree_n_instr_appel(appelFct);
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *IRET() {
	n_instr *$$ = NULL;
	n_exp *expression = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == RETOUR) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			expression = EXP();
			if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
				else error(__func__, "';' était attendu");
			$$ = cree_n_instr_retour(expression);
		} else syntaxError(__func__);
	} else error(__func__, "'retour' était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *IECR() {
	n_instr *$$ = NULL;
	n_exp *expression = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ECRIRE) {
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			if(est_premier(_expression_, uniteCourante)) {
				expression = EXP();
				if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
					else error(__func__, "')' était attendu");
				$$ = cree_n_instr_ecrire(expression);
			} else syntaxError(__func__);
		} else error(__func__, "'(' était attendu");
	} else error(__func__, "'ecrire' était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_instr *IVIDE() {
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
		else error(__func__, "';' était attendu");
	$$ = cree_n_instr_vide();
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *EXP() {
	n_exp *$$ = NULL;
	n_exp *conjonction = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_conjonction_, uniteCourante)) {
		conjonction = CONJ();
		if(est_premier(_expressionBis_, uniteCourante)) $$ = EXPB(conjonction);
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *EXPB(n_exp *heritage) {
	n_exp *$$ = heritage;
	n_exp *conjonction = NULL;
	n_exp *expressionBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == OU) {
		uniteCourante = yylex();
		if(est_premier(_conjonction_, uniteCourante)) {
			conjonction = CONJ();
			expressionBis = cree_n_exp_op(ou, heritage, conjonction);
			if(est_premier(_expressionBis_, uniteCourante)) expressionBis = EXPB(expressionBis);
			$$ = expressionBis;
		} else syntaxError(__func__);
	}
	if(!est_suivant(_expressionBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
;}

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
