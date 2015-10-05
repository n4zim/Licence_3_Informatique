#include <stdio.h>

#define NEW_TACHE (T_Tache) malloc(sizeof(struct tache))
#define NEW_COUPLE (T_Couple) malloc(sizeof(struct couple))

struct tache {
	int numero;
	int explore;
	T_LISTE precedents;
};

struct couple {
	int numero;
	T_LISTE graphe;
};


int main(int argc, char **argv) {
	
	
	
	return 0;
}

