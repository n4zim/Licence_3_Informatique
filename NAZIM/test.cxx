#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

int diviseurs[300];
vector <string> seq;

string coder(string texte, string cle)
{
	cout << endl << "-------------CODAGE-------------" << endl;
	string code = "";
	int tailleTexte = texte.length();
	int tailleCle = cle.length();
	
	for(int i=0; i<tailleTexte ; i++)
		code += (texte[i] + cle[i%tailleCle]-194)%26 + 97;

	return code;
}

int kasiski(string texte)
{
	cout << endl << "------------KASISKI-------------" << endl << endl;
	int tailleTexte = texte.length();
	
	int start = 0;
	
	for(int i=start; i < tailleTexte; i++)
	{
		for(int j=i+1; j < tailleTexte; j++)
		{
			if(texte[i] == texte[j])
			{
				string sequence = "";
				sequence += texte[i];
				for(int k=1; k < tailleTexte; k++)
				{
					if(texte[i+k] == texte[j+k])
					{
						sequence += texte[i+k];
					}
					else
					{
						if(sequence.length() > 1)
						{
							int rep = j-i;
							i = i+k;

							seq.push_back(sequence);

							cout << "Sequence : \"" << sequence << "\" | interval de repetition : " << rep << " | PGCD : ";

							/* Recherche des diviseurs commun */
							for (int f=2;f<rep;f++)
							{
								if ((rep % f == 0))
								{  
									cout << f << " ";
									diviseurs[f]++;
								}
							}
							cout << endl;
						}
						break;
					}
				}
				//break;
			}
		}
	}

	/* Recherche du PGCD le plus commun */
	int longueurCle = 0;
	for (int i = 0;i < 300;i++)
	{
		if (diviseurs[i] >= diviseurs[longueurCle])
		{                        
			longueurCle = i;
		}
	}
	cout << "Longueur de la cle trouve : " << longueurCle << endl << endl;

	return longueurCle;
}

void decoder(string code, int tailleCle)
{
	cout << "------------DECODAGE------------" << endl << endl;
	int tailleCode = code.length();

	string texte = "";

	/*
	for(unsigned int i=0; i<seq.size(); i++)
	{
		string cle = seq.at(i);

		if((int) cle.length() == tailleCle)
		{
			texte = "";
			cout << "Test avec la clef : " << cle << endl;
			for(int j=0; j<tailleCode; j++)
			{
				texte += (code[j] - cle[j%tailleCle]-194)%26 + 97;
			}
			cout << endl << endl << endl << texte << endl << endl << endl;
		}
		else
		{
			cout << "Mauvaise longueur (" << cle.length() << ")" << endl;
		}
	}
	*/
	string cle = "kilo";

	for(int i = 0; i < tailleCode; i++)
		texte += (code[i] - cle[i%tailleCle] + 26) % 26 + 97;

	cout << "Message decode : " << texte << endl;

	cout << endl << "--------------------------------" << endl;
}

int main()
{
	string cle, texte = "Loin tres loin au delà des monts Mots à mille lieues des pays Voyellie et Consonnia demeurent les Bolos Bolos Ils vivent en retrait a Bourg en Lettres sur les côtes de la Semantique un vaste ocean de langues Un petit ruisseau du nom de Larousse, coule en leur lieu et les approvisionne en reglalades necessaires en tout genre un pays paradisiagmatique dans lequel des pans entiers de phrases premâchees vous volent literalement tout cuit dans la bouche Pas même la toute puissante Ponctuation ne regit les Bolos Bolos une vie on ne peut moins orthodoxographique Un jour pourtant une petite ligne de Bolo Bolo du nom de Lorem Ipsum decida de saventurer dans la vaste Grammaire Le grand Oxymore voulut len dissuader le prevenant que là-bas cela fourmillait de vils Virgulos de sauvages Pointdexclamators et de sournois Semicolons qui lattendraient pour sûr au prochain paragraphe, mais ces mots ne firent echo dans l'oreille du petit Bolo qui ne se laissa point deconcerter Il pacqua ses 12 lettrines glissa son initiale dans sa panse et se mit en route. Alors qu'il avait gravi les premiers flancs de la chaîne des monts Italiques il jeta un dernier regard sur la skyline";
	
	//cout << endl << "Rentrez le texte > ";
	//cin >> texte;
	
	cout << "Rentrez la cle > ";
	cin >> cle;	
	
	string code = coder(texte, cle);
	
	cout << endl << "Message code: " << code << endl;
	
	int tailleCle = kasiski(code);

	decoder(code, tailleCle);
	
	return 0;
}
