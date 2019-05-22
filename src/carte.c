#include "../include/carte.h"


Carte* allouerCarte(void)
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
	libererIndicesEntrees(carte->indicesEntrees);
	free(carte);
}

void terminalCarte(Carte *carte)
{
	MotClef MC_lu;
	printf("\n--- AFFICHAGE CARTE ---\n");
	/*** Version ***/
	printf("Version carte : %d\n", carte->version);
	/*** Image ***/
	printf("Image associée : %s\n", carte->nomImage);
	/*** Paramètres ***/
	printf("Énergie : %u\n", carte->energie);
	
	for( MC_lu=0; MC_lu < NB_COULEURS_CLEFS; MC_lu++) /*NB_PARAM_PAR_VERSION[carte->version-1] correspond au nombre de mot-clefs pour la carte->version*/
	{
		printf("Couleur %s : ", MOTCLEFS[MC_lu]);
		afficherCouleur(carte->couleurClef[MC_lu]);
	}
	/*** Chemins ***/
	afficherGraphe(carte->chemins, carte->nombreNoeuds);
	printf("\n--- FIN AFFICHAGE ---\n");
}

MotClef correspondanceType(TypeNoeud type)
{
	switch(type)
	{
		case entree :
			return MC_in;
		case sortie :
			return MC_out;
		case coude: case intersection :
			return MC_chemin;
		default :
			printf("Échec de correspondance du type %d à un mot-clef.\n", type);
			exit(EXIT_FAILURE);
	}
}