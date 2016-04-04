#include <stdio.h>
#include "util.h"
#include "dico.h"
#include "generation_mips.h"

int next_reg = 0;

/*-------------------------------------------------------------------------*/
void parcours_n_prog(n_prog *n)
{
  int ind;
	printf("\t.data\n");
  parcours_l_dec(n->variables);
	printf("\n\t.text\n");
  parcours_l_dec(n->fonctions);
  if((ind = rechercheDeclarative("main")) == -1)
    erreur("Erreur : fonction main inexistante\n");
  if(dico.tab[ind].complement != 0)
    erreur("Erreur : main ne doit pas contenir d'argument");  
}
/*-------------------------------------------------------------------------*/
void parcours_l_instr(n_l_instr *n)
{

  if(n){ 
  parcours_instr(n->tete);
  parcours_l_instr(n->queue); 
  }
}
/*-------------------------------------------------------------------------*/
void parcours_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) parcours_l_instr(n->u.liste);
    else if(n->type == affecteInst) parcours_instr_affect(n);
    else if(n->type == siInst) parcours_instr_si(n);
    else if(n->type == tantqueInst) parcours_instr_tantque(n);
    else if(n->type == faireInst) parcours_instr_faire(n);
    else if(n->type == pourInst) parcours_instr_pour(n);    
    else if(n->type == appelInst) parcours_instr_appel(n);
    else if(n->type == retourInst) parcours_instr_retour(n);
    else if(n->type == ecrireInst) parcours_instr_ecrire(n);
  }
}
/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n)
{  
  parcours_exp(n->u.si_.test);
  parcours_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    parcours_instr(n->u.si_.sinon);
  } 
}
/*-------------------------------------------------------------------------*/
void parcours_instr_tantque(n_instr *n)
{
  parcours_exp(n->u.tantque_.test);
  parcours_instr(n->u.tantque_.faire); 
}
/*-------------------------------------------------------------------------*/
void parcours_instr_faire(n_instr *n)            
{                                                                     
  parcours_instr(n->u.faire_.faire);             
  parcours_exp(n->u.faire_.test);                       
}                                               
/*-------------------------------------------------------------------------*/
void parcours_instr_pour(n_instr *n)                  
{                                                                                         
  parcours_instr(n->u.pour_.init);                    
  parcours_exp(n->u.pour_.test);                      
  parcours_instr(n->u.pour_.faire);                   
  parcours_instr(n->u.pour_.incr);                                
}                                                    
/*-------------------------------------------------------------------------*/
void parcours_instr_affect(n_instr *n)
{
  parcours_var(n->u.affecte_.var);
  parcours_exp(n->u.affecte_.exp); 
  printf("\tsw\t$t%d, %s\t\t#stocke variable\n", next_reg, n->u.affecte_.var->nom);
  next_reg++;
  //printf("%d\n", next_reg);

}
/*-------------------------------------------------------------------------*/
void parcours_instr_appel(n_instr *n)
{
  parcours_appel(n->u.appel); 
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n)
{
  int ind;
  if((ind = rechercheExecutable(n->fonction)) == -1)
     erreur("Erreur : fonction non declaree\n");
  if(dico.tab[ind].complement != parcours_l_exp(n->args))
    erreur("Erreur : nombre d'argument incorrecte");                                             
}
/*-------------------------------------------------------------------------*/
void parcours_instr_retour(n_instr *n)
{
  parcours_exp(n->u.retour_.expression); 
}
/*-------------------------------------------------------------------------*/
void parcours_instr_ecrire(n_instr *n)
{
  parcours_exp(n->u.ecrire_.expression); /* TODO : a verif */
	printf("\tli\t$v0, 1\n");
	printf("\tmove\t$a0, $t%d\n", next_reg);
	printf("\tsyscall\t\t\t# ecriture\n");
  printf("\tli\t$a0, \'\\n\'\n");
  printf("\tli\t$v0, 11\n");
  printf("\tsyscall\t\t\t# ecrire char\n");

}
/*-------------------------------------------------------------------------*/
int parcours_l_exp(n_l_exp *n)
{
  int cpt = 0;
  if(n){
    parcours_exp(n->tete);
    cpt = parcours_l_exp(n->queue) + 1;
  }
  return cpt; 
}
/*-------------------------------------------------------------------------*/
int parcours_exp(n_exp *n)
{
  if(n->type == varExp){
    parcours_varExp(n);
    printf("\tlw\t$t%d, %s\t\t#lit variable dans $t%d\n",next_reg, n->u.var->nom, next_reg);
  } 

  else if(n->type == opExp) parcours_opExp(n);
  else if(n->type == intExp){
    parcours_intExp(n);
    printf("\tli\t$t%d, %d\n", next_reg, n->u.entier);
  } 

  else if(n->type == appelExp) parcours_appelExp(n);
  else if(n->type == lireExp) parcours_lireExp(n);
	return next_reg;
}
/*-------------------------------------------------------------------------*/
void parcours_varExp(n_exp *n)
{
  if(rechercheExecutable(n->u.var->nom) < 0)   
    erreur("Erreur : variable inconnue\n");
  parcours_var(n->u.var);
  
}
/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n)
{
 if( n->u.opExp_.op1 != NULL ) {
    parcours_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    parcours_exp(n->u.opExp_.op2);
  } 
}
/*-------------------------------------------------------------------------*/
void parcours_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf(texte, "%d", n->u.entier);
}
/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n)
{
	printf("\tli \t$v0, 5\n");
  printf("\tsyscall\n");
  printf("\tmove \t$t%d, \t$v0\n", next_reg);
}
/*-------------------------------------------------------------------------*/
void parcours_appelExp(n_exp *n)
{
  parcours_appel(n->u.appel); 
}
/*-------------------------------------------------------------------------*/
void parcours_l_dec(n_l_dec *n)
{
  if( n ){
    parcours_dec(n->tete);
    parcours_l_dec(n->queue);
  }
}
/*-------------------------------------------------------------------------*/
void parcours_dec(n_dec *n)
{
  if(n){
    if(n->type == foncDec) {
      parcours_foncDec(n);
    }
    else if(n->type == varDec) {
      parcours_varDec(n);
    }
    else if(n->type == tabDec) { 
      parcours_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/
void parcours_foncDec(n_dec *n)
{
	int foncDecArgC;
	n_l_dec *currentParam = n -> u.foncDec_.param;
	if(rechercheDeclarative(n->nom) != -1)                                    
		erreur("Erreur : fonction deja existante\n");
	for(foncDecArgC = 0 ; currentParam != NULL ; foncDecArgC++)
		currentParam = currentParam->queue;
	ajouteIdentificateur(n->nom, contexte, T_FONCTION, 0, foncDecArgC);
	entreeFonction();
	contexte = C_ARGUMENT;
	parcours_l_dec(n->u.foncDec_.param);
	contexte = C_VARIABLE_LOCALE;
	parcours_l_dec(n->u.foncDec_.variables);
	parcours_instr(n->u.foncDec_.corps);
	sortieFonction();
}
/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n)
{				
	if(rechercheDeclarative(n->nom) != -1)
		erreur("Erreur : variable deja existante\n");
	if(contexte == C_VARIABLE_GLOBALE)
	{
		printf("\t%s:\t.space\t4\n", n->nom);
		ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseGlobaleCourante, -1);
		adresseGlobaleCourante += 4;
	}
	if(contexte == C_VARIABLE_LOCALE)
	{
		ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseLocaleCourante, -1);
		adresseLocaleCourante += 4 ;
	}
	if(contexte == C_ARGUMENT)
	{
		ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseArgumentCourant, -1);
		adresseArgumentCourant += 4 ;
	}
}
/*-------------------------------------------------------------------------*/
void parcours_tabDec(n_dec *n)
{
	char texte[100]; // Max. 100 chars nom tab + taille
	sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
	if(rechercheDeclarative(n->nom) != -1)
		erreur("Erreur : parcoursau deja existant\n");
	if(contexte == C_VARIABLE_GLOBALE)
	{
		printf("%s\t.space\t%d\n", n->nom, n->u.tabDec_.taille);
		ajouteIdentificateur(n->nom, contexte, T_TABLEAU_ENTIER, adresseGlobaleCourante, -1);
		adresseGlobaleCourante += n->u.tabDec_.taille;
	}
	if(contexte == C_VARIABLE_LOCALE)
	{
    erreur("Erreur : declaration de parcoursau interdite\n");
	}
	if(contexte == C_ARGUMENT)
	{
    erreur("Erreur : usage interdit\n");
	}
}
/*-------------------------------------------------------------------------*/
void parcours_var(n_var *n)
{
  if(n->type == simple) {
    parcours_var_simple(n);
  }
  else if(n->type == indicee) {
    parcours_var_indicee(n);
  }
}
/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n)
{
  int ind;
  if((ind = rechercheExecutable(n->nom)) == -1)
    erreur("Erreur : entier non declaree");
  if (dico.tab[ind].type != T_ENTIER)
  {
    erreur("Erreur : tableau attendu");
  }
}
/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n)
{
  int ind;
  if((ind = rechercheExecutable(n->nom)) == -1)
    erreur("Erreur : tableau non declaree");
    if (dico.tab[ind].type != T_TABLEAU_ENTIER)
    {
      erreur("Erreur : entier attendu");
    }
	parcours_exp( n->u.indicee_.indice ); 
}
/*-------------------------------------------------------------------------*/
