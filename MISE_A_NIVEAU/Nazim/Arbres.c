/*
 * =====================================================================================
 *
 *       Filename:  aho.c
 *
 *    Description:  Aho&Corsick pattern search algortihm
 *
 *        Version:  1.0
 *        Created:  09/24/2014 03:20:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:  aucune
 *
 * =====================================================================================
 */

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#define NUM_MOTIFS 6


char *motifs[NUM_MOTIFS] = { "CENTRAL"
                           , "CERISE"
                           , "DIZAINE"
                           , "FRAISE"
                           , "PENTAGONALE"
                           , "ROUGES"
};

char string[] = "LETOILEDEMERAMAILLESROUGESESTUNEASTERIDEACINQBRASTRIANGULAIRESRAYONNANTAUTOURDUNDISQUECENTRALAPLATIELLEESTDETEINTEROUGEPARFOISORANGEEVOIREVIOLETTEOUBLEUEPLUSOUMOINSDENSEMENTCOUVERTEDEGROSSESPLAQUESBLANCHESOUBEIGESARRONDIESETLISSESGENERALEMENTPLUSPETITESETMOINSDENSESAPARTIRDELAMOITIELAPLUSDISTALEDESBRASDONTLESPOINTESSONTDONCROUGESOUDUMOINSDELACOULEURDEFONDCESPLAQUESLAISSENTNORMALEMENTUNEZONENUEPLUSOUMOINSPENTAGONALESURLEDISQUECENTRALMAISPEUVENTAUSSIPARFOISLECOUVRIRENTIEREMENTLAPLAQUEMADREPORITIQUEESTORANGECOMMELANUSCENTRALELLEMESUREUNEDIZAINEDECENTIMETRESDEDIAMETREMAXIMUMALAGEADULTE";
//Texte dans lequel faire la recherche, mais sans les espaces ni la ponctuation.


int motifs_length[NUM_MOTIFS];

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  definir_longueur_tableau_etats
 *  Description:  Obtenir le nombre d'états maximal.
 * =====================================================================================
 */
	int
definir_longueur_tableau_etats ( char **motifs )
{
	int max_length = 0;

    for ( int i = 0; i < NUM_MOTIFS; i++ )
        max_length += strlen ( motifs[i] );

	return max_length;
}		/* -----  end of function define_pattern_number  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  motifs_longueur_max
 *  Description:  Calcul de la longeur du plus grand motif (ie du plus grand mot chercher).
 * =====================================================================================
 */
	int
motifs_longueur_max ( int* motifs_longueur )
{
	int max_length = 0;

    for ( int i = 0; i < NUM_MOTIFS; i++ )
    {
        if ( motifs_longueur[i] > max_length )
			max_length = motifs_longueur[i];
	}

	return max_length;
}		/* -----  end of function motifs_longueur_max  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  prepare_tableau_longueur_motifs
 *  Description:  Précalcule la longueur de chaque motif dans un tableau.
 * =====================================================================================
 */
	void
prepare_tableau_longueur_motifs ( char **motifs
                                , int* motifs_longueur )
{
    for ( int i = 0; i < NUM_MOTIFS; i++ )
        motifs_longueur[i] = strlen(motifs[i]);

	return ;
}		/* -----  end of function prepare_tableau_longueur_motifs  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  allocate_tableau_backtrack
 *  Description:  Alloue la mémoire requise pour le tableau des erreurs.
 * =====================================================================================
 */
	char*
allocate_tableau_backtrack ( int longueur_tableau_backtrack )
{
	size_t longueur_tableau = longueur_tableau_backtrack * sizeof(char);
	char* tableau_backtrack = (char* )malloc(longueur_tableau);

    if ( tableau_backtrack == NULL)
    {
        fprintf ( stderr, "NPE allocation backtrack : %s",__func__ );
		exit(1);
	}

    memset ( tableau_backtrack, 0, longueur_tableau );

	return tableau_backtrack;
}		/* -----  end of function allocate_tableau_backtrack  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  allocate_tableau_etats
 *  Description:  Alloue la mémoire requise pour le tableau d'états.
 * =====================================================================================
 */
	int**
allocate_tableau_etats (int longueur_tableau_etats )
{
    int** state_tableau_etats = (int**)malloc(sizeof(int*) * 26);

    if ( state_tableau_etats == NULL )
    {
        fprintf ( stderr, "NPE alocation : %s", __func__ );
		exit(1);
	}

	size_t longueur_strip_tableau = longueur_tableau_etats * sizeof(int*);
    size_t longueur_strip_tableau_elemnets = longueur_tableau_etats * sizeof(int);

    for ( int i = 0; i < 26; i++ )
    {
		state_tableau_etats[i] = (int*)malloc(longueur_strip_tableau);
        if ( state_tableau_etats[i] == NULL )
        {
            fprintf ( stderr, "NPE alocation : %s", __func__ );
			exit(1);
		}

        memset ( state_tableau_etats[i], 0, longueur_strip_tableau_elemnets );
	}

	return state_tableau_etats;
}		/* -----  end of function allocate_tableau_etats  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  free_tableau_etats
 *  Description:  Libère la mémoire allouée du tableau d'états.
 * =====================================================================================
 */
	void
free_tableau_etats ( int **tableau_etats )
{
    for ( int i = 0; i < 26; i++ )
        free ( tableau_etats[i] );

    free ( tableau_etats );

    return ;
}		/* -----  end of function free_tableau_etats  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  remplir_tableau_etat
 *  Description:  Remplit le tableau des états des transitions issues des motifs.
 * =====================================================================================
 */
	void
remplir_tableau_etat ( int** tableau_etats
                     , int* longueur_strings)
{
    int longueur_max_motifs = motifs_longueur_max ( motifs_length ), numero_etat = 1;

    for ( int position_longueur = 0; position_longueur < longueur_max_motifs; position_longueur++ )
    {
        for ( int index_string_courant = 0; index_string_courant < NUM_MOTIFS; index_string_courant++ )
        {
            if ( position_longueur >= longueur_strings[index_string_courant] )
				continue;

            int lettre_courante = motifs[index_string_courant][position_longueur] - 'A', etat_ou_placer_la_transition = 0;

            if(position_longueur > 0)
            {
				int etat_temporaire = 0;
                for ( int k = 0; k < position_longueur; k++ )
					etat_temporaire = tableau_etats[motifs[index_string_courant][k] -'A'][etat_temporaire];

				etat_ou_placer_la_transition = etat_temporaire;

            }

            if ( tableau_etats[lettre_courante][etat_ou_placer_la_transition] == 0 )
            {
				tableau_etats[lettre_courante][etat_ou_placer_la_transition] = numero_etat;

                if ( position_longueur == longueur_strings[index_string_courant] - 1 )
                {
                    for ( int f = 0; f < 26; f++)
                        tableau_etats[f][numero_etat] = -1;
				}
				numero_etat++;
			}
		}
	}
	return ;
}		/* -----  end of function remplir_tableau_etat  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  transitions_echec
 *  Description:  Remplit le tableau des échec.
 * =====================================================================================
 */
	void
transitions_echec ( int** tableau
                  , int longueur_tableau_etats
                  , char* tableau_backtrack
                  , int taille_tableau_backtrack
                  , int index_tableau_backtrack
                  , int current_state
                  , int current_letter )
{
    for ( int i = 0; i < 26; i++ )
    {
        tableau_backtrack[index_tableau_backtrack] = i;

        if(current_state < index_tableau_backtrack && tableau[i][current_state] != 0)
        {
            printf("current_state = %d\n", current_state);
            transitions_echec(tableau, longueur_tableau_etats
                            , tableau_backtrack
                            , taille_tableau_backtrack
                            , index_tableau_backtrack + 1
                            , tableau[i][current_state]
                            , i);
        }

        else
        {
            for ( int j = 1; j < index_tableau_backtrack; j++ )
            {
                //TODO quand le tableau des échec sera fait.
			}
		}
	}

	return ;
}		/* -----  end of function transitions_echec  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  rajouter_transitions_echec_tableau_etat
 *  Description:  Modifie le tableau des états en fonction du tableau des erreurs.
 * =====================================================================================
 */
	void
rajouter_transitions_echec_tableau_etat ( int** tableau
                                        , int longueur_tableau_etats
                                        , char* tableau_backtrack
                                        , int taille_tableau_backtrack)
{
    for ( int i = 0; i < 26; i++ )
    {
        if ( tableau[i][0] != 0 )
            transitions_echec(tableau
                            , longueur_tableau_etats
                            , tableau_backtrack
                            , taille_tableau_backtrack
                            , 0
                            , tableau[i][0]
                            , i);
    }

	return ;
}		/* -----  end of function rajouter_transitions_echec_tableau_etat  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  afficher_tableau
 *  Description:  Affiche le tableau des états.
 * =====================================================================================
 */
	void
afficher_tableau ( int** tableau
                 , int hauteur
                 , int longueur )
{
	printf("  ");
    for ( int i = 0; i < longueur; i++ )
		printf("%4i", i);

	puts("");

    for (int i = 0; i < longueur; i++ )
		printf("----");

    puts("");

    for ( int i = 0; i < hauteur; i++ )
    {
        printf ( "%c|", 'A'+i );

        for ( int j = 0; j < longueur; j++ )
            printf ( "%4i", tableau[i][j] );

		puts("");
	}

	return ;
}		/* -----  end of function afficher_tableau  ----- */

    /*
     * ===  FUNCTION  ======================================================================
     *         Name:  remplir_tableau_backtrack
     *  Description:  Remplit le tableau de erreurs.
     * =====================================================================================
     */

void remplir_tableau_backtrack ( char* tableau_backtrack
                               , int longueur_max
                               , int** tableau_etats
                               , int longueur_tableau_etats )
{
    for ( int j = 0;  j < 26; j++ )
    {
        if ( tableau_etats[j][1] == 0 )
        {
            for ( int i = 0; i < longueur_max; i++ )
                if ( j+'a' == motifs[i][0] )
            {
                //Remplir le tableau des errerus avec ce qu'il faut.
            }
        }
    }

    return ;
}            /* -----  end of function afficher_tableau  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Entry point
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
    puts ( "" );
	prepare_tableau_longueur_motifs(motifs, motifs_length);

    int longueur_tableau_etats = definir_longueur_tableau_etats ( motifs ), longueur_max = motifs_longueur_max ( motifs_length ), **tableau_etats  = allocate_tableau_etats ( longueur_tableau_etats );
    char* tableau_backtrack = allocate_tableau_backtrack ( longueur_max );

    remplir_tableau_etat ( tableau_etats
                         , motifs_length );

    puts ( "== TABLEAU ETATS ==" );
    afficher_tableau ( tableau_etats
                     , 26
                     , longueur_tableau_etats );
    remplir_tableau_backtrack ( tableau_backtrack
                              , longueur_max
                              , tableau_etats
                              , longueur_tableau_etats );

    puts ( "" );
    puts ( "" );
    printf ( "  " );

    for ( int i = 0; i < 6; i++ )
        printf("%4i", i);

    puts("");
    printf("#|");

    for ( int i = 0; i < 6; i++ ) {
        printf ( "%4i", tableau_backtrack[i] );
    }

    puts("");

    int str = strlen(string), etat_courant = 0, counter = 0;
    char mot[longueur_max];

    printf("\n\n");

    for (int k=0; k<str; k++)
        printf("%c", string[k]);
    printf("\n\n\n");

    for (int i = 0; i < str; i++)
    {
        int nouvel_etat = tableau_etats[string[i]-'A'][etat_courant];
        if(nouvel_etat > 0)
        {
            mot[counter] = string[i];
            counter++;
        }

        else if(etat_courant != 0 && nouvel_etat != -1)
            counter = 0;

        if(nouvel_etat == -1)
        {
            etat_courant = tableau_etats[string[i]-'A'][0];

            for (int k=0; k<counter; k++)
                printf("%c", mot[k]);
            printf(" : trouve au caractere numero %d!!\n", i);

        }
        else
            etat_courant = nouvel_etat;
    }
    printf("\n\n");

	free_tableau_etats(tableau_etats);
	return EXIT_SUCCESS;
}
