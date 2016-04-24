#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_syntaxique.h"
#include "affiche_arbre_abstrait.h"
#include "analyseur_lexical.h"
#include "symboles.h"
#include "parcours_mips.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"
#include "syntabs.h"

extern FILE *yyin;
extern int nb_ligne;
extern char yytext[100];

int uniteCourante;

void analyseur_syntaxique(void) {
	initialise_premiers();
	initialise_suivants();
	uniteCourante = yylex();
	n_prog * pg = PG();
	printf("La syntaxe gère un max. #CestCool #Pèse #YOLO\n\n");
	//printf("Arbre abstrait : \n");
	parcours_n_prog(pg);
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
    $$ = cree_n_prog(vars, funcs);
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
        	nomVar = strdup(yytext);
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
   		nomFonc = strdup(yytext);
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
    affiche_balise_ouvrante(__func__, 1);
    if(uniteCourante == PARENTHESE_OUVRANTE) {
        uniteCourante = yylex();
        if(est_premier(_optListeDecVariables_, uniteCourante)) $$ = OLDV();
        if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
            else error(__func__, "')' était attendu");
    } else error(__func__, "'(' était attendu");
    affiche_balise_fermante(__func__, 1);
  
    return $$;
}

n_l_dec* OLDV() {
	n_l_dec *$$ = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_listeDecVariables_, uniteCourante)) $$ = LDV();
    if(!est_suivant(_optListeDecVariables_, uniteCourante)) syntaxError(__func__);
    affiche_balise_fermante(__func__, 1);
  
    return $$;
}

n_instr* I() {
	n_instr *$$ = NULL;
	n_instr *instruction = NULL;
    affiche_balise_ouvrante(__func__, 1);
    if(est_premier(_instructionAffect_, uniteCourante)) {
    	instruction = IAFF();
    } else if(est_premier(_instructionBloc_, uniteCourante)) {
    	instruction = IB();
    } else if(est_premier(_instructionSi_, uniteCourante)) {
    	instruction = ISI();
    } else if(est_premier(_instructionTantque_, uniteCourante)) {
    	instruction = ITQ();
    } else if(est_premier(_instructionAppel_, uniteCourante)) {
    	instruction = IAPP();
    } else if(est_premier(_instructionRetour_, uniteCourante)) {
    	instruction = IRET();
    } else if(est_premier(_instructionEcriture_, uniteCourante)) {
    	instruction = IECR();
    } else if(est_premier(_instructionVide_, uniteCourante)) {
    	IVIDE();
    } else syntaxError(__func__);
    $$ = instruction;
    affiche_balise_fermante(__func__, 1);
  
    return $$;
}

n_instr* IAFF() {
	n_instr *$$ = NULL;
	n_var *var = NULL;
	n_exp *expr = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_var_, uniteCourante)) {
		var = VAR();
		if(uniteCourante == EGAL) {
			uniteCourante = yylex();
			if(est_premier(_expression_, uniteCourante)) {
				expr = EXP();
				if(uniteCourante == POINT_VIRGULE) uniteCourante = yylex();
					else error(__func__, "';' était attendu");
				$$ = cree_n_instr_affect(var, expr);
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
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_conjonction_, uniteCourante)) {
		$$ = CONJ();
		if(est_premier(_expressionBis_, uniteCourante)) $$ = EXPB($$);
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
}

n_exp *CONJ() {
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_negation_, uniteCourante)) {
		$$ = NEG();
		if(est_premier(_conjonctionBis_, uniteCourante)) $$ = CONJB($$);
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *CONJB(n_exp *heritage) {
	n_exp *$$ = heritage;
	n_exp *negation = NULL;
	n_exp *conjonctionBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ET) {
		uniteCourante = yylex();
		if(est_premier(_negation_, uniteCourante)) {
			negation = NEG();
			conjonctionBis = cree_n_exp_op(et, heritage, negation);
			if(est_premier(_conjonctionBis_, uniteCourante)) conjonctionBis = CONJB(conjonctionBis);
			$$ = conjonctionBis;
		} else syntaxError(__func__);
	}
	if(!est_suivant(_conjonctionBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *NEG() {
	n_exp *$$ = NULL;
	n_exp *comparaison = NULL;
	int estNon = 0;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == NON) {
		estNon = 1;
		uniteCourante = yylex();
		if(est_premier(_comparaison_, uniteCourante)) comparaison = COMP();
			else syntaxError(__func__);
	} else if(est_premier(_comparaison_, uniteCourante)) {
		comparaison = COMP();
	} else syntaxError(__func__);
	if(estNon == 1) $$ = cree_n_exp_op(non, comparaison, NULL);
		else $$ = comparaison;
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *COMP() {
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_expression_, uniteCourante)) {
		$$ = E();
		if(est_premier(_comparaisonBis_, uniteCourante)) $$ = COMPB($$);
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *COMPB(n_exp *heritage) {
	n_exp *$$ = heritage;
	n_exp *expArith = NULL;
	n_exp *comparaisonBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == EGAL) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			expArith = E();
			comparaisonBis = cree_n_exp_op(egal, heritage, expArith);
			if(est_premier(_comparaisonBis_, uniteCourante)) comparaisonBis = COMPB(comparaisonBis);
		} else syntaxError(__func__);
	} else if(uniteCourante == INFERIEUR) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			expArith = E();
			comparaisonBis = cree_n_exp_op(inf, heritage, expArith);
			if(est_premier(_comparaisonBis_, uniteCourante)) comparaisonBis = COMPB(comparaisonBis);
		} else syntaxError(__func__);
	} else if(uniteCourante == SUPERIEUR) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			expArith = E();
			comparaisonBis = cree_n_exp_op(sup, heritage, expArith);
			if(est_premier(_comparaisonBis_, uniteCourante)) comparaisonBis = COMPB(comparaisonBis);
		} else syntaxError(__func__);
	}
	$$ = comparaisonBis;
	if(!est_suivant(_comparaisonBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *E() {
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_terme_, uniteCourante)) {
		$$ = T();
		if(est_premier(_expArithBis_, uniteCourante)) $$ = EB($$);
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *EB(n_exp *heritage) {
	n_exp *$$ = heritage;
	n_exp *terme = NULL;
	n_exp *expArithBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == PLUS) {
		uniteCourante = yylex();
		if(est_premier(_terme_, uniteCourante)) {
			terme = T();
			expArithBis = cree_n_exp_op(plus, heritage, terme);
			if(est_premier(_expArithBis_, uniteCourante)) expArithBis = EB(expArithBis);
		} else syntaxError(__func__);
	} else if(uniteCourante == MOINS) {
		uniteCourante = yylex();
		if(est_premier(_terme_, uniteCourante)) {
			terme = T();
			expArithBis = cree_n_exp_op(moins, heritage, terme);
			if(est_premier(_expArithBis_, uniteCourante)) expArithBis = EB(expArithBis);
		} else syntaxError(__func__);
	}
	$$ = expArithBis;
	if(!est_suivant(_expArithBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *T() {
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_facteur_, uniteCourante)) {
		$$ = F();
		if(est_premier(_termeBis_, uniteCourante))
			$$ = TB($$);
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *TB(n_exp *heritage) {
	n_exp *$$ = heritage;
	n_exp *facteur = NULL;
	n_exp *termeBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == FOIS) {
		uniteCourante = yylex();
		if(est_premier(_facteur_, uniteCourante)) {
			facteur = F();
			termeBis = cree_n_exp_op(fois, heritage, facteur);
			if(est_premier(_termeBis_, uniteCourante)) termeBis = TB(termeBis);
		} else syntaxError(__func__);
	} else if(uniteCourante == DIVISE) {
		uniteCourante = yylex();
		if(est_premier(_facteur_, uniteCourante)) {
			facteur = F();
			termeBis = cree_n_exp_op(divise, heritage, facteur);
			if(est_premier(_termeBis_, uniteCourante)) termeBis = TB(termeBis);
		} else syntaxError(__func__);
	}
	$$ = termeBis;
	if(!est_suivant(_termeBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *F() {
	n_exp *$$ = NULL;
	char *entier = NULL;
	n_appel *appelFct = NULL;
	n_var *var = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			$$ = EXP();
			if(uniteCourante == PARENTHESE_FERMANTE) uniteCourante = yylex();
				else error(__func__, "')' était attendu");
		} else syntaxError(__func__);
	} else if(uniteCourante == NOMBRE) {
		entier = strdup(yytext);
		uniteCourante = yylex();
		$$ = cree_n_exp_entier(atoi(entier));
	} else if(est_premier(_appelFct_, uniteCourante)) {
		appelFct = APPF();
		$$ = cree_n_exp_appel(appelFct);
	} else if(est_premier(_var_, uniteCourante)) {
		var = VAR();
		$$ = cree_n_exp_var(var);
	} else if(uniteCourante == LIRE) {
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			if(uniteCourante == PARENTHESE_FERMANTE) {
				uniteCourante = yylex();
				$$ = cree_n_exp_lire();
			} else error(__func__, "')' était attendu");
		} else error(__func__, "'(' était attendu");
	} else syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_var *VAR() {
	n_var *$$ = NULL;
	n_exp *optIndice = NULL;
	char *nom = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ID_VAR) {
		nom = strdup(yytext);
		uniteCourante = yylex();
		if(est_premier(_optIndice_, uniteCourante)) optIndice = OIND();
		if(optIndice == NULL) $$ = cree_n_var_simple(nom);
			else $$ = cree_n_var_indicee(nom, optIndice);
	} else error(__func__, "Indice de variable était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_exp *OIND() {
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == CROCHET_OUVRANT) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			$$ = EXP();
			if(uniteCourante == CROCHET_FERMANT) uniteCourante = yylex();
				else error(__func__, "']' était attendu");
		} else syntaxError(__func__);
	}
	if(!est_suivant(_optIndice_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_appel *APPF() {
	n_appel *$$ = NULL;
	char *nom = NULL;
	n_l_exp *listeExpressions = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == ID_FCT) {
		nom = strdup(yytext);
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			uniteCourante = yylex();
			if(est_premier(_listeExpressions_, uniteCourante)) listeExpressions = LEXP();
			if(uniteCourante == PARENTHESE_FERMANTE) {
				uniteCourante = yylex();
				$$ = cree_n_appel(nom, listeExpressions);
			}
				else error(__func__, "')' était attendu");
		} else error(__func__, "'(' était attendu");
	} else error(__func__, "Identificateur de fonction était attendu");
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_l_exp *LEXP() {
	n_l_exp *$$ = NULL;
	n_exp *expression = NULL;
	n_l_exp *listeExpressionsBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(_expression_, uniteCourante)) {
		expression = EXP();
		if(est_premier(_listeExpressionsBis_, uniteCourante)) listeExpressionsBis = LEXPB();
		$$ = cree_n_l_exp(expression, listeExpressionsBis);
	}
	if(!est_suivant(_listeExpressions_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}

n_l_exp *LEXPB() {
	n_l_exp *$$ = NULL;
	n_exp *expression = NULL;
	n_l_exp *listeExpressionsBis = NULL;
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == VIRGULE) {
		uniteCourante = yylex();
		if(est_premier(_expression_, uniteCourante)) {
			expression = EXP();
			if(est_premier(_listeExpressionsBis_, uniteCourante)) listeExpressionsBis = LEXPB();
			$$ = cree_n_l_exp(expression, listeExpressionsBis);
		} else syntaxError(__func__);
	}
	if(!est_suivant(_listeExpressionsBis_, uniteCourante)) syntaxError(__func__);
	affiche_balise_fermante(__func__, 1);
	return $$;
}