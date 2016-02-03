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

void Eprim(void)
{
	if(uniteCourante == PLUS)
	{
		nom_token( uniteCourante, nom, valeur );
		printf("%s\t%s\t%s\n", yytext, nom, valeur);
		uniteCourante = yylex();
		E();
		return;
	}
	else
	{
		return;
	}
}

void T(void)
{
	F();
	Tprim();
	return;
}

void Tprim(void)
{
	if(uniteCourante == FOIS)
	{
		nom_token( uniteCourante, nom, valeur );
		printf("%s\t%s\t%s\n", yytext, nom, valeur);
		uniteCourante = yylex();
		T();
		return;
	}
	else
	{
		return;
	}
}

void F(void)
{
	if(uniteCourante == PARENTHESE_OUVRANTE)
	{
		nom_token( uniteCourante, nom, valeur );
		printf("%s\t%s\t%s\n", yytext, nom, valeur);
		uniteCourante = yylex();
		E();
		if(uniteCourante != PARENTHESE_FERMANTE)
		{
			printf( "Erreur de syntaxe \n" );
			exit( -1 );
		}
		else
		{
			nom_token( uniteCourante, nom, valeur );
			printf("%s\t%s\t%s\n", yytext, nom, valeur);
			uniteCourante = yylex();
			return;
		}
	}
	else 
	{
		if(uniteCourante != NOMBRE)
		{
			if(uniteCourante == FIN)
				return;

			printf( "Erreur de syntaxe \n" );
			exit( -1 );
		}
	}
	nom_token( uniteCourante, nom, valeur );
	printf("%s\t%s\t%s\n", yytext, nom, valeur);
	uniteCourante = yylex();
}

void E(void)
{
	T();
	Eprim();
	return;
}

void analyseur_syntaxique(int uc)
{
	uniteCourante = uc;
	E();
}