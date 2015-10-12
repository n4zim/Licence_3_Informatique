#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

/* --------------- FONCTIONS DE DEBITUB.C ------------------ */
int sigaction(int sig, const struct sigaction *p_action, struct sigaction *p_action_ancien);
unsigned long n = 0; // Taille
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

int main(int argc, char *argv[]) {
	if(argc != 2) { printf("Usage : ./tradu2tub <bufsize>\n"); exit(0); }
	
	char *s;
	int tube1[2], tube2[2], pid, bufsize;
	
	bufsize = atoi(argv[1]); // Définition de la taille du buffer
	
	s = malloc(bufsize * sizeof *s); // Taille du tableau de char
	if(!s){ perror("MALLOC"); exit(1); }

	// Création des tubes
	pipe(tube1); pipe(tube2);
	if(tube1 < 0 || tube2 < 0){ perror("PIPE"); exit(1); }

	// Création du fils
	pid = fork();
	if(pid < 0) { perror("fork"); exit(1); }
	
	if(pid == 0) { /* FILS */
		// Fermeture des sorties inutiles
		close(tube1[1]);
		close(tube2[0]);
		close(0);
		
		if(dup(tube1[0]) < 0){ perror("DUP TUBE1"); exit(1); }
		close(1);
		if(dup(tube2[1]) < 0){ perror("DUP TUBE2"); exit(1); }
	
		execlp("tr", "tr", "a-z", "A-Z", NULL);
		//execlp("cat","cat",NULL);
		
		perror("EXECLP");
		exit(1);
	}

	if(pid > 0) { /* PERE */
		fd_set set;
		int res, k, in=1;
		printf("Nous sommes avec le père %d et le fils %d\n", getpid(), pid);
		
		signalType(17, capte);
		signalType(18, capte);
		signalType(20, capte);
		
		close(tube1[0]);
		close(tube2[1]); 
		
		while(1) {
			FD_ZERO(&set);
			if(in) FD_SET(0, &set);
			FD_SET(tube2[0], &set);
			
			res = select(tube2[0]+1, &set, NULL, NULL, NULL);
			if(res > 0) {
				if(FD_ISSET(0, &set)) { /* TUBE 1 */
					k = read( 0, s, bufsize);
					if(k < 0) { perror("READ"); exit(1); }
					if(k == 0) { in = 0; close(tube1[1]); continue; }
					
					if(write(tube1[1], s, k) < 0) { perror("WRITE"); exit(1); }
				}
				
				if(FD_ISSET(tube2[0], &set)){ /* TUBE 2 */
					k = read(tube2[0], s, bufsize);
					if(k < 0) { perror("READ"); exit(1); }
					
					if(write(1, s, k) < 0) { perror("write()"); exit(1); }
				}
			
			} else { perror("SELECT"); }
		}
		
		close(tube2[0]);
		close(tube1[1]);
	}
	
	return 0;
}
