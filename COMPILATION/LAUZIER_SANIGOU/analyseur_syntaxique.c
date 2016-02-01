#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"

int uniteCourante;
char nom[100];
char valeur[100];
char yytext[100];
int trace_xml = 1;

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
	affiche_balise_ouvrante("listeDecVariables", trace_xml);

	declarationVariables(); 
	listeDecVariablesBis();

	affiche_balise_fermante("listeDecVariables", trace_xml);
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
	affiche_balise_ouvrante("declarationVariables", trace_xml);
	
	if(uniteCourante == ENTIER){
		affiche_element("mot_clef", "entier", trace_xml);
		
		uniteCourante = yylex();
		
		if(uniteCourante == ID_VAR){
			affiche_element("id_variable", yytext, trace_xml);
			
			uniteCourante = yylex();
			optTailleTableau();
		}
		else{	printf("Erreur Syntaxe \n"); 	}
	}

	affiche_balise_fermante("declarationVariables", trace_xml);
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
			else{	printf("Erreur Syntaxe \n"); 	}
		}
		else{	printf("Erreur Syntaxe \n"); 	}
	}

	affiche_balise_fermante("optTailleTableau", trace_xml);
}

void listeDecFonctions(void) {
	affiche_balise_ouvrante("listeDecFonctions", trace_xml);
	
	declarationFonction();
	listeDecFonctions(); // Probleme 
	
	affiche_balise_fermante("listeDecFonctions", trace_xml);
}

void declarationFonction(void) {
	affiche_balise_ouvrante("declarationFonction", trace_xml);

	if(uniteCourante == ID_FCT){
		affiche_element("id_fonction", yytext, trace_xml);
		uniteCourante = yylex();
		listeParam();
		optDecVariables();
	}

	affiche_balise_fermante("declarationFonction", trace_xml);
}

void listeParam(void) {
	affiche_balise_ouvrante("listeParam", trace_xml);

	if(uniteCourante == PARENTHESE_OUVRANTE){
		affiche_element("symbole", "PARENTHESE_OUVRANTE", trace_xml);
		
		uniteCourante = yylex();
		optListeVariables();
		if(uniteCourante == PARENTHESE_FERMANTE){
			affiche_element("symbole", "PARENTHESE_FERMANTE", trace_xml);
			uniteCourante = yylex();
		}
		else { printf ("Erreur de Syntaxe");	}
	}

	affiche_balise_fermante("listeParam", trace_xml);
}

void optListeDecVariables(void) {
	affiche_balise_ouvrante("optListeDecVariables", trace_xml);

	listeDecVariables();

	affiche_balise_fermante("optListeDecVariables", trace_xml);
}






























void analyseur_syntaxique(int uc)
{
	uniteCourante = uc;
	
	programme(void);
}