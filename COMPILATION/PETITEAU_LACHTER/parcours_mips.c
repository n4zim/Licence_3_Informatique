#include <stdio.h>
#include <stdlib.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

void parcours_n_prog(n_prog *n);
void parcours_l_instr(n_l_instr *n);
void parcours_instr(n_instr *n);
void parcours_instr_si(n_instr *n);
void parcours_instr_tantque(n_instr *n);
void parcours_instr_faire(n_instr *n);      /* MODIFIE POUR EVAL */
void parcours_instr_pour(n_instr *n);       /* MODIFIE POUR EVAL */
void parcours_instr_affect(n_instr *n);
void parcours_instr_appel(n_instr *n);
void parcours_instr_retour(n_instr *n);
void parcours_instr_ecrire(n_instr *n);
void parcours_l_exp(n_l_exp *n);
void parcours_exp(n_exp *n);
void parcours_varExp(n_exp *n);
void parcours_opExp(n_exp *n);
void parcours_intExp(n_exp *n);
void parcours_lireExp(n_exp *n);
void parcours_appelExp(n_exp *n);
void parcours_l_dec(n_l_dec *n);
void parcours_dec(n_dec *n);
void parcours_foncDec(n_dec *n);
void parcours_varDec(n_dec *n);
void parcours_tabDec(n_dec *n);
void parcours_var(n_var *n);
void parcours_var_simple(n_var *n);
void parcours_var_indicee(n_var *n);
void parcours_appel(n_appel *n);

int courantVar = C_VARIABLE_GLOBALE;
int courantAdrLoc = 0;
int courantAdrArg = 0;
static int courantLabel = 0;


void pop_reg(const char* r) {
    printf("\tlw %s, 0($sp)\n", r);
    printf("\taddi $sp, $sp, 4\n");
}

void stack_reg(const char* r) {
    printf("\tsubi $sp, $sp, 4\n");
    printf("\tsw %s, 0($sp)\n", r);
}

/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n) {

    printf(".data\n");

    parcours_l_dec(n->variables);

    printf("\n.text\n");
    //printf("\n__start:\n\tjal main\n\tli $v0, 10\n\tsyscall\n");

    parcours_l_dec(n->fonctions);   
}

/*-------------------------------------------------------------------------*/

void parcours_l_instr(n_l_instr *n) {
    if(n) {    
        parcours_instr(n->tete);
        parcours_l_instr(n->queue);        
    }
}

/*-------------------------------------------------------------------------*/

void parcours_instr(n_instr *n) {
    if(n) {
        if(n->type == blocInst)
            parcours_l_instr(n->u.liste);
        else if(n->type == affecteInst)
            parcours_instr_affect(n);
        else if(n->type == siInst)
            parcours_instr_si(n);
        else if(n->type == tantqueInst)
            parcours_instr_tantque(n);
        else if(n->type == faireInst)
            parcours_instr_faire(n);
        else if(n->type == pourInst)
            parcours_instr_pour(n);
        else if(n->type == appelInst)
            parcours_instr_appel(n);
        else if(n->type == retourInst)
            parcours_instr_retour(n);
        else if(n->type == ecrireInst)
            parcours_instr_ecrire(n);
    }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n) {
    parcours_exp(n->u.si_.test);
    parcours_instr(n->u.si_.alors);
    if(n->u.si_.sinon) parcours_instr(n->u.si_.sinon);    
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n) {
    parcours_exp(n->u.tantque_.test);
    parcours_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
parcours_instr(n->u.faire_.faire);          /* MODIFIE POUR EVAL */
parcours_exp(n->u.faire_.test);             /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcours_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
parcours_instr(n->u.pour_.init);                 /* MODIFIE POUR EVAL */
parcours_exp(n->u.pour_.test);                   /* MODIFIE POUR EVAL */
parcours_instr(n->u.pour_.faire);                /* MODIFIE POUR EVAL */
parcours_instr(n->u.pour_.incr);                 /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void parcours_instr_affect(n_instr *n) {

    parcours_var(n->u.affecte_.var);
    parcours_exp(n->u.affecte_.exp);

    printf("\n\n");

    n_var* var = n->u.affecte_.var;

    int i = rechercheExecutable(var->nom);
    if(i == -1) exit(1);

    if(var->type == simple) {
        pop_reg("$t1");

        switch(dico.tab[i].classe){
            case C_VARIABLE_GLOBALE:
                printf("\tsw $t1, %s\n", var->nom);
                break;
            case C_VARIABLE_LOCALE:
                printf("\tsw $t1, -%d($fp)\n", dico.tab[i].adresse+8);
                break;
            default:
                exit(1);
        }
    } else {
        parcours_exp(var->u.indicee_.indice);

        pop_reg("$t0");

        printf("\tadd $t0, $t0, $t0\n");
        printf("\tadd $t0, $t0, $t0\n");

        pop_reg("$t1");

        switch(dico.tab[i].classe){
            case C_VARIABLE_GLOBALE:
                printf("\tsw $t1, %s($t0)\n", var->nom);
                break;
            default:
                exit(1);
        }
    }   
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n) {

    parcours_appel(n->u.appel);
    
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n) {
    int existe = rechercheExecutable(n->fonction);
    if(existe != -1) {
        if(dico.tab[existe].type == T_FONCTION) {
            int nbArgs = 0;
            n_l_exp* l = NULL;
            for(l = n->args;l != NULL;l = l->queue) nbArgs++;
            if(dico.tab[existe].complement != nbArgs) {
                printf("\n--> /!\\ Erreur sur le nombre d'arguments\n");
            }
        } else {
            printf("\n--> /!\\ %s n'est pas une fonction\n", n->fonction);
        }
    } else {
        printf("\n--> /!\\ Fonction non déclarée\n");
    }
    parcours_l_exp(n->args);
    
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n) {

    parcours_exp(n->u.retour_.expression);
    
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n) {
    parcours_exp(n->u.ecrire_.expression);

    printf("\n\n");

    pop_reg("$a0");

    printf("\tli $v0, 1\n");
    printf("\tsyscall\n");

    printf("\tli, $v0, '\\n'\n");
    printf("\tli $v0, 11\n");
    printf("\tsyscall\n");
}

/*-------------------------------------------------------------------------*/

void parcours_l_exp(n_l_exp *n) {

    if(n) {
        parcours_exp(n->tete);
        parcours_l_exp(n->queue);
    }
    
}

/*-------------------------------------------------------------------------*/

void parcours_exp(n_exp *n) {
    if(n->type == varExp) parcours_varExp(n);
    else if(n->type == opExp) parcours_opExp(n);
    else if(n->type == intExp) parcours_intExp(n);
    else if(n->type == appelExp) parcours_appelExp(n);
    else if(n->type == lireExp) parcours_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void parcours_varExp(n_exp *n) {
    parcours_var(n->u.var);

    char* nom = n->u.var->nom;

    int i = rechercheExecutable(nom);
    if(i==-1) exit(1);
    
    if(n->u.var->type == simple) {
        printf("\n\n");
        switch(dico.tab[i].classe){
            case C_VARIABLE_GLOBALE:
                printf("\tlw $t1, %s\n", nom);
                break;
            case C_ARGUMENT:
                printf("\tlw $t1, %d($fp)\n", (courantAdrArg - dico.tab[i].adresse));
                break;
            case C_VARIABLE_LOCALE:
                printf("\tlw $t1, -%d($fp)\n", (dico.tab[i].adresse+8));
                break;
            default:
                exit(1);
        }
        stack_reg("$t1");
    } else {
        parcours_exp(n->u.var->u.indicee_.indice);

        printf("\n\n");

        pop_reg("$t0");

        printf("\tadd $t0, $t0, $t0\n");
        printf("\tadd $t0, $t0, $t0\n");

        switch(dico.tab[i].classe) {
            case C_VARIABLE_GLOBALE:
                printf("\tlw $t1, %s($t0)\n", nom);
                break;
            default:
                exit(1);
        }

        stack_reg("$t1");
    } 
}

/*-------------------------------------------------------------------------*/

void parcours_opExp(n_exp *n) {

    if( n->u.opExp_.op1 != NULL ) parcours_exp(n->u.opExp_.op1);
    if( n->u.opExp_.op2 != NULL ) parcours_exp(n->u.opExp_.op2);

    printf("\n\n");

    switch(n->u.opExp_.op){
        case inf:
            courantLabel++;

            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tli $t2, -1\n");
            printf("\tblt $t0, $t1, e%d\n", courantLabel);
            printf("\tli $t2, 0\n");

            printf("e%d:\n",courantLabel);

            stack_reg("$t2");
            break;

        case fois:
            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tmult $t0, $t1\n");
            printf("\tmflo $t2\n");

            stack_reg("$t2");
            break;

        case divise:
            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tdiv $t0, $t1\n");
            printf("\tmflo $t2\n");

            stack_reg("$t2");
            break;

        case plus:
            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tadd $t2, $t0, $t1\n");

            stack_reg("$t2");
            break;

        case moins:
            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tsub $t2, $t0, $t1\n");

            stack_reg("$t2");
            break;

        case et:
            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tand $t2, $t0, $t1\n");

            stack_reg("$t2");
            break;

        case ou:
            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tor $t2, $t0, $t1\n");

            stack_reg("$t2");
            break;

        case egal:
            courantLabel++;

            pop_reg("$t1");
            pop_reg("$t0");

            printf("\tli $t2, -1\n");
            printf("\tbeq $t0, $t1, e%d\n", courantLabel);
            printf("\tli $t2, 0\n");
            printf("e%d:\n",courantLabel);
            stack_reg("$t2");
            break;

        case non:
            pop_reg("$t0");

            printf("\tli $t1, -1\n");
            printf("\txor $t2, $t0, $t1\n");

            stack_reg("$t2");
            break;

        case negatif:
            pop_reg("$t0");

            printf("\tsub $t2, $zero, $t0\n");

            stack_reg("$t2");
            break;

        default:
            exit(1);
    }
    
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n) {
    printf("\tli $t0, %d\n", n->u.entier);
    stack_reg("$t0");
}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n) {
    printf("\tli $v0, 5\n");
    printf("\tsyscall\n");
    
    stack_reg("$v0");
}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n) {
    parcours_appel(n->u.appel);
    
}

/*-------------------------------------------------------------------------*/

void parcours_l_dec(n_l_dec *n) {
    if( n ) {
    
        parcours_dec(n->tete);
        parcours_l_dec(n->queue);
        
    }
}

/*-------------------------------------------------------------------------*/

void parcours_dec(n_dec *n) {
    if(n){
        if(n->type == foncDec) parcours_foncDec(n);
            else if(n->type == varDec) parcours_varDec(n);
            else if(n->type == tabDec) parcours_tabDec(n);
    }
}

/*-------------------------------------------------------------------------*/

void parcours_foncDec(n_dec *n) {

    int existe = rechercheDeclarative(n->nom);
    if(existe == -1) {
        int nbArgs = 0;
        n_l_dec* l = NULL;
        
        for(l = n->u.foncDec_.param;l != NULL;l = l->queue) nbArgs++;
        ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_FONCTION, 0, nbArgs);

        entreeFonction();

        courantVar = C_ARGUMENT;
        parcours_l_dec(n->u.foncDec_.param);
        courantVar = C_VARIABLE_LOCALE;
        parcours_l_dec(n->u.foncDec_.variables);

        parcours_instr(n->u.foncDec_.corps);
        

        //parcours_dico();

        sortieFonction();
    } else {
        printf("\n--> /!\\ Fonction déjà déclarée\n");
    }
}

/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n) {
    int declare = rechercheDeclarative(n->nom);
    if(declare == -1) {
        int globale = rechercheExecutable(n->nom);
        if(globale == -1) {
            int adresse = 0;
            if(courantVar == C_ARGUMENT) {
                adresse = courantAdrArg;
                courantAdrArg = courantAdrArg + 4;
            } else {
                adresse = courantAdrLoc;
                courantAdrLoc = courantAdrLoc + 4;
            }
            ajouteIdentificateur(n->nom, courantVar, T_ENTIER, adresse, -1);
        } else {
            printf("Erreur : la variable %s est déjà déclarée en globale\n", n->nom);
            exit(1);
        }
    } else {
        printf("Erreur : la variable %s est déjà déclarée\n", n->nom);
        exit(1);
    }

    if(courantVar==C_VARIABLE_GLOBALE) printf("%s:\t.space\t4\n", n->nom);
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n) {
    char texte[100]; // Max. 100 chars nom tab + taille
    sprintf(texte, "%s[%d]", n->nom, n->u.tabDec_.taille);
    if(courantVar == C_VARIABLE_GLOBALE) {
        int existe = rechercheDeclarative(n->nom);

        if(existe == -1) {
            ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_TABLEAU_ENTIER,
                courantAdrLoc, n->u.tabDec_.taille);
            courantAdrLoc += (4*n->u.tabDec_.taille);
        } else {
            printf("Erreur : la variable %s est déjà déclarée\n", n->nom);
            exit(1);
        }
    } else {
        printf("Erreur : la variable %s doit être déclarée en globale\n", n->nom);
        exit(1);
    }
}

/*-------------------------------------------------------------------------*/

void parcours_var(n_var *n) {
    if(n->type == simple) parcours_var_simple(n);
        else if(n->type == indicee) parcours_var_indicee(n);
}

/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n) {
    int existe = rechercheExecutable(n->nom);
    if(existe == -1) {
        printf("Erreur : la variable %s n'est pas déclarée\n", n->nom);
        exit(1);
    } else if(dico.tab[existe].type != T_ENTIER) {
        printf("Erreur : la variable %s est un entier\n", n->nom);
        exit(1);
    }
}

/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n) {
    parcours_exp( n->u.indicee_.indice );
    
    int existe = rechercheExecutable(n->nom);
    if(existe == -1) {
        printf("Erreur : la variable %s n'est pas déclarée\n", n->nom);
        exit(1);
    } else if(dico.tab[existe].type != T_TABLEAU_ENTIER) {
        printf("Erreur : la variable %s doit être un entier\n", n->nom);
        exit(1);
    }
}
