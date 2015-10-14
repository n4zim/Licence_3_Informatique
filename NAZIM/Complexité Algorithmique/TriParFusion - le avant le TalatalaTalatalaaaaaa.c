#include <stdio.h>

void ranger(int numbers[]) {
	printf("BLABLABLA ");
	printf("%d ", numbers[4]);
}

int main(int argc, char **argv) {
	int numbers[10000];	// On va faire simple :)
	int rang = 0, tmp, i;
	
	printf("Tapez des nombres, le tri est automatique, terminez par q\n\n");
	while (scanf("%d", &tmp) == 1) {
		if(tmp == 'q') return 0;
		numbers[rang] = tmp;
		
		printf("Pas rangé : ");
		for(i = 0; i <= rang; i++) {
			printf("%d ", numbers[i]);
		}
		printf("\nRangé : ");
		ranger(numbers);
		
		printf("\n\n");
		rang++;
	}
	
	return 0;
}
