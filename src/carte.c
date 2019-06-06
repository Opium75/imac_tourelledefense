#include "../include/carte.h"


Carte* allouerCarte(void)
{
	Carte *carte = malloc(sizeof(Carte));
	if( !carte )
	{
		printf("Échec d'allocation de la carte.\n");
		exit(EXIT_FAILURE);
	}
	carte->chemins = NULL;
	carte->indicesEntrees = NULL;
	carte->indicesSorties =NULL;
	carte->nombreSorties = NULL;
	return carte;
}

void libererCarte(Carte *carte)
{
	libererGraphe(carte->nombreNoeuds, carte->chemins);
	libererIndicesEntrees(carte->indicesEntrees);
	libererIndicesSorties(carte->indicesSorties, carte->nombreEntrees);
	free(carte);
}

void terminalCarte(Carte *carte)
{
	MotClef MC_lu;
	OPT_MotClef OPT_lue;
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
	if( carte->possedeDecor )
	{
		printf("(*) Décor : %s\n", carte->nomDecor);
	}

	/*** Chemins ***/
	printf("Entrées :\n");
	afficherVecteurEntier(carte->indicesEntrees, carte->nombreEntrees);
	printf("Sorties :\n");
	for( int i=0; i<carte->nombreEntrees; i++ )
	{
		printf("Sorties de l'entrée n°%d : ", i);
		afficherVecteurEntier(carte->indicesSorties[i], carte->nombreSorties[i]);
	}
	afficherGraphe(carte->chemins, carte->nombreNoeuds);
	printf("\n--- FIN AFFICHAGE ---\n");
}


int correspondanceIndicesEntrees(int indice, int *indicesEntrees, int nombreEntrees)
{
	int k;
	k=0;
	while( (k<nombreEntrees) && indicesEntrees[k] != indice )
		k++;
	if( k == nombreEntrees)
	{
		/* ce noeud n'est pas dans la liste des entrée, problème qqp */
		printf("Parcours -- Le noeud d'indice n°%d n'est pas dans la liste des entrées.\n", indice);
		return -1;
	}
	else
		return k;

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