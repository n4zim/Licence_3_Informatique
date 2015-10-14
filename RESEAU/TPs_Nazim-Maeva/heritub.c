#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int p[2], n;
	unsigned i;
	char tmp[256], chars[10];
	if(pipe(p) < 0) { perror("PIPE"); exit(1); }
	
	scanf("%s", tmp);
	for(i = 0;  i < 10; i++) { chars[i] = tmp[i];  }
	printf("Les 3 premiers caractères : ");
	for(i = 0;  i < 3; i++) { printf("%c ", chars[i]); /*rand()%9*/ }
	printf("\n");
	
	n = fork();
	if(n < 0) { perror("FORK"); exit(1); }
	if(n == 0) { /* FILS */
		printf("Les 7 suivants : ");
		for(i = 3;  i < 10; i++) { printf("%c ", chars[i]); /*rand()%9*/ }
	}
	
	return 0;
}
