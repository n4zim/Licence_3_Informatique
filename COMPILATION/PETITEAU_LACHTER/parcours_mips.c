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
void parcours_appel(n_appel *n, int);

int courantVar = C_VARIABLE_GLOBALE;
int courantAdrLoc = 0;
int courantAdrArg = 0;
static int courantLabel = 0;


void depile(const char* r) {
    printf("\tlw %s, 0($sp)\n", r);
    printf("\taddi $sp, $sp, 4\n");
}

void empile(const char* r) {
    printf("\tsubi $sp, $sp, 4\n");
    printf("\tsw %s, 0($sp)\n", r);
}

/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n) {

    printf(".data\n");

    parcours_l_dec(n->variables);

    printf("\n.text\n");
    printf("\n__start:\n");
    printf("\tjal main\n");
    printf("\tli $v0, 10\n");
    printf("\tsyscall\n");

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
    int sinon = courantLabel;
    courantLabel++;
    int finSi = courantLabel;
    courantLabel++;

    parcours_exp(n->u.si_.test);

    depile("$t0");

    if(n->u.si_.sinon) printf("\tbeq $t0, $zero, e%d\n", sinon);
    else printf("\tbeq $t0, $zero, e%d\n", finSi);

    parcours_instr(n->u.si_.alors);

    if(n->u.si_.sinon) {
        printf("\tj e%d\n", finSi);
        printf("\ne%d:\n", sinon);

        parcours_instr(n->u.si_.sinon);
    }

    printf("\ne%d:\n", finSi);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n) {
    int debutTq = courantLabel;
    courantLabel++;
    int finTq = courantLabel;
    courantLabel++;

    printf("\ne%d:\n", debutTq);

    parcours_exp(n->u.tantque_.test);

    depile("$t0");
    printf("\tbeq $t0, $zero, e%d\n", finTq);

    parcours_instr(n->u.tantque_.faire);

    printf("\tj e%d\n", debutTq);
    printf("\ne%d:\n", finTq);

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

    n_var* var = n->u.affecte_.var;

    int i = rechercheExecutable(var->nom);
    if(i == -1) exit(1);

    if(var->type == simple) {
        depile("$t1");

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

        depile("$t0");

        printf("\tadd $t0, $t0, $t0\n");
        printf("\tadd $t0, $t0, $t0\n");

        depile("$t1");

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

    parcours_appel(n->u.appel, 0);
    
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n, int estExp) {
    int existe = rechercheExecutable(n->fonction);
    if(existe != -1) {
        if(dico.tab[existe].type == T_FONCTION) {
            printf("\tsubi $sp, $sp, 4\n");

            int nbArgs = 0;

            n_l_exp* l = NULL;

            for(l = n->args;l != NULL;l = l->queue) {
                nbArgs++;
                parcours_exp(l->tete);
            }

            if(dico.tab[existe].complement != nbArgs) printf("\n--> /!\\ Erreur sur le nombre d'arguments\n");

            printf("\tjal %s\n", n->fonction);

            if(nbArgs > 0) printf("\taddi $sp, $sp, %d\n", nbArgs*4);

            if(!estExp) printf("\taddi $sp, $sp, 4\n");

        } else {
            printf("\n--> /!\\ %s n'est pas une fonction\n", n->fonction);
        }
    } else {
        printf("\n--> /!\\ Fonction non déclarée\n");
    }
    //parcours_l_exp(n->args);
    
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n) {
    parcours_exp(n->u.retour_.expression);

    depile("$t0");
    printf("\tsw $t0, %d($fp)\n", courantAdrArg+4);

    if(courantAdrLoc != 0) printf("\taddi $sp, $sp, %d\n", courantAdrLoc);

    depile("$ra");
    depile("$fp");

    printf("\tjr $ra\n");
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n) {
    parcours_exp(n->u.ecrire_.expression);

    depile("$a0");

    printf("\tli $v0, 1\n");
    printf("\tsyscall\n");

    printf("\tli, $a0, '\\n'\n");
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
        empile("$t1");
    } else {
        parcours_exp(n->u.var->u.indicee_.indice);

        depile("$t0");

        printf("\tadd $t0, $t0, $t0\n");
        printf("\tadd $t0, $t0, $t0\n");

        switch(dico.tab[i].classe) {
            case C_VARIABLE_GLOBALE:
                printf("\tlw $t1, %s($t0)\n", nom);
                break;
            default:
                exit(1);
        }

        empile("$t1");
    } 
}

/*-------------------------------------------------------------------------*/

void parcours_opExp(n_exp *n) {

    if( n->u.opExp_.op1 != NULL ) parcours_exp(n->u.opExp_.op1);
    if( n->u.opExp_.op2 != NULL ) parcours_exp(n->u.opExp_.op2);

    switch(n->u.opExp_.op){
        case inf:
            depile("$t1");
            depile("$t0");

            printf("\tli $t2, -1\n");
            printf("\tblt $t0, $t1, e%d\n", courantLabel);
            printf("\tli $t2, 0\n");

            printf("\ne%d:\n",courantLabel);

            empile("$t2");

            courantLabel++;
            break;

        case fois:
            depile("$t1");
            depile("$t0");

            printf("\tmult $t0, $t1\n");
            printf("\tmflo $t2\n");

            empile("$t2");
            break;

        case divise:
            depile("$t1");
            depile("$t0");

            printf("\tdiv $t0, $t1\n");
            printf("\tmflo $t2\n");

            empile("$t2");
            break;

        case plus:
            depile("$t1");
            depile("$t0");

            printf("\tadd $t2, $t0, $t1\n");

            empile("$t2");
            break;

        case moins:
            depile("$t1");
            depile("$t0");

            printf("\tsub $t2, $t0, $t1\n");

            empile("$t2");
            break;

        case et:
            depile("$t1");
            depile("$t0");

            printf("\tand $t2, $t0, $t1\n");

            empile("$t2");
            break;

        case ou:
            depile("$t1");
            depile("$t0");

            printf("\tor $t2, $t0, $t1\n");

            empile("$t2");
            break;

        case egal:
            depile("$t1");
            depile("$t0");

            printf("\tli $t2, -1\n");
            printf("\tbeq $t0, $t1, e%d\n", courantLabel);
            printf("\tli $t2, 0\n");
            printf("\ne%d:\n",courantLabel);
            empile("$t2");
            courantLabel++;
            break;

        case non:
            depile("$t0");

            printf("\tli $t1, -1\n");
            printf("\txor $t2, $t0, $t1\n");

            empile("$t2");
            break;

        case negatif:
            depile("$t0");

            printf("\tsub $t2, $zero, $t0\n");

            empile("$t2");
            break;

        default:
            exit(1);
    }
    
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n) {
    printf("\tli $t0, %d\n", n->u.entier);
    empile("$t0");
}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n) {
    printf("\tli $v0, 5\n");
    printf("\tsyscall\n");
    
    empile("$v0");
}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n) {
    parcours_appel(n->u.appel, 1);
    
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

        printf("\n%s:\n", n->nom);
        empile("$fp");
        printf("\tmove $fp, $sp\n");
        empile("$ra");

        entreeFonction();

        courantVar = C_ARGUMENT;
        parcours_l_dec(n->u.foncDec_.param);
        courantVar = C_VARIABLE_LOCALE;
        parcours_l_dec(n->u.foncDec_.variables);

        if(courantAdrLoc!=0) printf("\tsubi $sp, $sp, %d\n", courantAdrLoc);

        parcours_instr(n->u.foncDec_.corps);

        sortieFonction();

        if(courantAdrLoc!=0) printf("\taddi $sp, $sp, %d\n", courantAdrLoc);
        depile("$ra");
        depile("$fp");
        printf("\tjr $ra\n");
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

            if(courantVar==C_VARIABLE_GLOBALE) printf("%s: .space 4\n", n->nom);

        } else {
            printf("Erreur : la variable %s est déjà déclarée en globale\n", n->nom);
            exit(1);
        }
    } else {
        printf("Erreur : la variable %s est déjà déclarée\n", n->nom);
        exit(1);
    }
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n) {
    if(courantVar == C_VARIABLE_GLOBALE) {
        int existe = rechercheDeclarative(n->nom);

        if(existe == -1) {
            ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_TABLEAU_ENTIER, courantAdrLoc, n->u.tabDec_.taille);
            courantAdrLoc += (4*n->u.tabDec_.taille);

            printf("%s: .space %d\n", n->nom, 4*n->u.tabDec_.taille);
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
