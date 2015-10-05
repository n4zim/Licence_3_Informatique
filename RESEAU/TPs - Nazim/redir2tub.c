#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int tube1[2], tube2[2], fils1, fils2;
	if(pipe(tube1) < 0) { perror("PIPE TUBE1"); exit(1); }
	if(pipe(tube2) < 0) { perror("PIPE TUBE2"); exit(1); }
	
	fils1 = fork();
	if(fils1 < 0) { perror("FORK FILS1"); exit(1); }
	if(fils1 == 0) { /* FILS1 */
		
		// Sortie st. -> Tube 1
		close(0);
		dup(tube1[0]);
		close(tube1[0]);
		
		close(tube1[1]);
		close(tube2[1]);
		close(tube2[0]);
	}
	
	fils2 = fork();
	if(fils2 < 0) { perror("FORK FILS2"); exit(1); }
	if(fils2 == 0) { /* FILS2 */
		
		// Sortie st. -> Tube 1
		close(0);
		dup(tube1[0]);
		close(tube1[0]);
		
		close(tube1[1]);
		close(tube2[1]);
		close(tube2[0]);
	}
	
	// Sortie st. -> Tube 2
	close(0);
	dup(tube2[0]);
	close(tube2[0]);
	/*close(p[0]);
	close(0);
	dup(p[0]);
	close(p[0]);
	execpl("wc", "wc", "-l", NULL);
	perror("execpl wc");*/
	exit(1);
}
