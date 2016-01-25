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
	printf("%d\n", uniteCourante);
	E();
}

void E(void) {
	T();
	Ep();
	return;
}

void Ep(void) {
	if(uniteCourante == PLUS) {
		uniteCourante = yylex();
		printf("%d\n", uniteCourante);
		E();
	}
	return;
}

void T(void) {
	F();
	Tp();
	return;
}

void Tp(void) {
	if(uniteCourante == FOIS) {
		uniteCourante = yylex();
		printf("%d\n", uniteCourante);
		T();
	}
	return;
}

void F(void) {
	if(uniteCourante == PARENTHESE_OUVRANTE) {
		uniteCourante = yylex();
		printf("%d\n", uniteCourante);
		E();
		
		if(uniteCourante == PARENTHESE_FERMANTE) {
			uniteCourante = yylex();
			printf("%d\n", uniteCourante);
		} else {
			printf("Erreur de syntaxe");
			exit(-1);
		}
	} else if(uniteCourante == NOMBRE) {
		uniteCourante = yylex();
		printf("%d\n", uniteCourante);
	}
	return;
}