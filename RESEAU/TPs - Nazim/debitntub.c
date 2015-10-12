#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

/* --------------- FONCTIONS DE DEBITUB.C ------------------ */
int sigaction(int sig, const struct sigaction *p_action, struct sigaction *p_action_ancien);
unsigned long n = 0;
int signalType(int sig, void (*h)(int)) {
	struct sigaction s;
	s.sa_handler = h;
	sigemptyset(&s.sa_mask);
	s.sa_flags = SA_RESTART;
	return sigaction(sig, &s, NULL);
}
void capte(int sig) {
	if(sig == SIGPIPE) { printf("Arrêt du tube\n"); exit(1); }
	else if(sig == SIGALRM) { printf("Lecture par le fils : %d ko\n", n/1024); alarm(1); }
	else { printf("Signal recu : %d\n", sig); }
}
/* --------------------------------------------------------- */

#define NB_TUBES 100

int main(int argc, char *argv[]) {
	char *s;
	int tubes[NB_TUBES][2];
	int bufsize, nbtubes, pid, cpt;

	if(argc != 3) { printf("Usage : ./debitntub <bufsize> <nbtubes>\n"); exit(0); }

	signalType(SIGPIPE, capte);
	bufsize = atoi(argv[1]);
	nbtubes = atoi(argv[2]);

	if(nbtubes > NB_TUBES) { printf("Le nombre de tubes soit être inférieur ou égal à %d !\n", NB_TUBES); exit(0); }

	s = malloc(bufsize * sizeof *s); // (Taille du tableau de char = taille du buffer)
	if(!s) { perror("MALLOC"); exit(1); }

	// Création de l'ensemble des pipes
	for(cpt = 0; cpt < nbtubes; cpt++) if(pipe(tubes[cpt]) < 0) { perror("PIPE"); exit(1); }
  
	pid = fork();
	if(pid < 0) { perror("FORK"); exit(1); }
	
	if(pid == 0) { /* FILS */
		fd_set set;
		int res, k;
		
		signalType(SIGALRM, capte);
		alarm(1);
		
		for(cpt = 0; cpt < nbtubes; cpt++) close(tubes[cpt][1]); // Femeture des sorties inutilisées
		
		while(1) {
			FD_ZERO(&set);
			for(cpt = 0; cpt<nbtubes; cpt++) FD_SET(tubes[cpt][0], &set); // Ajout de l'ensemble des descriptions
			
			res = select(tubes[nbtubes-1][0]+1, &set, NULL, NULL, NULL);
			if(res > 0) {
				for(cpt = 0; cpt < nbtubes; cpt++) {
					if(FD_ISSET(tubes[cpt][0], &set)) {
						k = read(tubes[cpt][0], s, bufsize);
						if(k < 0) { perror("READ"); exit(1); }
						n += k;
					}
				}
			
			} else { perror("SELECT"); }
		}
		
		for(cpt = 0; cpt < nbtubes; cpt++) close(tubes[cpt][0]); // Fermeture du reste des sorties
		
		exit(0); // Fin du fils
	}
	
	if(pid > 0) { /* PERE */
		fd_set set;
		int res;	
		printf("Nous sommes avec le père %d et le fils %d\n", getpid(), pid);

		for(cpt = 0; cpt < nbtubes; cpt++) close(tubes[cpt][0]); // Fermeture des sorties
	
		while(1) {
			FD_ZERO(&set);
			for(cpt = 0; cpt < nbtubes; cpt++)
			FD_SET(tubes[cpt][1], &set);
			
			res = select(tubes[nbtubes-1][1]+1, NULL, &set, NULL, NULL);
			if(res > 0) {
				for(cpt = 0; cpt < nbtubes; cpt++)
					if(FD_ISSET(tubes[cpt][1], &set))
						if(write(tubes[cpt][1],s,bufsize) < 0) { perror("WRITE"); exit(1); }
			
			} else { perror("SELECT"); }
		}
	
		for(cpt = 0; cpt < nbtubes; cpt++) close(tubes[cpt][1]); // Fermeture du reste des sorties
	}

	return 0;
}
