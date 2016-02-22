#include <stdio.h>
#include <stdlib.h>
#include "analyseur_syntaxique.h"
#include "analyseur_lexical.h"
#include "symboles.h"
#include "util.h"


extern FILE *yyin;
int uniteCourante;

void analyseur_syntaxique(void) {
	uniteCourante = yylex();
	//printf("%d\n", uniteCourante);
	E();
	printf("La syntaxe gère un max. #CestCool #Pèse #YOLO\n");
}

void E(void) {
	affiche_balise_ouvrante(__func__, 1);
	T();
	Ep();
	affiche_balise_fermante(__func__, -1);
	return;
}

void Ep(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == PLUS) {
		uniteCourante = yylex();
		//printf("%d\n", uniteCourante);
		E();
	}
	affiche_balise_fermante(__func__, -1);
	return;
}

void T(void) {
	affiche_balise_ouvrante(__func__, 1);
	F();
	Tp();
	affiche_balise_fermante(__func__, -1);
	return;
}

void Tp(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == FOIS) {
		uniteCourante = yylex();
		//printf("%d\n", uniteCourante);
		T();
	}
	affiche_balise_fermante(__func__, -1);
	return;
}

void F(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		//printf("%d\n", uniteCourante);
		E();
		
		if(uniteCourante == PARENTHESE_FERMANTE) {
			uniteCourante = yylex();
			//printf("%d\n", uniteCourante);
		} else {
			printf("Erreur de syntaxe");
			exit(-1);
		}
	} else if(uniteCourante == NOMBRE) {
		uniteCourante = yylex();
		//printf("%d\n", uniteCourante);
	}
	affiche_balise_fermante(__func__, -1);
	return;
}