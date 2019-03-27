#include "../include/carte.h"


Carte* allouerCarte()
{
	Carte *carte = malloc(sizeof(Carte));
	if( !carte )
	{
		printf("Échec d'allocation de la carte.\n");
		exit(EXIT_FAILURE);
	}
	return carte;
}

void libererCarte(Carte *carte)
{
	libererGraphe(carte->nombreNoeuds, carte->chemins);
	free(carte);
}

void afficherCarte(Carte *carte)
{
	int couleur, indice, j;
	MotClef MC_lu;
	/*** Version ***/
	printf("Version carte : %d\n", carte->version);
	/*** Image ***/
	printf("Image associée : %s\n", carte->nomImage);
	/*** Paramètres ***/
	printf("Énergie : %u\n", carte->energie);
	
	for( MC_lu=0; MC_lu < NB_PARAM_PAR_VERSION[carte->version-1]; MC_lu++) /*NB_PARAM_PAR_VERSION[carte->version-1] correspond au nombre de mot-clefs pour la carte->version*/
	{
		printf("Couleur %s : (", MOTCLEFS[MC_lu]);
		for(couleur=0; couleur < NB_COULEURS; couleur++)
		{
			printf("%hhu ", carte->couleurClef[MC_lu][couleur]);
					
		}
		printf(")\n");
	}
	/*** Chemins ***/
	printf("Nombre de noeuds : %d\n", carte->nombreNoeuds);
	for(indice=0; indice < carte->nombreNoeuds; indice++)
	{
		printf("Noeud d'indice %d : type : %s, coord : (%u, %u)\n", indice, TYPENOEUD[(carte->chemins)[indice]->type], (carte->chemins)[indice]->x, (carte->chemins)[indice]->y);
		printf("Successeurs (nombre = %d) : ", (carte->chemins)[indice]->nombreSuccesseurs);
		for(j=0; j < (carte->chemins)[indice]->nombreSuccesseurs; j++)
		{
			printf("%d, ", ((carte->chemins)[indice]->successeurs[j])->indice);
		}
		printf("\n");
	}
}