#ifndef _H_graphes_lecture
#define _H_graphes_lecture

typedef struct maillon * liste;

struct maillon {
      int val;
      int w;
      liste suiv;
};

#endif
