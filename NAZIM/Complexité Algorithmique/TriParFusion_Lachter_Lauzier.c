#include <stdio.h>

void fusion(int* nombresIn, int g, int m, int d, int* nombresOut) {
    int i, j, k;
    i = k = g;
    j = m + 1;

	while(i <= m && j <= d)
		if(nombresIn[i] < nombresIn[j]) nombresOut[k++] = nombresIn[i++];
			else nombresOut[k++] = nombresIn[j++];
	
	while(i <= m) nombresOut[k++] = nombresIn[i++];
	while(j <= d) nombresOut[k++] = nombresIn[j++];
	
	for(i = g; i <= d; i++) nombresIn[i] = nombresOut[i];
}


void ranger(int nombresIn[], int g, int d, int nombresOut[]) {
    if(g < d) {
        int m = ((g + d) / 2);
        ranger(nombresIn, g, m, nombresOut);
        ranger(nombresIn, (m + 1), d, nombresOut);        
        fusion(nombresIn, g, m, d, nombresOut);
    }
}


int main(int argc, char **argv) {
    int tailleTab = 1000000;
    int nombresIn[tailleTab];
    int nombresOut[tailleTab];
    int nbEntrees = 0, i;
    
    while(scanf("%d", &nombresIn[nbEntrees]) == 1) nbEntrees++;
    
    printf("\nPas rangé : ");
    for(i = 0; i < nbEntrees; i++) printf("%d ", nombresIn[i]);
    
    ranger(nombresIn, 0, (nbEntrees-1), nombresOut);
    
    printf("\n\nRangé : ");
    for(i = 0; i < nbEntrees; i++) printf("%d ", nombresIn[i]);
    
    printf("\n");
    return 0;
}
