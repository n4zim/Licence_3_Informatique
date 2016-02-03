#!/bin/sh

# Teste si les fichiers générés sont justes
DIRECTORYSOURCE="./test/ressources/ProgrammeSource/*" #Dossier des fichier .l
DIRECTORYTRUE="./test/ressources/AnalyseLexicale/*" #Dossier dans lequel il y a les corrections
DIRECTORYTOTEST="./" #Dossier dans lequel il y a les fichiers générés

pwd

for f in $DIRECTORYSOURCE
do
  echo "Execution du fichier : $f"
  ./test_yylex $f
  echo
  echo
  if (( $? != 0 )); then return 1; fi

done



for f in $DIRECTORYTRUE
do
	FILENAME=`basename $f`
	FILENAME="$DIRECTORYTOTEST""$FILENAME"
	if [ -e "$FILENAME" ]; then

		RESULTDIFF=0
		diff -q -s $f $FILENAME #2>&RESULTDIFF

		if [[ $? -gt 0 ]]; then
			exit 1;
		fi
	fi 
done




