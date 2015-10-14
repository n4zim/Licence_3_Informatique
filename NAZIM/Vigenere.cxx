#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

class Vigenere {
	public:
		string key;
		vector<unsigned> spaces;
	 
		Vigenere(string key) {
			for(unsigned i = 0; i < key.size(); ++i) {
				if(key[i] >= 'A' && key[i] <= 'Z')
					this->key += key[i]; // Insere le caractere de la cle dejà en majuscules
				
				else if(key[i] >= 'a' && key[i] <= 'z')
					this->key += key[i]+'A'-'a'; // Insere le caractere de la cle en la passant en majuscules
			}
		}
	 
	 
		string encode(string input) {
			string tmp, output;
			
			//cout << "----- CODAGE -----" << endl;
			
			for(unsigned currentCntInput = 0, j = 0; currentCntInput < input.length(); currentCntInput++) {
				char currentChar = input[currentCntInput];
				
				if(currentChar == ' ') { spaces.push_back(currentCntInput); }
				
				if(currentChar >= 'a' && currentChar <= 'z') { currentChar += 'A'-'a';
								
					} else if(currentChar < 'A' || currentChar > 'Z') { continue; }

				tmp = ((currentChar - 'A') + (key[j] - 'A')) % 26 + 'A';
				//cout << (char)toupper(currentChar) << " + " << (char)toupper(key[j]) << " = " << tmp << endl;
				output += tmp;

				j = (j + 1) % key.length();
			}
				 
			return output;
		}
		
		double foundKeyLength(string codedMessage) {
			double indice = 0.0;
			int freq[26] = {0};

			for(int i = 0; codedMessage[i] != '\0'; ++i)
				if(isalpha(codedMessage[i]))
					++freq[tolower(codedMessage[i]) - 'a'];

			for(int i = 0; i < 26; ++i)
				indice += (double) (freq[i] * (freq[i] - 1)) / (codedMessage.size() * (codedMessage.size() - 1));

			return (codedMessage.size()*(0.074-0.038)) / (indice*(codedMessage.size()-1) - codedMessage.size()*0.038+0.078);
		}

		char lookAtTheLine(char ligne[]) {
			int freq[26] = {0};
			for(int i = 0; ligne[i] != '\0'; ++i) ++freq[tolower(ligne[i]) - 'a'];

			cout << "Fréquences : ";
			for(int i = 0; i < 26; ++i) cout << freq[i] << " ";
			cout << endl;
			
			int maxCount = 0, max = 0;
			for(int i = 0; i < 26; ++i) {
				if(freq[i] > max) {
					max = freq[i];
					maxCount = i;
				}
			}

			cout << "Distances : " << maxCount+'A' << " -> " << max << endl;

			char keyLetter = (maxCount + 'A') -  ('E' - 'A');
			while(keyLetter < 'A') keyLetter += 26; // Contrôle des majuscules
            cout << "Lettre trouvée : " << keyLetter << "\n\n";
			return keyLetter;
		}

		string freqAnalysis(string codedMessage) {
			int currentMessage;
			int keyLength = foundKeyLength(codedMessage); // Récupération de la longueur de la clé
			char key[keyLength]; // Clé de sortie
			
			cout << "Longueur de la clé : " << keyLength << "\n\n";
			
			for(int i = 0; i < keyLength; ++i) {
				char line[codedMessage.size()];
				
				int currentLine;
				for(currentMessage = i, currentLine = 0; currentMessage < (int)codedMessage.size(); ++currentLine) {
					line[currentLine] = codedMessage[currentMessage];
					currentMessage += keyLength;
				}

				line[currentLine] = '\0';

				cout << "Ligne : " << line << endl;
				key[i] = lookAtTheLine(line); // Recherche de la clé dans la ligne courante
			}

			key[keyLength] = '\0';
			return key;
		}
		
		string decode(string input) {
			string tmp, output;
			string keyFounded = freqAnalysis(input);
			
			//cout << "----- DECODAGE -----" << endl;
			for(unsigned currentCntInput = 0, j = 0; currentCntInput < input.length(); currentCntInput++) {
				char currentChar = input[currentCntInput];
		 
				if(currentChar >= 'a' && currentChar <= 'z')
					currentChar += 'A' - 'a';
				
				else if(currentChar < 'A' || currentChar > 'Z')
					continue;
		 
				tmp = (currentChar - keyFounded[j] + 26) % 26 + 'A';
				//cout << (char)toupper(currentChar) << " + " << (char)toupper(keyFounded[j]) << " = " << tmp << endl;
				output += tmp;
				j = (j + 1) % keyFounded.length();
			}
		 
			return output;
		}
		
		string insertSpaces(string decoded) {
			unsigned resizer = 1;
			for(unsigned i = 0; i <= ARRAY_SIZE(spaces); i++) {
				//cout << spaces[i]+resizer << "-";
				decoded.insert(spaces[i]+resizer-1, 1, ' ');
				resizer++;
			}
			
			return decoded;
		}
};

int main(int argc, char **argv) {
	string entree;
	string cle = "ATRP";
	//string message = "Crois-moi, ta vie sera très heureuse. Redescendez et attendez-moi, si vous voyez ce petit levier, là, je suppose. Réel signifie ou ce qui est purement conservatrice à la fonction pratique de la raison d'être. Sur des couches de composition minéralogique différente, nous trouvons la meilleure preuve de ce que des millions d'hommes ont autant écrit dans autant de genres. Prévenu de cette bonne aubaine, parce que sinon, à quoi bon en effet, ils jugèrent convenable de faciliter la nature. Majesté, se joignit aux applaudissements qui lui avaient interdit de s'occuper de lui demain matin ! Lis charmant, votre calice ne s'ouvrira pas, dit un autre. Oppressé par le sentiment des proportions, justement parce qu'il le voyait, et imagina tout ce qu'aurait fait n'importe quoi. Aie, quelque chose clochait toujours quelque part. Méfiant, inquiet, soupçonneux, le cardinal et lui parla. Abandonnant l'étude de cette branche de littérature, me mêlant quelquefois d'en trouver le terme, l'élan d'une humanité aux prises avec... Quand tant d'événements ; elle fait naître. Force gens croient de bonne foi, à la droite sait-il ce qui se cache dans un château voisin où il devait passer trois ou quatre pieds, ce qui consommait beaucoup de carburant, d'oxygène et de la cannelle et du safran. Un professeur de russe de l'université, envers son pays et celui-ci. Épaules rentrées, nuque baissée, avec la conviction d'être utile aux modernes, son âme s'est-elle formée à votre image et à celle des moulins. Chauffer nos pieds en même temps vous tenez à le savoir. Énormément d'objets de luxe et de prodigalités. Chancelant, il s'écria, en retenant sa robe d'intérieur faisait parfaitement l'affaire. Apparemment cela a déjà été inventé, ni une tête d'homme ou de femme. Vois-tu, ma petite dame, aux yeux d'angoisse ; tandis que la maîtresse de mon ami. Ravi de pouvoir faire des enfants, qui couraient devant lui. Accoutumée, comme toutes les autres : le temps ruine tout, et nombre de gens qui connaissent à fond que je ne nomme pas et que vous arrivez seulement ? Débat national, mon petit, lui dit qu'un être. Funeste à la mienne ; car, enfin, nous tâcherons de vous servir même de termes très peu convenables à votre dignité qu'à votre charité évangélique. Après une vingtaine de personnes, avançait en riant aux éclats ; il n'accusa personne ; il lui fit violence... Dépêchez-vous, il est dans le souci des autorités agricoles de réserver à travers le monde. Collés contre un mur, haut de trente coudées, en pierres, s'enjolivaient de galeries extérieures qui donnaient un râle à ses joies. Examinons l'armée ennemie, il est devenu l'écuyer de la duchesse et la princesse y avait donné, dans une occasion si funeste ; et l'intérêt que m'inspirait le mépris et l'esclave, les reliques outragées des deux frères fut réglée. Relevez-vous, dit le barbier. Aidez votre mère à me servir de pupitre à monsieur, répondit l'enfant en sanglotant. Voies stratégiques, bastilles, cités ouvrières dans la prévision des morts n'ont jamais reçu par les quatre fenêtres du cabinet de lecture, je perçus bientôt un bruit venu d'un pays plus riche pour cela. Marchant à grands pas le paysage. Allongeant alors le cou comme quelqu'un qui ait vu jusqu'ici de nombreux jours. Parler de problèmes à résoudre, soit. Depuis il s'en revenait de chez sa famille. Comptant les séduire, il suffit d'ajouter des narcisses et des primevères des champs dans le lointain. Armé, de cette haine, il n'abattait, et en fit pour toujours un affranchi dans la vallée lui causèrent de véritables jouissances. Dont la crise définitive, crise innommable ! Premier ou dernier, c'est terminé. Arrivée à mi-chemin de l'autre dans le chemin, j'entrerai... Bloquée par les gardes du domicile conjugal. Fut-ce l'impossibilité de lui proposer de coucher avec un autre ; il suffit encore une fois tous les huit jours, se le reprocha. Coupe-lui la moustache et aspire avec force le sang qui est versé au milieu de ces planches mal jointes de la porte ouest me regardèrent détaler, puis je les fis pleurer tous trois. Sortez par cette porte un trou à la cire glissante du parquet. Idéal, loin de mériter ses bontés, on entendit battre le balancier de l'horloge, accrochée au chevet du lit ? Profitons de ces minutes graves où le soleil atteindrait son méridien. Aussi bas que mes semblables, ne paraissaient pas s'en souvenir. Barré pour trois mois de répit. Décomposons le plan en relief des inscriptions de plusieurs lignes destinées à marquer les étapes de ma découverte à mon père qui était alors retiré dans son égoïsme, enfin à ce patronage qu'elles exerçaient dans leurs terres, leurs villes et dénature leur langue. Ignoriez-vous que je suis ici pour me faire face";
	string message = "Loin tres loin au delà des monts Mots à mille lieues des pays Voyellie et Consonnia demeurent les Bolos Bolos Ils vivent en retrait a Bourg en Lettres sur les côtes de la Semantique un vaste ocean de langues Un petit ruisseau du nom de Larousse, coule en leur lieu et les approvisionne en reglalades necessaires en tout genre un pays paradisiagmatique dans lequel des pans entiers de phrases premâchees vous volent literalement tout cuit dans la bouche Pas même la toute puissante Ponctuation ne regit les Bolos Bolos une vie on ne peut moins orthodoxographique Un jour pourtant une petite ligne de Bolo Bolo du nom de Lorem Ipsum decida de saventurer dans la vaste Grammaire Le grand Oxymore voulut len dissuader le prevenant que là-bas cela fourmillait de vils Virgulos de sauvages Pointdexclamators et de sournois Semicolons qui lattendraient pour sûr au prochain paragraphe, mais ces mots ne firent echo dans l'oreille du petit Bolo qui ne se laissa point deconcerter Il pacqua ses 12 lettrines glissa son initiale dans sa panse et se mit en route. Alors qu'il avait gravi les premiers flancs de la chaîne des monts Italiques il jeta un dernier regard sur la skyline";

	cout << "Entrez la clé de cryptage (par défaut " << cle << ") : ";
	getline(cin, entree);
	if(!entree.empty()) cle = entree;
	cout << "La clé sera : " << cle << endl;
	
	cout << "\nEntrez le texte (par défaut un long texte aléatoire) : ";
	getline(cin, entree);
	if(!entree.empty()) message = entree;
	cout << "Le message sera : " << message << endl;
	
	Vigenere Crepes = Vigenere(cle);
	
	string encode = Crepes.encode(message);
	cout << "\n--------------> Crypté : " << encode << "\n\n";
	
	string decode = Crepes.decode(encode);
	//cout << "\n--------------> Décrypté (clé connue) : " << decode << endl;
	
	cout << "\n--------------> Décrypté (avec espaces) : " << Crepes.insertSpaces(decode) << endl;

	cout << "\n--------------> Décrypté (clé non connue) : " << decode << endl;
}
