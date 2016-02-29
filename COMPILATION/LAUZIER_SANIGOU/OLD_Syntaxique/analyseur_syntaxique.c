#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"

int uniteCourante;
//char nom[100];
//char valeur[100];
char yytext[100];
int trace_xml = 1;

void syntaxError(void) {
	printf ("Erreur de Syntaxe \n");
}

void programme(void){
	affiche_balise_ouvrante("programme", trace_xml);
	optDecVariables();
	listeDecFonctions();
	affiche_balise_fermante("programme", trace_xml);
}

void optDecVariables(void){
	affiche_balise_ouvrante("optDecVariables", trace_xml);

	listeDecVariables();

	if(uniteCourante == POINT_VIRGULE){
		affiche_element("symbole", "POINT_VIRGULE", trace_xml);
		uniteCourante = yylex();
	}

	affiche_balise_fermante("optDecVariables", trace_xml);
	
}

void listeDecVariables(void) {
	if(uniteCourante == ENTIER) {
		affiche_balise_ouvrante("listeDecVariables", trace_xml);

		declarationVariables(); 
		listeDecVariablesBis();

		affiche_balise_fermante("listeDecVariables", trace_xml);
	}
}

void listeDecVariablesBis(void) {
	affiche_balise_ouvrante("listeDecVariablesBis", trace_xml);

	if(uniteCourante == VIRGULE){
		affiche_element("symbole", "VIRGULE", trace_xml);
		uniteCourante = yylex();

		declarationVariables();
		listeDecVariablesBis();
	}

	affiche_balise_fermante("listeDecVariablesBis", trace_xml);
}

void declarationVariables(void) {
	affiche_balise_ouvrante("declarationVariable", trace_xml);
	
	if(uniteCourante == ENTIER){
		affiche_element("mot_clef", "entier", trace_xml);
		
		uniteCourante = yylex();
		
		if(uniteCourante == ID_VAR){
			affiche_element("id_variable", yytext, trace_xml);
			
			uniteCourante = yylex();
			optTailleTableau();
		}
		else{	syntaxError(); 	}
	}

	affiche_balise_fermante("declarationVariable", trace_xml);
}

void optTailleTableau(void) {
	affiche_balise_ouvrante("optTailleTableau", trace_xml);

	if(uniteCourante == CROCHET_OUVRANT){
		affiche_element("symbole", "CROCHET_OUVRANT", trace_xml);
		uniteCourante = yylex();

		if(uniteCourante == NOMBRE){
			affiche_element("nombre", yytext, trace_xml);
			uniteCourante = yylex();

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
}

void listeDecFonctions(void) {
	affiche_balise_ouvrante("listeDecFonctions", trace_xml);
	
	if(uniteCourante == ID_FCT) {
		declarationFonction();
		listeDecFonctions(); 
	}	

	affiche_balise_fermante("listeDecFonctions", trace_xml);
}

void declarationFonction(void) {
	affiche_balise_ouvrante("declarationFonction", trace_xml);

	if(uniteCourante == ID_FCT){
		affiche_element("id_fonction", yytext, trace_xml);
		uniteCourante = yylex();
		listeParam();
		optDecVariables();
		instructionBloc();
	}

	affiche_balise_fermante("declarationFonction", trace_xml);
}

void listeParam(void) {
	affiche_balise_ouvrante("listeParam", trace_xml);

	if(uniteCourante == PARENTHESE_OUVRANTE){
		affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
		
		uniteCourante = yylex();
		optListeDecVariables();
		if(uniteCourante == PARENTHESE_FERMANTE){
			affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
			uniteCourante = yylex();
		}
		else { syntaxError();	}
	}

	affiche_balise_fermante("listeParam", trace_xml);
}

void optListeDecVariables(void) {
	affiche_balise_ouvrante("optListeDecVariables", trace_xml);

	listeDecVariables();

	affiche_balise_fermante("optListeDecVariables", trace_xml);
}

void instruction(void) {
	affiche_balise_ouvrante("instruction", trace_xml);

	if(uniteCourante == ID_VAR)
		instructionAffect();
	else if(uniteCourante == ACCOLADE_OUVRANTE)
		instructionBloc();
	else if(uniteCourante == SI)
		instructionSi();
	else if(uniteCourante == TANTQUE)
		instructionTantque();
	else if(uniteCourante == ID_FCT)
		instructionAppel();
	else if(uniteCourante == RETOUR)
		instructionRetour();
	else if(uniteCourante == ECRIRE)
		instructionEcriture();
	else if(uniteCourante == POINT_VIRGULE)
		instructionVide();

	affiche_balise_fermante("instruction", trace_xml);
}

void instructionAffect(void) {
	affiche_balise_ouvrante("instructionAffect", trace_xml);
	
	var();

	if(uniteCourante == EGAL) {
		affiche_element("symbole", "EGAL", trace_xml);
		uniteCourante = yylex();

		expression();
		
		if(uniteCourante == POINT_VIRGULE) {
			affiche_element("symbole", "POINT_VIRGULE", trace_xml);
			uniteCourante = yylex();
		}
		else { 	syntaxError(); }
	}
	else { 	syntaxError(); 	}	

	affiche_balise_fermante("instructionAffect", trace_xml);
}

void instructionBloc(void) {
	affiche_balise_ouvrante("instructionBloc", trace_xml);
	
	if(uniteCourante == ACCOLADE_OUVRANTE) {
		affiche_element("symbole", "ACCOLADE_OUVRANTE", trace_xml);
		uniteCourante = yylex();

		listeInstructions();
		
		if(uniteCourante == ACCOLADE_FERMANTE) { 
			affiche_element("symbole", "ACCOLADE_FERMANTE", trace_xml);
			uniteCourante = yylex();
		}
		else { 	syntaxError(); 	}
	} 
	else { 	syntaxError();	}
	
	affiche_balise_fermante("instructionBloc", trace_xml);
}

void listeInstructions(void) {
	affiche_balise_ouvrante("listeInstructions", trace_xml);

	if(uniteCourante == ID_VAR || uniteCourante == ACCOLADE_OUVRANTE || uniteCourante == SI || uniteCourante == TANTQUE || uniteCourante == ID_FCT || uniteCourante == RETOUR || uniteCourante == ECRIRE || uniteCourante == POINT_VIRGULE)
	{
		instruction();
		listeInstructions();
	}

	affiche_balise_fermante("listeInstructions", trace_xml);
}

void instructionSi(void) {
	affiche_balise_ouvrante("instructionSi", trace_xml);
	
	if(uniteCourante == SI) {
		affiche_element("mot_clef", "si", trace_xml);
		uniteCourante = yylex();

		expression();

		if(uniteCourante == ALORS) {
			affiche_element("mot_clef", "alors", trace_xml);
			uniteCourante = yylex();

			instructionBloc();
			optSinon();	
		}
		else { 	syntaxError(); }
	}
	
	affiche_balise_fermante("instructionSi", trace_xml);
}

void optSinon(void) {
	affiche_balise_ouvrante("optSinon", trace_xml);

	if(uniteCourante == SINON){
		affiche_element("mot_clef", "sinon", trace_xml);
		uniteCourante = yylex();

		instructionBloc();
	}

	affiche_balise_fermante("optSinon", trace_xml);
}

void instructionTantque(void) {
	affiche_balise_ouvrante("instructionTantque", trace_xml);

	if(uniteCourante == TANTQUE) {
		affiche_element("mot_clef", "tantque", trace_xml);
		uniteCourante = yylex();

		expression();

		if(uniteCourante == FAIRE) {
			affiche_element("mot_clef", "faire", trace_xml);
			uniteCourante = yylex();

			instructionBloc();
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("instructionTantque", trace_xml);
}

void instructionAppel(void) {
	affiche_balise_ouvrante("instructionAppel", trace_xml);

	appelFct();

	if(uniteCourante == POINT_VIRGULE) {
		affiche_element("symbole", "POINT_VIRGULE", trace_xml);
		uniteCourante = yylex();
	}
	else {	syntaxError();	}

	affiche_balise_fermante("instructionAppel", trace_xml);
}

void instructionRetour(void) {
	affiche_balise_ouvrante("instructionRetour", trace_xml);

	if(uniteCourante == RETOUR) {
		affiche_element("mot_clef", "retour", trace_xml);
		uniteCourante = yylex();

		expression();

		if(uniteCourante == POINT_VIRGULE) {
			affiche_element("symbole", "POINT_VIRGULE", trace_xml);
			uniteCourante = yylex();
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("instructionRetour", trace_xml);
}

void instructionEcriture(void) {
	affiche_balise_ouvrante("instructionEcriture", trace_xml);

	if(uniteCourante == ECRIRE) {
		affiche_element("mot_clef", "ecrire", trace_xml);
		uniteCourante = yylex();

		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
			uniteCourante = yylex();

			expression();

			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
				uniteCourante = yylex();

				if(uniteCourante == POINT_VIRGULE) {
					affiche_element("symbole", "POINT_VIRGULE", trace_xml);
					uniteCourante = yylex();
				}
			}
			else {	syntaxError();	}
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("instructionEcriture", trace_xml);
}

void instructionVide(void) {
	affiche_balise_ouvrante("instructionVide", trace_xml);

	if(uniteCourante == POINT_VIRGULE) {
		affiche_element("symbole", "POINT_VIRGULE", trace_xml);
		uniteCourante = yylex();
	}

	affiche_balise_fermante("instructionVide", trace_xml);
}

void expression(void) {
	if(uniteCourante == NON || uniteCourante == PARENTHESE_OUVRANTE || uniteCourante == ID_FCT || uniteCourante == ID_VAR || uniteCourante == NOMBRE || uniteCourante == LIRE)
	{
		affiche_balise_ouvrante("expression", trace_xml);

		conjonction();
		expressionBis();

		affiche_balise_fermante("expression", trace_xml);
	}
}

void expressionBis(void) {
	affiche_balise_ouvrante("expressionBis", trace_xml);

	if(uniteCourante == OU) {
		affiche_element("symbole", "OU", trace_xml);
		uniteCourante = yylex();

		conjonction();
		expressionBis();
	}

	affiche_balise_fermante("expressionBis", trace_xml);
}

void conjonction(void) {
	affiche_balise_ouvrante("conjonction", trace_xml);

	negation();
	conjonctionBis();

	affiche_balise_fermante("conjonction", trace_xml);
}

void conjonctionBis(void) {
	affiche_balise_ouvrante("conjonctionBis", trace_xml);

	if(uniteCourante == ET) {
		affiche_element("symbole", "ET", trace_xml);
		uniteCourante = yylex();

		negation();
		conjonctionBis();
	}

	affiche_balise_fermante("conjonctionBis", trace_xml);
}

void negation(void) {
	affiche_balise_ouvrante("negation", trace_xml);

	if(uniteCourante == NON) {
		affiche_element("symbole", "NON", trace_xml);
		uniteCourante = yylex();
	}

	comparaison();

	affiche_balise_fermante("negation", trace_xml);
}

void comparaison(void) {
	affiche_balise_ouvrante("comparaison", trace_xml);

	expArith();
	comparaisonBis();

	affiche_balise_fermante("comparaison", trace_xml);
}

void comparaisonBis(void) {
	affiche_balise_ouvrante("comparaisonBis", trace_xml);

	if(uniteCourante == EGAL) {
		affiche_element("symbole", "EGAL", trace_xml);
		uniteCourante = yylex();

		expArith();
		comparaisonBis();
	}

	if(uniteCourante == INFERIEUR) {
		affiche_element("symbole", "INFERIEUR", trace_xml);
		uniteCourante = yylex();

		expArith();
		comparaisonBis();
	}

	//Ajouter supérieur ICI !!

	affiche_balise_fermante("comparaisonBis", trace_xml);
}

void expArith(void) {
	affiche_balise_ouvrante("expArith", trace_xml);

	terme();
	expArithBis();

	affiche_balise_fermante("expArith", trace_xml);
}

void expArithBis(void) {
	affiche_balise_ouvrante("expArithBis", trace_xml);

	if(uniteCourante == PLUS) {
		affiche_element("symbole", "PLUS", trace_xml);
		uniteCourante = yylex();

		terme();
		expArithBis();
	}

	if(uniteCourante == MOINS) {
		affiche_element("symbole", "MOINS", trace_xml);
		uniteCourante = yylex();

		terme();
		expArithBis();
	}

	affiche_balise_fermante("expArithBis", trace_xml);
}

void terme(void) {
	affiche_balise_ouvrante("terme", trace_xml);

	facteur();
	termeBis();

	affiche_balise_fermante("terme", trace_xml);
}

void termeBis(void) {
	affiche_balise_ouvrante("termeBis", trace_xml);

	if(uniteCourante == FOIS) {
		affiche_element("symbole", "FOIS", trace_xml);
		uniteCourante = yylex();

		facteur();
		termeBis();
	}

	if(uniteCourante == DIVISE) {
		affiche_element("symbole", "DIVISE", trace_xml);
		uniteCourante = yylex();

		facteur();
		termeBis();
	}

	affiche_balise_fermante("termeBis", trace_xml);
}

void facteur(void) {
	affiche_balise_ouvrante("facteur", trace_xml);

	if(uniteCourante == PARENTHESE_OUVRANTE) {
		affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
		uniteCourante = yylex();

		expression();

		if(uniteCourante == PARENTHESE_FERMANTE) {
			affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
			uniteCourante = yylex();
		}
		else {	syntaxError();	}
	}
	else if(uniteCourante == NOMBRE) {
		affiche_element("nombre", yytext, trace_xml);
		uniteCourante = yylex();
	}
	else if(uniteCourante == ID_FCT) {
		appelFct();
	}
	else if(uniteCourante == ID_VAR) {
		var();
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
			}
			else {	syntaxError(); 	}
		}
		else {	syntaxError();	}
	}
	else {	syntaxError();	}

	affiche_balise_fermante("facteur", trace_xml);
}

void var(void) {
	affiche_balise_ouvrante("var", trace_xml);

	if(uniteCourante == ID_VAR) {
		affiche_element("id_variable", yytext, trace_xml);
		uniteCourante = yylex();

		optIndice();
	}

	affiche_balise_fermante("var", trace_xml);
}

void optIndice(void) {
	affiche_balise_ouvrante("optIndice", trace_xml);

	if(uniteCourante == CROCHET_OUVRANT) {
		affiche_element("symbole", "CROCHET_OUVRANT", trace_xml);
		uniteCourante = yylex();

		expression();

		if(uniteCourante == CROCHET_FERMANT) {
			affiche_element("symbole", "CROCHET_FERMANT", trace_xml);
			uniteCourante = yylex();
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("optIndice", trace_xml);
}

void appelFct(void) {
	affiche_balise_ouvrante("appelFct", trace_xml);

	if(uniteCourante == ID_FCT) {
		affiche_element("id_fonction", yytext, trace_xml);
		uniteCourante = yylex();

		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
			uniteCourante = yylex();

			listeExpressions();

			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
				uniteCourante = yylex();
			}
			else {	syntaxError();	}
		}
		else {	syntaxError();	}
	}

	affiche_balise_fermante("appelFct", trace_xml);
}

void listeExpressions(void) {
	affiche_balise_ouvrante("listeExpressions", trace_xml);

	if(uniteCourante == NON || uniteCourante == PARENTHESE_OUVRANTE || uniteCourante == ID_FCT || uniteCourante == ID_VAR || uniteCourante == NOMBRE || uniteCourante == LIRE)
	{
		expression();
		listeExpressionsBis();
	}

	affiche_balise_fermante("listeExpressions", trace_xml);
}

void listeExpressionsBis(void) {
	affiche_balise_ouvrante("listeExpressionsBis", trace_xml);

	if(uniteCourante == VIRGULE) {
		affiche_element("symbole", "VIRGULE", trace_xml);
		uniteCourante = yylex();

		expression();
		listeExpressionsBis();
	}

	affiche_balise_fermante("listeExpressionsBis", trace_xml);
}

void analyseur_syntaxique(int uc)
{
	uniteCourante = uc;
	
	programme();
}