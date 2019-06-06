#include "../include/fichier.h"

void sautLigne(FILE *fichierCarte)
{
	while( fgetc(fichierCarte) != CODE_SAUT_LIGNE || feof(fichierCarte) )
	{
					/*rien à faire ici */
	}
}

int lireEntier(FILE *fichierCarte, char position[],int ligne, int *e)
{
	int format = fscanf(fichierCarte, "%d", e);
	if( !format )
	{
		printf("%s -- Ligne n°%d, erreur de formatage : entier attendu.\n", position, ligne);
		return 0;	
	}
	return format;
}

int lireChaine(FILE *fichierCarte, char position[],int ligne, char chaine[])
{
	int format = fscanf(fichierCarte, "%s", chaine);
	if( !format )
	{
		printf("%s -- Ligne n°%d, erreur de formatage : chaîne de caractères attendue.\n", position, ligne);
		return 0;
	}
	return format;
}