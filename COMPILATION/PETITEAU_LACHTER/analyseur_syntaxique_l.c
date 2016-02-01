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

void PG(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(!est_premier(uniteCourante, ODV)) printf("Erreur sur ODV avec %d", uniteCourante);
	ODV();
	if(!est_premier(uniteCourante, LDF)) printf("Erreur sur LDF avec %d", uniteCourante);
	LDF();
	affiche_balise_fermante(__func__, -1);
}

void ODV(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(uniteCourante, LDV)) LDV();
	affiche_balise_fermante(__func__, -1);
}

void LDV(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(!est_premier(uniteCourante, DV)) printf("Erreur sur DV avec %d", uniteCourante);
	DV();
	if(!est_premier(uniteCourante, LDVB)) printf("Erreur sur LDVB avec %d", uniteCourante);
	LDVB();
	affiche_balise_fermante(__func__, -1);
}

void LDVB(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante != VIRGULE) printf("Erreur avec VIRGULE et %d", uniteCourante);
		else uniteCourante = yylex();
	if(!est_premier(uniteCourante, DV)) printf("Erreur sur DV avec %d", uniteCourante);
	DV();
	if(!est_premier(uniteCourante, LDVB)) printf("Erreur sur LDVB avec %d", uniteCourante);
	LDVB();
	affiche_balise_fermante(__func__, -1);
}

void DV(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante != ENTIER) printf("Erreur avec ENTIER et %d", uniteCourante);
		else uniteCourante = yylex();
	if(uniteCourante != ID_VAR) printf("Erreur avec ID_VAR et %d", uniteCourante);
		else uniteCourante = yylex();
	if(!est_premier(uniteCourante, OTT)) printf("Erreur sur OTT avec %d", uniteCourante);
	OTT();
	affiche_balise_fermante(__func__, -1);
}

void OTT(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(uniteCourante != NOMBRE) printf("Erreur avec NOMBRE et %d", uniteCourante);
		else uniteCourante = yylex();
	affiche_balise_fermante(__func__, -1);
}

/*void PG(void) {
	affiche_balise_ouvrante(__func__, 1);
	if(est_premier(uniteCourante, ODV)) ODV();
		else if(est_premier(uniteCourante, LDF)) LDF();
		else printf("Erreur sur %s avec %d", __func__, uniteCourante);
	affiche_balise_fermante(__func__, -1);
}*/