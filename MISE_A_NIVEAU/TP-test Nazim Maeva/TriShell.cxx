#include <iostream>
#include <vector>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;

vector<int> triSimple(vector<int> nombres, unsigned divisions, unsigned debut, unsigned tailleTab) {
	unsigned index;
	int courant;
	
	for(unsigned i = divisions+debut; i < tailleTab; i += divisions) {
		courant = nombres[i];
		
		for(index = i; index >= divisions && nombres[index - divisions] > courant; index -= divisions)
			nombres[index] = nombres[index - divisions];
		
		nombres[index] = courant;
	}
		
	return nombres;
}

vector<int> triShell(vector<int> nombres, unsigned divisions, unsigned tailleTab) {	
	for(unsigned i = divisions; i > 0; i--) {
		for(unsigned j = 0; j < divisions; j++) {
			nombres = triSimple(nombres, i, j, tailleTab);
			cout << "Tri simple de la " << i << "ème division (début à l'indice " << j << " : ";
			for(vector<int>::iterator it = nombres.begin(); it != nombres.end(); ++it) cout << *it << " ";
			cout << endl;
		}
	}
	
	return nombres;
}

int main(int argc, char **argv) {
	
	unsigned tailleTab;
	cout << "Taille du tableau : ";
	cin >> tailleTab;
	vector<int> nombres;
	
	int tmp;
	srand (time(NULL));
	cout << "\nTableau généré : ";
	for(unsigned i = 0; i < tailleTab; i++) {
		tmp = rand()%100;
		nombres.push_back(tmp);
		cout << tmp << " ";
	}
	
	unsigned divisions = 5;
	cout << "\n\nLancement du tri !\n";
	vector<int> nombresTries = triShell(nombres, divisions, tailleTab);
		
	cout << "\nTableau trié : ";
	for(vector<int>::iterator it = nombresTries.begin(); it != nombresTries.end(); ++it) cout << *it << " ";
	
	return 0;
}
