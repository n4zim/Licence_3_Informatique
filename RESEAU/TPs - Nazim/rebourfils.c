#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void lancerRebours() {
	int curr; for(curr = 3; curr >= 0; curr--) {
		printf("----[%d]----> %d\n", (int)getpid(), curr);
		sleep(1);
	}
	printf("Fin de %d\n", (int)getpid());
	exit(0);
}

void creerFils(int nb) {
	int i;
	pid_t p, old = getpid();

	for(i = 0; i < nb; i++) {
		p = fork();
		if(p < 0) { perror("fork"); exit(1); }
		if(p == 0) {
			if(p == old) printf("Création de %d fils dans %d", nb, getpid());
			printf("Nouveau fils dans %d : %d\n", (int)getppid(), (int)getpid());
			lancerRebours();
		}
	}
	printf("\n");
}

int main(int argc, char **argv) {
	int tmp = 0;
	while(scanf("%d", &tmp) == 1)
		if(tmp > 0) creerFils(tmp);
			else return;
	return 0;
}
