#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
#include "syntabs.h"
#include "affiche_arbre_abstrait.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"

int uniteCourante;
//char nom[100];
//char valeur[100];
char yytext[100];
int nb_ligne;
int trace_xml = 0;

void syntaxError(void) {
	printf ("Erreur de Syntaxe : Ligne %d caractère %s\n", nb_ligne, yytext);
	exit(-1);
}

n_prog * programme(void){

	n_prog *res = NULL;
	n_l_dec *var1 = NULL;
	n_l_dec *var2 = NULL;

	affiche_balise_ouvrante("programme", trace_xml);
	var1 = optDecVariables();
	var2 = listeDecFonctions();
	affiche_balise_fermante("programme", trace_xml);

	res = cree_n_prog(var1, var2);

	return res;
}

n_l_dec * optDecVariables(void){

	n_l_dec *res = NULL;

	affiche_balise_ouvrante("optDecVariables", trace_xml);

	if(uniteCourante == ENTIER)
	{
		res = listeDecVariables();

		if(uniteCourante == POINT_VIRGULE){
			affiche_element("symbole", "POINT_VIRGULE", trace_xml);
			uniteCourante = yylex();
		}
		else
			syntaxError();

		affiche_balise_fermante("optDecVariables", trace_xml);		
		
	} 
	else
		res = NULL;

	return res;
}

n_l_dec * listeDecVariables(void) {
	n_l_dec *res = NULL;
	n_dec * var1 = NULL;
	n_l_dec *var2 = NULL;

	if(uniteCourante == ENTIER) {
		affiche_balise_ouvrante("listeDecVariables", trace_xml);

		var1 = declarationVariables(); 
		var2 = listeDecVariablesBis();

		res = cree_n_l_dec(var1, var2);

		affiche_balise_fermante("listeDecVariables", trace_xml);
	}

	return res;
}

n_l_dec * listeDecVariablesBis(void) {
	n_l_dec *res = NULL;
	n_dec *var1 = NULL;
	n_l_dec *var2 = NULL;

	affiche_balise_ouvrante("listeDecVariablesBis", trace_xml);

	if(uniteCourante == VIRGULE){
		affiche_element("symbole", "VIRGULE", trace_xml);
		uniteCourante = yylex();

		var1 = declarationVariables();
		var2 = listeDecVariablesBis();
		res = cree_n_l_dec(var1, var2);
	}
	else
		res = NULL;

	affiche_balise_fermante("listeDecVariablesBis", trace_xml);

	return res;
}

n_dec * declarationVariables(void) {
	n_dec *res = NULL;

	affiche_balise_ouvrante("declarationVariable", trace_xml);
	
	if(uniteCourante == ENTIER){
		affiche_element("mot_clef", "entier", trace_xml);
		
		uniteCourante = yylex();
		
		if(uniteCourante == ID_VAR){
			affiche_element("id_variable", yytext, trace_xml);
			char * nom = strdup(yytext);
			
			uniteCourante = yylex();

			res = optTailleTableau(nom);

			if(res == NULL)
				res = cree_n_dec_var(nom);
		}
		else{	syntaxError(); 	}
	}
	else{	syntaxError(); 	} // peut etre erreur

	affiche_balise_fermante("declarationVariable", trace_xml);

	return res;
}

n_dec * optTailleTableau(char * nom) {
	n_dec *res = NULL;

	affiche_balise_ouvrante("optTailleTableau", trace_xml);

	if(uniteCourante == CROCHET_OUVRANT){
		affiche_element("symbole", "CROCHET_OUVRANT", trace_xml);
		uniteCourante = yylex();

		if(uniteCourante == NOMBRE){
			affiche_element("nombre", yytext, trace_xml);
			char * nombre = strdup(yytext);
			uniteCourante = yylex();

			res = cree_n_dec_tab(nom, atoi(nombre));

			if(uniteCourante == CROCHET_FERMANT)
			{
				affiche_element("symbole", "CROCHET_FERMANT", trace_xml);
				uniteCourante = yylex();
			}
			else{	syntaxError();	}
		}
		else{	syntaxError();	}
	}

	affiche_balise_fermante("optTailleTableau", trace_xml);

	return res;
}

n_l_dec * listeDecFonctions(void) {

	n_l_dec * res = NULL;

	affiche_balise_ouvrante("listeDecFonctions", trace_xml);
	
	if(uniteCourante == ID_FCT) {
		n_dec * var1 = NULL;
		n_l_dec * var2 = NULL;

		var1 = declarationFonction();
		var2 = listeDecFonctions(); 

		res = cree_n_l_dec(var1, var2);
	}	

	affiche_balise_fermante("listeDecFonctions", trace_xml);

	return res;
}

n_dec * declarationFonction(void) {
	n_dec * res = NULL;

	affiche_balise_ouvrante("declarationFonction", trace_xml);

	if(uniteCourante == ID_FCT){
		affiche_element("id_fonction", yytext, trace_xml);
		char * nom = strdup(yytext);
		uniteCourante = yylex();

		n_l_dec * param = NULL;
		n_l_dec * variables = NULL;
		n_instr * corps = NULL;

		param = listeParam();
		variables = optDecVariables();
		corps = instructionBloc();

		res = cree_n_dec_fonc(nom, param, variables, corps);
	}

	affiche_balise_fermante("declarationFonction", trace_xml);

	return res;
}

n_l_dec * listeParam(void) {
	n_l_dec * res = NULL;

	affiche_balise_ouvrante("listeParam", trace_xml);

	if(uniteCourante == PARENTHESE_OUVRANTE){
		affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
		
		uniteCourante = yylex();

		res = optListeDecVariables();

		if(uniteCourante == PARENTHESE_FERMANTE){
			affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
			uniteCourante = yylex();
		}
		else { syntaxError();	}
	}

	affiche_balise_fermante("listeParam", trace_xml);

	return res;
}

n_l_dec * optListeDecVariables(void) {
	n_l_dec * res = NULL;

	affiche_balise_ouvrante("optListeDecVariables", trace_xml);

	res = listeDecVariables();

	affiche_balise_fermante("optListeDecVariables", trace_xml);

	return res;
}

n_instr * instruction(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instruction", trace_xml);

	if(uniteCourante == ID_VAR)
		res = instructionAffect();
	else if(uniteCourante == ACCOLADE_OUVRANTE)
		res = instructionBloc();
	else if(uniteCourante == SI)
		res = instructionSi();
	else if(uniteCourante == TANTQUE)
		res = instructionTantque();
	else if(uniteCourante == ID_FCT)
		res = instructionAppel();
	else if(uniteCourante == RETOUR)
		res = instructionRetour();
	else if(uniteCourante == ECRIRE)
		res = instructionEcriture();
	else if(uniteCourante == POINT_VIRGULE)
		res = instructionVide();
	else 
		syntaxError();

	affiche_balise_fermante("instruction", trace_xml);

	return res;
}

n_instr * instructionAffect(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionAffect", trace_xml);
	
	n_var * var1 = var();

	if(uniteCourante == EGAL) {
		affiche_element("symbole", "EGAL", trace_xml);
		uniteCourante = yylex();

		n_exp * expr = expression();
		
		if(uniteCourante == POINT_VIRGULE) {
			affiche_element("symbole", "POINT_VIRGULE", trace_xml);
			uniteCourante = yylex();

			res = cree_n_instr_affect(var1, expr);
		}
		else { 	syntaxError(); }
	}
	else {
		if(uniteCourante == INCR)
		{
			affiche_element("symbole", "INCR", trace_xml);
			uniteCourante = yylex();
			
			if(uniteCourante == POINT_VIRGULE) {
				affiche_element("symbole", "POINT_VIRGULE", trace_xml);
				uniteCourante = yylex();

				res = cree_n_instr_affect_incr(var1);
			}
			else { 	syntaxError(); }
		}
		else
	 		syntaxError(); 	
	}	

	affiche_balise_fermante("instructionAffect", trace_xml);

	return res;
}

n_instr * instructionBloc(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionBloc", trace_xml);
	
	if(uniteCourante == ACCOLADE_OUVRANTE) {
		affiche_element("symbole", "ACCOLADE_OUVRANTE", trace_xml);
		uniteCourante = yylex();

		n_l_instr * liste = listeInstructions();
		
		if(uniteCourante == ACCOLADE_FERMANTE) { 
			affiche_element("symbole", "ACCOLADE_FERMANTE", trace_xml);
			uniteCourante = yylex();

			res = cree_n_instr_bloc(liste);
		}
		else { 	syntaxError(); 	}
	} 
	else { 	syntaxError();	}
	
	affiche_balise_fermante("instructionBloc", trace_xml);

	return res;
}

n_l_instr * listeInstructions(void) {
	n_l_instr * res = NULL;

	affiche_balise_ouvrante("listeInstructions", trace_xml);

	if(uniteCourante == ID_VAR || uniteCourante == ACCOLADE_OUVRANTE || uniteCourante == SI || uniteCourante == TANTQUE || uniteCourante == ID_FCT || uniteCourante == RETOUR || uniteCourante == ECRIRE || uniteCourante == POINT_VIRGULE)
	{
		n_instr * tete = instruction();
		n_l_instr * queue = listeInstructions();

		res = cree_n_l_instr(tete, queue);
	}

	affiche_balise_fermante("listeInstructions", trace_xml);

	return res;
}

n_instr * instructionSi(void) {
	n_instr * res = NULL;
	affiche_balise_ouvrante("instructionSi", trace_xml);
	
	if(uniteCourante == SI) {
		affiche_element("mot_clef", "si", trace_xml);
		uniteCourante = yylex();

		n_exp * test = expression();

		if(uniteCourante == ALORS) {
			affiche_element("mot_clef", "alors", trace_xml);
			uniteCourante = yylex();

			n_instr * alors = instructionBloc();
			n_instr * sinon = optSinon();	

			res = cree_n_instr_si(test, alors, sinon);
		}
		else { 	syntaxError(); }
	}
	
	affiche_balise_fermante("instructionSi", trace_xml);

	return res;
}

n_instr * optSinon(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("optSinon", trace_xml);

	if(uniteCourante == SINON){
		affiche_element("mot_clef", "sinon", trace_xml);
		uniteCourante = yylex();

		res = instructionBloc();
	}

	affiche_balise_fermante("optSinon", trace_xml);

	return res;
}

n_instr * instructionTantque(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionTantque", trace_xml);

	if(uniteCourante == TANTQUE) {
		affiche_element("mot_clef", "tantque", trace_xml);
		uniteCourante = yylex();

		n_exp * test = expression();

		if(uniteCourante == FAIRE) {
			affiche_element("mot_clef", "faire", trace_xml);
			uniteCourante = yylex();

			n_instr * faire = instructionBloc();

			res = cree_n_instr_tantque(test, faire);
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("instructionTantque", trace_xml);

	return res;
}

n_instr * instructionAppel(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionAppel", trace_xml);

	n_appel * app = appelFct();

	if(uniteCourante == POINT_VIRGULE) {
		affiche_element("symbole", "POINT_VIRGULE", trace_xml);
		uniteCourante = yylex();

		res = cree_n_instr_appel(app);
	}
	else {	syntaxError();	}

	affiche_balise_fermante("instructionAppel", trace_xml);

	return res;
}

n_instr * instructionRetour(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionRetour", trace_xml);

	if(uniteCourante == RETOUR) {
		affiche_element("mot_clef", "retour", trace_xml);
		uniteCourante = yylex();

		n_exp * expression1 = expression();

		if(uniteCourante == POINT_VIRGULE) {
			affiche_element("symbole", "POINT_VIRGULE", trace_xml);
			uniteCourante = yylex();

			res = cree_n_instr_retour(expression1);
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("instructionRetour", trace_xml);

	return res;
}

n_instr * instructionEcriture(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionEcriture", trace_xml);

	if(uniteCourante == ECRIRE) {
		affiche_element("mot_clef", "ecrire", trace_xml);
		uniteCourante = yylex();

		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
			uniteCourante = yylex();

			n_exp * expression1 = expression();

			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
				uniteCourante = yylex();

				if(uniteCourante == POINT_VIRGULE) {
					affiche_element("symbole", "POINT_VIRGULE", trace_xml);
					uniteCourante = yylex();

					res = cree_n_instr_ecrire(expression1);
				}
			}
			else {	syntaxError();	}
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("instructionEcriture", trace_xml);

	return res;
}

n_instr * instructionVide(void) {
	n_instr * res = NULL;

	affiche_balise_ouvrante("instructionVide", trace_xml);

	if(uniteCourante == POINT_VIRGULE) {
		affiche_element("symbole", "POINT_VIRGULE", trace_xml);
		uniteCourante = yylex();

		res = cree_n_instr_vide();
	}

	affiche_balise_fermante("instructionVide", trace_xml);

	return res;
}

n_exp * expression(void) {
	n_exp * res = NULL;

	if(uniteCourante == NON || uniteCourante == PARENTHESE_OUVRANTE || uniteCourante == ID_FCT || uniteCourante == ID_VAR || uniteCourante == NOMBRE || uniteCourante == LIRE)
	{
		affiche_balise_ouvrante("expression", trace_xml);

		n_exp * con = conjonction();
		res = expressionBis(con);

		affiche_balise_fermante("expression", trace_xml);
	}

	return res;
}

n_exp * expressionBis(n_exp * con) {
	n_exp * res = con;

	affiche_balise_ouvrante("expressionBis", trace_xml);

	if(uniteCourante == OU) {
		affiche_element("symbole", "OU", trace_xml);
		uniteCourante = yylex();

		operation op = ou;
		n_exp * op2 = conjonction();

		res = cree_n_exp_op(op, con, op2);

		res = expressionBis(res);
	}

	affiche_balise_fermante("expressionBis", trace_xml);

	return res;
}

n_exp * conjonction(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("conjonction", trace_xml);

	n_exp * op1 = negation();
	res = conjonctionBis(op1);

	affiche_balise_fermante("conjonction", trace_xml);

	return res;
}

n_exp * conjonctionBis(n_exp * op1) {
	n_exp * res = op1;

	affiche_balise_ouvrante("conjonctionBis", trace_xml);

	if(uniteCourante == ET) {
		affiche_element("symbole", "ET", trace_xml);
		uniteCourante = yylex();

		operation op = et;
		n_exp * op2 = negation();

		res = cree_n_exp_op(op, op1, op2);

		res = conjonctionBis(res);
	}

	affiche_balise_fermante("conjonctionBis", trace_xml);

	return res;
}

n_exp * negation(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("negation", trace_xml);

	int estNon = 0;

	if(uniteCourante == NON) {
		affiche_element("symbole", "NON", trace_xml);
		uniteCourante = yylex();

		estNon = 1;
	}

	
	n_exp * op1 = comparaison();

	if(estNon)
	{
		operation op = non;
		res = cree_n_exp_op(op, op1, NULL);
	}
	else
		res = op1;

	affiche_balise_fermante("negation", trace_xml);

	return res;
}

n_exp * comparaison(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("comparaison", trace_xml);

	n_exp * ex = expArith();
	res = comparaisonBis(ex);

	affiche_balise_fermante("comparaison", trace_xml);

	return res;
}

n_exp * comparaisonBis(n_exp * ex) {
	n_exp * res = ex;

	affiche_balise_ouvrante("comparaisonBis", trace_xml);
	if(uniteCourante == EGAL) {
		affiche_element("symbole", "EGAL", trace_xml);
		uniteCourante = yylex();

		operation op = egal;
		n_exp * op2 = expArith();

		res = cree_n_exp_op(op, ex, op2);

		res = comparaisonBis(res);
	}

	if(uniteCourante == INFERIEUR) {
		affiche_element("symbole", "INFERIEUR", trace_xml);
		uniteCourante = yylex();

		operation op = inf;
		n_exp * op2 = expArith();

		res = cree_n_exp_op(op, ex, op2);

		res = comparaisonBis(res);
	}

	if(uniteCourante == SUPERIEUR) {
		affiche_element("symbole", "SUPERIEUR", trace_xml);
		uniteCourante = yylex();

		operation op = sup;
		n_exp * op2 = expArith();

		res = cree_n_exp_op(op, ex, op2);

		res = comparaisonBis(res);
	}

	affiche_balise_fermante("comparaisonBis", trace_xml);

	return res;
}

n_exp * expArith(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("expArith", trace_xml);

	n_exp * ter = terme();
	res = expArithBis(ter);

	affiche_balise_fermante("expArith", trace_xml);

	return res;
}

n_exp * expArithBis(n_exp * ter) {
	n_exp * res = ter;

	affiche_balise_ouvrante("expArithBis", trace_xml);

	if(uniteCourante == PLUS) {
		affiche_element("symbole", "PLUS", trace_xml);
		uniteCourante = yylex();

		operation op = plus;
		n_exp * op2 = terme();

		res = cree_n_exp_op(op, ter, op2);

		res = expArithBis(res);
	}

	if(uniteCourante == MOINS) {
		affiche_element("symbole", "MOINS", trace_xml);
		uniteCourante = yylex();

		operation op = moins;
		n_exp * op2 = terme();

		res = cree_n_exp_op(op, ter, op2);

		res = expArithBis(res);
	}

	if(uniteCourante == INCR) {
		affiche_element("symbole", "INCR", trace_xml);
		uniteCourante = yylex();

		operation op = incr;

		res = cree_n_exp_op(op, ter, NULL);

		res = expArithBis(res);
	}

	affiche_balise_fermante("expArithBis", trace_xml);

	return res;
}

n_exp * terme(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("terme", trace_xml);

	n_exp * fac = facteur();
	res = termeBis(fac);

	affiche_balise_fermante("terme", trace_xml);

	return res;
}

n_exp * termeBis(n_exp * fac) {
	n_exp * res = fac;

	affiche_balise_ouvrante("termeBis", trace_xml);

	if(uniteCourante == FOIS) {
		affiche_element("symbole", "FOIS", trace_xml);
		uniteCourante = yylex();

		operation op = fois;
		n_exp * op2 = facteur();

		res = cree_n_exp_op(op, fac, op2);

		res = termeBis(res);
	}

	if(uniteCourante == DIVISE) {
		affiche_element("symbole", "DIVISE", trace_xml);
		uniteCourante = yylex();

		operation op = divise;
		n_exp * op2 = facteur();

		res = cree_n_exp_op(op, fac, op2);

		res = termeBis(res);

	}

	affiche_balise_fermante("termeBis", trace_xml);

	return res;
}

n_exp * facteur(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("facteur", trace_xml);

	if(uniteCourante == PARENTHESE_OUVRANTE) {
		affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
		uniteCourante = yylex();

		res = expression();

		if(uniteCourante == PARENTHESE_FERMANTE) {
			affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
			uniteCourante = yylex();
		}
		else {	syntaxError();	}
	}
	else if(uniteCourante == NOMBRE) {
		affiche_element("nombre", yytext, trace_xml);
		char * entier = strdup(yytext);
		uniteCourante = yylex();

		res = cree_n_exp_entier(atoi(entier));
	}
	else if(uniteCourante == ID_FCT) {
		n_appel * app = appelFct();

		res = cree_n_exp_appel(app);
	}
	else if(uniteCourante == ID_VAR) {
		n_var * var1 = var();

		res = cree_n_exp_var(var1);
	}
	else if(uniteCourante == LIRE) {
		affiche_element("mot_clef", "lire", trace_xml);
		uniteCourante = yylex();

		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
			uniteCourante = yylex();

			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
				uniteCourante = yylex();

				res = cree_n_exp_lire();
			}
			else {	syntaxError(); 	}
		}
		else {	syntaxError();	}
	}
	else {	syntaxError();	}

	affiche_balise_fermante("facteur", trace_xml);

	return res;
}

n_var * var(void) {
	n_var * res = NULL;

	affiche_balise_ouvrante("var", trace_xml);

	if(uniteCourante == ID_VAR) {
		affiche_element("id_variable", yytext, trace_xml);
		char * nom = strdup(yytext);

		uniteCourante = yylex();

		n_exp * indice = optIndice();

		if(indice == NULL)
			res = cree_n_var_simple(nom);
		else
			res = cree_n_var_indicee(nom, indice);
	}

	affiche_balise_fermante("var", trace_xml);

	return res;
}

n_exp * optIndice(void) {
	n_exp * res = NULL;

	affiche_balise_ouvrante("optIndice", trace_xml);

	if(uniteCourante == CROCHET_OUVRANT) {
		affiche_element("symbole", "CROCHET_OUVRANT", trace_xml);
		uniteCourante = yylex();

		res = expression();

		if(uniteCourante == CROCHET_FERMANT) {
			affiche_element("symbole", "CROCHET_FERMANT", trace_xml);
			uniteCourante = yylex();
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("optIndice", trace_xml);

	return res;
}

n_appel * appelFct(void) {
	n_appel * res = NULL;

	affiche_balise_ouvrante("appelFct", trace_xml);

	if(uniteCourante == ID_FCT) {
		affiche_element("id_fonction", yytext, trace_xml);
		
		char * fonction = strdup(yytext);

		uniteCourante = yylex();

		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
			uniteCourante = yylex();

			n_l_exp * args = listeExpressions();

			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
				uniteCourante = yylex();

				res = cree_n_appel(fonction, args);
			}
			else {	syntaxError();	}
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("appelFct", trace_xml);

	return res;
}

n_l_exp * listeExpressions(void) {
	n_l_exp * res = NULL;

	affiche_balise_ouvrante("listeExpressions", trace_xml);

	if(uniteCourante == NON || uniteCourante == PARENTHESE_OUVRANTE || uniteCourante == ID_FCT || uniteCourante == ID_VAR || uniteCourante == NOMBRE || uniteCourante == LIRE)
	{
		n_exp * tete = expression();
		n_l_exp * queue = listeExpressionsBis();

		res = cree_n_l_exp(tete, queue);
	}

	affiche_balise_fermante("listeExpressions", trace_xml);

	return res;
}

n_l_exp * listeExpressionsBis(void) {
	n_l_exp * res = NULL;

	affiche_balise_ouvrante("listeExpressionsBis", trace_xml);

	if(uniteCourante == VIRGULE) {
		affiche_element("symbole", "VIRGULE", trace_xml);
		uniteCourante = yylex();

		n_exp * tete = expression();
		n_l_exp * queue = listeExpressionsBis();

		res = cree_n_l_exp(tete, queue);
	}

	affiche_balise_fermante("listeExpressionsBis", trace_xml);

	return res;
}

void analyseur_syntaxique(int uc) {
	uniteCourante = uc;
	
	n_prog * res = programme();

	affiche_n_prog(res);
}