#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graphes_lecture.h"


/***************************************************
 * 	         Fonction CreerMaillon()               *
 **************************************************/

liste CreerMaillon(int v, liste s, int w) {
      liste p = malloc(sizeof(struct maillon));
      assert(p != NULL);
      p->val = v;
      p->suiv = s;
      p->w = w;
      return p;
}

/*******************************************
 * 	         Fonction Lire()               *
 ******************************************/

liste * lire_graphe (int *n, int *m, int *max) {
      int i, u, v, poids, naretes = 0;
      liste som;
      liste * V;
      
      *max = 0;
      
      /* lecture du nombre de sommets et d'aretes */
      scanf ("%d", n);
      /* allocation du tableau de pointeurs */
      V = malloc(*n*sizeof(liste));
      assert (V != NULL);
      
      printf("====== NOMBRES ENTRES ======\n");
      
      while (scanf("%d %d %d",&u, &v, &poids) == 3) {
            
            printf("%d %d %d\n", u, v, poids);
            
            if (poids > *max) *max = poids;
            
            V[u] = CreerMaillon( v, V[u], poids);
            V[v] = CreerMaillon( u, V[v], poids);
            naretes++;
      }
      *m = naretes;
      
      return V;
}



/***********************************************
 * 	         Algo de Dijkstra                  *
 **********************************************/
void Dijkstra(liste *Voisins, int sommets, int max) {
	/*int i; for(i = 0; i <= sommets; i++) {
		if(NULL != Voisins[i]) {
			//printf("%d(%d)  ", Voisins[i]->val, Voisins[i]->w);
			while(Voisins[i] != NULL) {
				printf("%d(%d)  ", Voisins[i]->val, Voisins[i]->w);
				Voisins[i]->suiv;
			}
		}
	}*/
	int* distances = malloc(sizeof(int) * sommets);
	int* F = malloc(sizeof(int) * sommets);
	int* prec = malloc(sizeof(int) * sommets);
	int taille=sommets, i, depart=0;

	for(i = 0; i < sommets; ++i) {
		distances[i] = sommets * max + 1;
		F[i] = i;
		prec[i] = -1;
	}

	distances[depart] = 0;
	
	while(taille > 0) {
		int minIndex = 0;
		int minDistance = distances[F[0]];

		for(i = 1; i < taille; ++i){
			int curDist = distances[F[i]];

			if(curDist < minDistance){
				minDistance = curDist;
				minIndex = i;
			}
		}

		int minSommet = F[minIndex];
		if(--taille > 0) F[minIndex] = F[taille];

		liste VoisinsMin = Voisins[minSommet];
		while(VoisinsMin != NULL) {
			if(distances[VoisinsMin->val] > distances[minSommet] + VoisinsMin->w) {
				distances[VoisinsMin->val] = distances[minSommet] + VoisinsMin->w;
				prec[VoisinsMin->val] = minSommet;
			}
			VoisinsMin = VoisinsMin->suiv;
		}
	}

	for(i = 1; i < sommets; i++) {
		printf("Sommet %d\tPoids : %d\t", i, distances[i]);
		
		printf("Chemin : ");
		int p = prec[i];
		while(p != -1){
			printf("%d ", p);
			p = prec[p];
		}
		printf("\n");
	}

}



/***********************************************
 * 	         Programme Principal               *
 **********************************************/

int main (int nargs, char ** args) {
	int n;    /* nombre de sommets */
	int m;   /* nombre d''aretes */
	int max; /* poids max des aretes */
	liste *Voisins;

	Voisins = lire_graphe(&n, &m, &max);

	printf("\n========== INFOS ===========\n");
	printf("%d sommets, %d arrêtes et poid max à %d\nVoisins : ", n, m, max);
	int i; for(i = 0; i <= n; i++) {
		if(NULL != Voisins[i]) printf("%d(%d)  ", Voisins[i]->val, Voisins[i]->w);
	}
	printf("\n");

	printf("\n========= DIJKSTRA =========\n");
	Dijkstra(Voisins, n, max);

	printf("\n");
	return 0;
}
