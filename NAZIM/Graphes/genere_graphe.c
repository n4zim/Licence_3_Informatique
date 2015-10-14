
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>



int ** mat;


int main (int nargs, char ** args)
{
      int nb_sommets, min_poids, max_poids, nb_aretes, i, j, x, y;
      float prob_aretes;

      if (5 != nargs) 
      {
            printf("Nombre d'arguments incorrect\n");
            printf("Forme : ./genere_graphe  nombre_sommets  probabilite  poids_min  poids_max\n");
            return 1;
      }

      srand(time(NULL));
      
      nb_sommets = atoi(args[1]);
      prob_aretes = 10*atoi(args[2]);
      min_poids = atoi(args[3]);
      max_poids = atoi(args[4]);
      
      /* calcul du nombre d'aretes en fonction du nombre de somments et de la probabilite */
      nb_aretes = prob_aretes*nb_sommets*(nb_sommets-1)/2000;
      
      printf("%d \n",nb_sommets);
      if (0) printf("%d\n",nb_aretes);
      
      /* allocation de la matrice */
      mat = (int**) malloc (nb_sommets*sizeof(int*));
      assert(mat != NULL);
      for(i=0; i<nb_sommets; i++)
      {
            mat[i] = (int*) malloc(nb_sommets*sizeof(int));
            assert(mat[i] != NULL);
      }
      
      /* initialisation de la matrice */
      for(i=0; i<nb_sommets; i++)
            for(j=0; j<nb_sommets; j++)
                  mat[i][j] =0;
      
      for(i=0; i<nb_aretes; )
      {
            /* calcul aleatoire des sommets */
            x = rand()%(nb_sommets);
            y = rand()%(nb_sommets);
            if (x == y) 
                  continue;
            if (x > y) 
            { int z = x; x = y; y = z; }
            
            if (mat [x] [y])
                  continue;
            mat [x][y] = mat [y][x] = 1;
            i++;
            printf("%d %d %d\n", x, y, rand()%(max_poids - min_poids)+1 );
      }
      
      return 0;
}
