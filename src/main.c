#include "../include/main.h"

int main(int argc, char *argv[])
{
	/* nom et chemin des fichiers de la carte*/
	char nomImage[MAX_TAILLE_NOM_FICHIER];
	char nomDonnees[MAX_TAILLE_NOM_FICHIER];
	char cheminImage[MAX_TAILLE_CHEMIN_FICHIER];
	char cheminDonnees[MAX_TAILLE_CHEMIN_FICHIER];

	/*données de la carte*/
	int energie;
	unsigned char couleurClef[NB_COULEURS_CLEFS][NB_COULEURS];

	/*vérif rapide.*/
	if(argc < 2)
	{
		printf("Entre le nom de la carte, connard.\n");
		return EXIT_FAILURE;
	}

	/*chemin du fichier et ouverture*/
	strcpy(nomDonnees, argv[1]);
	sprintf(cheminDonnees,"./data/%s",nomDonnees);
	FILE *carte = fopen(cheminDonnees,"r");
	if( !carte)
	{
		printf("Échec d'ouverture des données au chemin : %s\n",cheminDonnees);
		return EXIT_FAILURE;
	}

	/*premier essai*/
	lireCarte(carte, &energie, nomImage, couleurClef);

	
	return EXIT_SUCCESS;
}