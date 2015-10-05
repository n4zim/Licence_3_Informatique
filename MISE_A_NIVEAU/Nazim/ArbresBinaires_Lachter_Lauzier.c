#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct node {
    unsigned int val;
    struct node *gauche;
    struct node *droite;
} node;

node *recherche(int val, node *noeud) {
	if(NULL != noeud) {
		if(val == noeud->val) return noeud;
		else if(val < noeud->val) return recherche(val, noeud->gauche);
		else return recherche(val, noeud->droite);
	}

	return 0;
}

void insertion(int val, node **noeud) {
	if (NULL == *noeud) {
		*noeud = malloc(sizeof(node));
		(*noeud)->val = val;
		(*noeud)->gauche = NULL;
		(*noeud)->droite = NULL;
	
	} else if(val < (*noeud)->val) insertion(val, &(*noeud)->gauche);

	else if(val > (*noeud)->val) insertion(val, &(*noeud)->droite);
}

node *min_val(node *noeud) {
    node *noeud_courant = noeud;

    while (NULL != noeud_courant->gauche) noeud_courant = noeud_courant->gauche;

    return noeud_courant;
}

node *suppressionV1(int val, node *noeud) {
	if(NULL == noeud) return noeud;

	if(val < noeud->val) noeud->gauche = suppressionV1(val, noeud->gauche);

	else if(val > noeud->val) noeud->droite = suppressionV1(val, noeud->droite);

	else {
		if(NULL == noeud->gauche) {
			node *temp = noeud->droite;
			free(noeud);
			return temp;
		
		} else if(NULL == noeud->droite) {
			node *temp = noeud->gauche;
			free(noeud);
			return temp;
		}
		
		node *temp = min_val(noeud->droite);
        noeud->val = temp->val;
        noeud->droite = suppressionV1(temp->val, noeud->droite);
	}

	return noeud;
}

node *suppressionV2(int val, node *noeud) {
	node *temp;

	if(val == noeud->val) {
	   if(NULL != noeud->gauche) {
		  temp = noeud->gauche;
		  while(temp->droite != NULL) temp = temp->droite;
		  temp->droite = noeud->droite;
		  noeud = noeud->gauche;
	   
	   } else { noeud = noeud->droite; }

	} else {
		if(val < noeud->val) noeud->gauche = suppressionV2(val, noeud->gauche);
		else noeud->droite = suppressionV2(val, noeud->droite);
	}
	
	return noeud;
}

void afficher(node *noeud) {
	int ng, nd;
	if(NULL != noeud->gauche) ng = noeud->gauche->val; else ng = 0;
	if(NULL != noeud->droite) nd = noeud->droite->val; else nd = 0;
	
	printf("%d\" ( %d, %d ) \n", noeud->val, ng, nd);
	if(NULL != noeud->gauche) {
		printf("\"");
		afficher(noeud->gauche);
	}

	if(NULL != noeud->droite) {
		printf("\"");
		afficher(noeud->droite);
	}
}

int main(int argc, char **argv) {
	node *pere = NULL;
	
	int i; for(i = 0; i <= 5; i++) insertion((rand()%100), &pere);
	insertion(51, &pere);
	for(i = 0; i <= 5; i++) insertion((rand()%100), &pere);
	
	printf("Affichage des fils du noeud contenant \""); afficher(pere);
	
	node *resultat = recherche(51, pere);
	printf("\nAffichage des fils - si existants - du noeud contenant \""); afficher(resultat);
	
	/*
	suppressionV2(10, pere);
	printf("\nAffichage - après suppression (V2) du noeud contenant 10 - du noeud contenant \""); afficher(pere);
	suppressionV1(51, pere);
	printf("\nAffichage - après suppression (V1) du noeud contenant 51 - du noeud contenant \""); afficher(pere);
	*/
	
	return 0;
}
