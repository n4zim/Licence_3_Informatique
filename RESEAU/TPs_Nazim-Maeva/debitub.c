#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int sigaction(int sig,const struct sigaction *p_action,struct sigaction *p_action_ancien);
unsigned long n = 0;

int signalType(int sig, void (*h)(int)) {
	struct sigaction s;
	s.sa_handler = h;
	sigemptyset(&s.sa_mask);
	s.sa_flags = SA_RESTART;
	return sigaction(sig, &s, NULL);
}

void capte(int sig) {
	if(sig = SIGPIPE) { printf("Arrêt du tube\n"); exit(1); }
	else if(sig = SIGALRM) { printf("fils a lu : %d ko\n", n/1024); alarm(1); }
	else { printf("Signal recu : %d\n", sig); }
}

int main(int argc, char *argv[]) {
	char *s;
	int p[2], pid, k;
	
	if(argc!=2){
		printf("Il manque des arguments !");
		exit(0);
	}
	
	signalType(SIGPIPE, capte);
	k = atoi(argv[1]);
	s = malloc(k * sizeof *s);
	
	if(!s) { perror("malloc"); exit(1); }
	if(pipe(p) < 0){ perror("pipe"); exit(1); }
	
	pid = fork();
	if(pid < 0) { perror("FORK"); exit(1); }
	
	if(pid == 0) { /* FILS */
		int i;
		signalType( SIGALRM, capte);
		close(p[1]);
		alarm(1);
		while(1) {
			i = read(p[0], s, k);
			if(i == 0) kill(pid, SIGPIPE);
			n += i;
		}
		exit(0);
	}
	if(pid > 0) { /* PERE */
		printf("Père : %d\nFils : %d\n", getpid(), pid);
		close(p[0]);
		
		while(1) { if(write(p[1], s, k) < 0) { perror("write");exit(1); } }
		free(s);
	}
}
