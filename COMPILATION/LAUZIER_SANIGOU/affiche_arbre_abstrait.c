#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dico.h"
#include "syntabs.h"
#include "util.h"


void affiche_n_prog(n_prog *n);
void affiche_l_instr(n_l_instr *n);
void affiche_instr(n_instr *n);
void affiche_instr_si(n_instr *n);
void affiche_instr_tantque(n_instr *n);
void affiche_instr_faire(n_instr *n);      /* MODIFIE POUR EVAL */
void affiche_instr_pour(n_instr *n);       /* MODIFIE POUR EVAL */
void affiche_instr_affect(n_instr *n);
void affiche_instr_appel(n_instr *n);
void affiche_instr_retour(n_instr *n);
void affiche_instr_ecrire(n_instr *n);
void affiche_l_exp(n_l_exp *n);
int affiche_exp(n_exp *n);
void affiche_varExp(n_exp *n);
void affiche_opExp(n_exp *n);
void affiche_intExp(n_exp *n);
void affiche_lireExp(n_exp *n);
void affiche_appelExp(n_exp *n);
void affiche_l_dec(n_l_dec *n);
void affiche_dec(n_dec *n);
void affiche_foncDec(n_dec *n);
void affiche_varDec(n_dec *n);
void affiche_tabDec(n_dec *n);
void affiche_var(n_var *n);
void affiche_var_simple(n_var *n);
void affiche_var_indicee(n_var *n);
void affiche_appel(n_appel *n);

int trace_abs = 0;
int trace_dico = 0;

int contexte = C_VARIABLE_GLOBALE;
int adresseLocaleCourante = 0;
int adresseArgumentCourant = 0;

int num_registre = 1;
int num_etiquette = 0;

char * new_etiquette(const char * nom){
  static char str[80];

  sprintf(str, "%s%d", nom, num_etiquette++);

  return strdup(str);
}

/*-------------------------------------------------------------------------*/

void affiche_n_prog(n_prog *n)
{
  char *fct = "prog";
  affiche_balise_ouvrante(fct, trace_abs);

  printf("\t.data\n");
  affiche_l_dec(n->variables);

  printf("\n\t.text\n");
  affiche_l_dec(n->fonctions); 

  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void affiche_l_instr(n_l_instr *n)
{
  char *fct = "l_instr";
  if(n){
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_instr(n->tete);
  affiche_l_instr(n->queue);
  affiche_balise_fermante(fct, trace_abs);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_instr(n_instr *n)
{
  if(n){
    if(n->type == blocInst) affiche_l_instr(n->u.liste);
    else if(n->type == affecteInst) affiche_instr_affect(n);
    else if(n->type == siInst) affiche_instr_si(n);
    else if(n->type == tantqueInst) affiche_instr_tantque(n);
    else if(n->type == faireInst) affiche_instr_faire(n);
    else if(n->type == pourInst) affiche_instr_pour(n);    
    else if(n->type == appelInst) affiche_instr_appel(n);
    else if(n->type == retourInst) affiche_instr_retour(n);
    else if(n->type == ecrireInst) affiche_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_instr_si(n_instr *n)
{  
  char *fct = "instr_si";
  affiche_balise_ouvrante(fct, trace_abs);

  const char * eti_si_end = new_etiquette("siend");
  const char * eti_sinon = new_etiquette("sinon");

  int test = affiche_exp(n->u.si_.test);
  if(n->u.si_.sinon)
  {
    printf("beq  $t%d, $0, %s\n", test, eti_sinon);
    affiche_instr(n->u.si_.alors);
    printf("j\t%s\n", eti_si_end);
    printf("%s:\n", eti_sinon);
    affiche_instr(n->u.si_.sinon);
  }
  else
  {
    printf("beq  $t%d, $0, %s\n", test, eti_si_end);
    affiche_instr(n->u.si_.alors);
  }

  printf("%s:\n", eti_si_end);

  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_instr_tantque(n_instr *n)
{
  char *fct = "instr_tantque";
  affiche_balise_ouvrante(fct, trace_abs);

  int test = affiche_exp(n->u.tantque_.test);

  const char * eti_tq = new_etiquette("tq");
  const char * eti_tq_end = new_etiquette("tqend");

  printf("%s:\n", eti_tq);
  printf("beq  $t%d, $0, %s\n", test, eti_tq_end);

  affiche_instr(n->u.tantque_.faire);

  printf("j\t%s\n", eti_tq);
  printf("%s:\n", eti_tq_end);

  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
  char *fct = "instr_faire";                  /* MODIFIE POUR EVAL */
  affiche_balise_ouvrante(fct, trace_abs);    /* MODIFIE POUR EVAL */
  affiche_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
  affiche_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
  affiche_balise_fermante(fct, trace_abs);    /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void affiche_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
  char *fct = "instr_pour";                        /* MODIFIE POUR EVAL */
  affiche_balise_ouvrante(fct, trace_abs);         /* MODIFIE POUR EVAL */
  affiche_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
  affiche_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
  affiche_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
  affiche_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
  affiche_balise_fermante(fct, trace_abs);         /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void affiche_instr_affect(n_instr *n)
{
  char *fct = "instr_affect";
  affiche_balise_ouvrante(fct, trace_abs);


  affiche_var(n->u.affecte_.var);
  affiche_exp(n->u.affecte_.exp);

  printf("\tsw\t$t%d, %s\t\t#enregistre la variable\n", num_registre, n->u.affecte_.var->nom);
  num_registre++;

  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_instr_appel(n_instr *n)
{
  char *fct = "instr_appel";
  affiche_balise_ouvrante(fct, trace_abs);


  affiche_appel(n->u.appel);
  affiche_balise_fermante(fct, trace_abs);
}
/*-------------------------------------------------------------------------*/

void affiche_appel(n_appel *n)
{
  char *fct = "appel";
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_texte( n->fonction, trace_abs);

  int existe = rechercheExecutable(n->fonction);

  if(existe != -1)
  {
    if(dico.tab[existe].type == T_FONCTION)
    {
      int nbArguments = 0;

      n_l_exp* l = NULL;

      for(l = n->args; l != NULL; l = l->queue){
          nbArguments++;
      }

      if(dico.tab[existe].complement != nbArguments)
      {
        //Erreur nombre d'arguments
      }
    }
    else
    {
      //n'est pas une fonction
    }
  }
  else
  {
    //Fonction non déclarée
  }

  affiche_l_exp(n->args);
  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_instr_retour(n_instr *n)
{
  char *fct = "instr_retour";
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_exp(n->u.retour_.expression);
  affiche_balise_fermante(fct, trace_abs);

}

/*-------------------------------------------------------------------------*/

void affiche_instr_ecrire(n_instr *n)
{
  char *fct = "instr_ecrire";
  affiche_balise_ouvrante(fct, trace_abs);

  affiche_exp(n->u.ecrire_.expression);

  printf("\tli\t$v0, 1\n");
  printf("\tmove\t$a0, $t%d\n", num_registre);
  printf("\tsyscall\t\t\t# ecriture\n");
  printf("\tli\t$a0, \'\\n\'\n");
  printf("\tli\t$v0, 11\n");
  printf("\tsyscall\t\t\t# ecrit caractère\n");

  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_l_exp(n_l_exp *n)
{
  char *fct = "l_exp";
  affiche_balise_ouvrante(fct, trace_abs);

  if(n){
    affiche_exp(n->tete);
    affiche_l_exp(n->queue);
  }
  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

int affiche_exp(n_exp *n)
{
  if(n->type == varExp){
    affiche_varExp(n);
    printf("\tlw\t$t%d, %s\t\t#lit variable dans $t%d\n", num_registre, n->u.var->nom, num_registre);
  }
  else if(n->type == opExp) affiche_opExp(n);
  else if(n->type == intExp){
    affiche_intExp(n);
    printf("\tli\t$t%d, %d\n", num_registre, n->u.entier);
  }
  else if(n->type == appelExp) affiche_appelExp(n);
  else if(n->type == lireExp) affiche_lireExp(n);

  return num_registre;
}

/*-------------------------------------------------------------------------*/

void affiche_varExp(n_exp *n)
{
  char *fct = "varExp";
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_var(n->u.var);
  affiche_balise_fermante(fct, trace_abs);
}

const char * etiquette = NULL;

/*-------------------------------------------------------------------------*/
void affiche_opExp(n_exp *n)
{
  char *fct = "opExp";
  affiche_balise_ouvrante(fct, trace_abs);
  if(n->u.opExp_.op == plus) affiche_texte("plus", trace_abs);
  else if(n->u.opExp_.op == moins) affiche_texte("moins", trace_abs);
  else if(n->u.opExp_.op == fois) affiche_texte("fois", trace_abs);
  else if(n->u.opExp_.op == divise) affiche_texte("divise", trace_abs);
  else if(n->u.opExp_.op == egal) affiche_texte("egal", trace_abs);
  else if(n->u.opExp_.op == diff) affiche_texte("diff", trace_abs);
  else if(n->u.opExp_.op == inf) affiche_texte("inf", trace_abs);
  else if(n->u.opExp_.op == infeg) affiche_texte("infeg", trace_abs);
  else if(n->u.opExp_.op == ou) affiche_texte("ou", trace_abs);
  else if(n->u.opExp_.op == et) affiche_texte("et", trace_abs);
  else if(n->u.opExp_.op == non) affiche_texte("non", trace_abs);  
  
  int reg_op1 = NULL;
  int reg_op2 = NULL;

  if( n->u.opExp_.op1 != NULL ) {
    reg_op1 = affiche_exp(n->u.opExp_.op1);
    //num_registre++;
  }
  
  if(n->u.opExp_.op == ou)
  {
    etiquette = new_etiquette("ou");
    printf("\tli\t$t0,\t-1\t#OU\n");
    printf("\tbeq\t$t%d,\t$t0,\t%s\n", reg_op1, etiquette);
    printf("\tli\t$t0,\t0\n");
  }
  else if(n->u.opExp_.op == et)
  {
    etiquette = new_etiquette("et");
    printf("\tli\t$t0,\t0\t#ET\n");
    printf("\tbeq\t$t%d,\t$t0,\t%s\n", reg_op1, etiquette);
    printf("\tli\t$t0,\t0\n");
  }

  if( n->u.opExp_.op2 != NULL ) {
    reg_op2 = affiche_exp(n->u.opExp_.op2);
    //num_registre++;
  }

  if(n->u.opExp_.op == ou || n->u.opExp_.op == et)
  {
    printf("\t%s:\n", etiquette);
    printf("\tmove\t$t%d,\t$t0\n", num_registre);
  }



  //printf("1: %d | 2: %d | current : %d -------------------------\n", reg_op1, reg_op2, num_registre);

  if(n->u.opExp_.op == plus)
    printf("\tadd\t$t%d,\t$t%d,\t$t%d\t#addition\n", num_registre, reg_op1, reg_op2);
  else if(n->u.opExp_.op == moins)
    printf("\tsub\t$t%d,\t$t%d,\t$t%d\t#soustraction\n", num_registre, reg_op1, reg_op2);
  else if(n->u.opExp_.op == fois)
  {
    printf("\tmult\t$t%d,\t$t%d\t#multiplication\n", reg_op1, reg_op2);
    printf("\tmflo\t$t%d\t\t#récupère le résultat dans Lo\n", num_registre);
  }
  else if(n->u.opExp_.op == divise)
  {
    printf("\tdiv\t$t%d,\t$t%d\t#division\n", reg_op1, reg_op2);
    printf("\tmflo\t$t%d\t\t#récupère le résultat dans Lo\n", num_registre);
  }
  else if(n->u.opExp_.op == egal)
  {
    const char * egal_vrai = new_etiquette("egal");

    printf("\tli\t$t0,\t-1\n");
    printf("\tbeq\t$t%d,\t$t%d,\t%s\t#test égalité\n", reg_op1, reg_op2, egal_vrai);
    printf("\tli\t$t0,\t0\n");
    printf("\t%s:\n", egal_vrai);
    printf("\tmove\t$t%d,\t$t0\n", num_registre);
  }
  else if(n->u.opExp_.op == diff)
  {
    //Pas dans l'analyseur lexical
  }
  else if(n->u.opExp_.op == inf)
  {
    const char * inf_vrai = new_etiquette("inf");

    printf("\tli\t$t0,\t-1\n");
    printf("\tblt\t$t%d,\t$t%d,\t%s\t#test inférieur\n", reg_op1, reg_op2, inf_vrai);
    printf("\tli\t$t0,\t0\n");
    printf("\t%s:\n", inf_vrai);
    printf("\tmove\t$t%d,\t$t0\n", num_registre);
  }
  else if(n->u.opExp_.op == infeg)
  {
    //pas dans l'analyseur lexical
  }
  else if(n->u.opExp_.op == non)
  {
    
  }

  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_intExp(n_exp *n)
{
  char texte[ 50 ]; // Max. 50 chiffres
  sprintf(texte, "%d", n->u.entier);
  affiche_element( "intExp", texte, trace_abs );
}

/*-------------------------------------------------------------------------*/
void affiche_lireExp(n_exp *n)
{
  char *fct = "lireExp";
  affiche_balise_ouvrante(fct, trace_abs);

  printf("\tli \t$v0, 5\n");
  printf("\tsyscall\n");
  printf("\tmove \t$t%d, \t$v0\n", num_registre);

  affiche_balise_fermante(fct, trace_abs);

}

/*-------------------------------------------------------------------------*/

void affiche_appelExp(n_exp *n)
{
  char *fct = "appelExp";
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_appel(n->u.appel);
  affiche_balise_fermante(fct, trace_abs);
}

/*-------------------------------------------------------------------------*/

void affiche_l_dec(n_l_dec *n)
{
  char *fct = "l_dec";

  if( n ){
    affiche_balise_ouvrante(fct, trace_abs);
    affiche_dec(n->tete);
    affiche_l_dec(n->queue);
    affiche_balise_fermante(fct, trace_abs);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_dec(n_dec *n)
{

  if(n){
    if(n->type == foncDec) {
      affiche_foncDec(n);
    }
    else if(n->type == varDec) {
      affiche_varDec(n);
    }
    else if(n->type == tabDec) { 
      affiche_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/

void affiche_foncDec(n_dec *n)
{
  char *fct = "foncDec";
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_texte( n->nom, trace_abs );

  int existe = rechercheDeclarative(n->nom);

  if(existe == -1)
  {
    int nbArguments = 0;

    n_l_dec* l = NULL;

    for(l = n->u.foncDec_.param; l != NULL; l = l->queue){
      nbArguments++;
    }

    ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_FONCTION, 0, nbArguments);
    entreeFonction();

    contexte = C_ARGUMENT;    

    affiche_l_dec(n->u.foncDec_.param);

    contexte = C_VARIABLE_LOCALE;

    affiche_l_dec(n->u.foncDec_.variables);
    affiche_instr(n->u.foncDec_.corps);

    affiche_balise_fermante(fct, trace_abs);

    if(trace_dico)
      affiche_dico();

    sortieFonction();
  }
  else
  {
    //Erreur déjà déclarée
  }
}

/*-------------------------------------------------------------------------*/

void affiche_varDec(n_dec *n)
{
  affiche_element("varDec", n->nom, trace_abs);

  int declare = rechercheDeclarative(n->nom);
  if(declare == -1)
  {
    int globale = rechercheExecutable(n->nom);
    if(globale == -1)
    {
      int adresse = 0;
      if(contexte == C_ARGUMENT)
      {
        adresse = adresseArgumentCourant;
        adresseArgumentCourant = adresseArgumentCourant + 4;
      }
      else if(contexte == C_VARIABLE_GLOBALE)
      {
        adresse = adresseLocaleCourante;
        adresseLocaleCourante = adresseLocaleCourante + 4;
      }
      else
      {
        adresse = adresseLocaleCourante;
        adresseLocaleCourante = adresseLocaleCourante + 4;
      }
      printf("\t%s:\t.space\t4\n", n->nom);

      ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresse, -1);
    }
    else
    {
      printf("Erreur : la variable %s est déjà déclarée en globale\n", n->nom);
      exit(1);
    }
  }
  else
  {
    printf("Erreur : la variable %s est déjà déclarée\n", n->nom);
    exit(1);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_tabDec(n_dec *n)
{
  char texte[100]; // Max. 100 chars nom tab + taille
  sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
  affiche_element( "tabDec", texte, trace_abs );

  if(contexte == C_VARIABLE_GLOBALE)
  {
    int existe = rechercheDeclarative(n->nom);

    if(existe == -1)
    {
      printf("\t%s:\t.space\t%d\n", n->nom, n->u.tabDec_.taille*4);
      ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_TABLEAU_ENTIER, adresseLocaleCourante, n->u.tabDec_.taille);
      adresseLocaleCourante += (4*n->u.tabDec_.taille);
    }
    else
    {
      printf("Erreur : la variable %s est déjà déclarée\n", n->nom);
      exit(1);
    }
  }
  else
  {
    printf("Erreur : la variable %s doit être déclarée en globale\n", n->nom);
    exit(1);
  }
}

/*-------------------------------------------------------------------------*/

void affiche_var(n_var *n)
{
  if(n->type == simple) {
    affiche_var_simple(n);
  }
  else if(n->type == indicee) {
    affiche_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void affiche_var_simple(n_var *n)
{
  affiche_element("var_simple", n->nom, trace_abs);

  int existe = rechercheExecutable(n->nom);
  if(existe == -1)
  {
    printf("Erreur : la variable %s n'est pas déclarée\n", n->nom);
    exit(1);
  }
  else
  {
    if(dico.tab[existe].type != T_ENTIER)
    {
      printf("Erreur : la variable %s est un entier\n", n->nom);
    exit(1);
    }
  }
}

/*-------------------------------------------------------------------------*/
void affiche_var_indicee(n_var *n)
{
  char *fct = "var_indicee";
  affiche_balise_ouvrante(fct, trace_abs);
  affiche_element("var_base_tableau", n->nom, trace_abs);
  affiche_exp( n->u.indicee_.indice );
  affiche_balise_fermante(fct, trace_abs);

  int existe = rechercheExecutable(n->nom);
  if(existe == -1)
  {
    printf("Erreur : la variable %s n'est pas déclarée\n", n->nom);
    exit(1);
  }
  else
  {
    if(dico.tab[existe].type != T_TABLEAU_ENTIER)
    {
      printf("Erreur : la variable %s doit être un entier\n", n->nom);
    exit(1);
    }
  }
}
/*-------------------------------------------------------------------------*/
