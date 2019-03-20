#include "../include/gestion_fichier_carte.h"

/*On utilise les constantes relatives aux cartes définies dans "commun.h" */

/*Fonctionne uniquement pour des cartes de version */
int lireCarte(FILE *carte, int *energie, char nomImage[],  unsigned char couleurClef[][NB_COULEURS] )
{
	int i;
	int valide = 0; /*booléen*/
	char codeCarte[TAILLE_CODE];
	int versionCarte;
	char motClef[MAX_TAILLE_MOTCLEF];
	/*vérif code et enregistrement*/
	fscanf(carte,"%s %d\n",codeCarte, &versionCarte);
	if( strcmp(CODE_PROJET, codeCarte) != 0) 
	{
		printf("Code carte incorrect : %s au lieu de %s.\n", codeCarte, CODE_PROJET);
		return EXIT_FAILURE;
		
	}
	else
	{
		printf("Version carte : %d.\n", versionCarte);
	}

	/*vérif version (égale à 1)*/
	if( versionCarte != 1)
	{
		printf("Version de la carte non supportée : %d.\n", versionCarte);
		return EXIT_FAILURE;
	}


	/***** Lecture des paramètres d'en-tête ******/
	for(i=0; i < NB_PARAM_PAR_VERSION[versionCarte -1]; i++) /*condition correspond aux nombre de paramètres attendus pour la version*/
	{
		if( fgetc(carte) == CODE_COM )
		{
			/*gestion des commetaire ?*/

			/*On ignore le commentaire, on passe à la ligne suivante*/
			while( fgetc(carte) != '\n')
				{
					/*rien à faire ici */
				}
			i--; /*on  décrémente le compteur pour qu'il revienne à sa valeur initiale au début de la prochaine itération*/

		}
		else 
		{	
			/*on revient sur nos pas*/
			fseek(carte,-sizeof(char),SEEK_CUR); 
			/*et on recommence*/
			fscanf(carte,"%s ",motClef);
			switch( correspondanceMotClef(motClef, versionCarte) )
			{
				case MC_carte :
					fscanf(carte, "%s", nomImage);
					printf("Image trouvée : %s.\n",nomImage);
					break;
				case MC_energie :
					/*fread(energie, sizeof(int), 1, carte); ne Fonctionne pas*/
					fscanf(carte, "%d",energie);
					printf("Énergie déterminée : %d.\n",*energie);
					break;
				case MC_chemin :
					fscanf(carte, "%hhu %hhu %hhu", &couleurClef[CC_chemin][rouge], &couleurClef[CC_chemin][vert], &couleurClef[CC_chemin][bleu]);
					printf("Couleur chemin déterminée : (%hhu, %hhu, %hhu).\n", couleurClef[CC_chemin][rouge], couleurClef[CC_chemin][vert], couleurClef[CC_chemin][bleu]);
					break;
				case MC_noeud :
					fscanf(carte, "%hhu %hhu %hhu", &couleurClef[CC_noeud][rouge], &couleurClef[CC_noeud][vert], &couleurClef[CC_noeud][bleu]);
					printf("Couleur noeud déterminée : (%hhu, %hhu, %hhu).\n", couleurClef[CC_noeud][rouge], couleurClef[CC_noeud][vert], couleurClef[CC_noeud][bleu]);
					break;
				case MC_construct :
					fscanf(carte, "%hhu %hhu %hhu", &couleurClef[CC_construct][rouge], &couleurClef[CC_construct][vert], &couleurClef[CC_construct][bleu]);
					printf("Couleur de construction déterminée : (%hhu, %hhu, %hhu).\n", couleurClef[CC_construct][rouge], couleurClef[CC_construct][vert], couleurClef[CC_construct][bleu]);
					break;
				case MC_in :
					fscanf(carte, "%hhu %hhu %hhu", &couleurClef[CC_in][rouge], &couleurClef[CC_in][vert], &couleurClef[CC_in][bleu]);
					printf("Couleur d'entrée déterminée : (%hhu, %hhu, %hhu).\n", couleurClef[CC_in][rouge], couleurClef[CC_in][vert], couleurClef[CC_in][bleu]);
					break;
				case MC_out :
					fscanf(carte, "%hhu %hhu %hhu", &couleurClef[CC_out][rouge], &couleurClef[CC_out][vert], &couleurClef[CC_out][bleu]);
					printf("Couleur de sortie déterminée : (%hhu, %hhu, %hhu).\n", couleurClef[CC_out][rouge], couleurClef[CC_out][vert], couleurClef[CC_out][bleu]);
					break;

				case -1 : /*Mot-clef non reconnu*/
					return EXIT_FAILURE;
				default :
					printf("Vous ne devriez pas être ici !\n");
					return EXIT_FAILURE;
			}
			fscanf(carte, "\n"); /*On passe à la ligne suivante*/
		}

	}

	/********** Bientôt dans vos foyer : lecture des noeuds et vérifs ***********/


	return EXIT_SUCCESS;
}

MotClef correspondanceMotClef(char motClef[], int version)
{
	int i;
	for(i=0; i<NB_PARAM_PAR_VERSION[version-1]; i++)
	{
		if( !strcmp(motClef, MOTCLEFS[i]) )
		{
			return (MotClef) i;
		}
	}
	printf("Mot clef non reconnu : %s (version %d).\n", motClef, version);
	return -1; /*valeur -1 taitée comme un échec*/
}