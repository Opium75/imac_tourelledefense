#include "../include/main.h"

int main(int argc, char *argv[])
{
	/* nom et chemin des fichiers de la carte*/
	char nomDonnees[MAX_TAILLE_NOM_FICHIER];
	char cheminDonnees[MAX_TAILLE_CHEMIN_FICHIER];
	char cheminImage[MAX_TAILLE_CHEMIN_FICHIER];

	/*Données carte*/
	Carte *carte = allouerCarte();

	/*Données image*/
	PPM_Image *img;

	/*vérif rapide.*/
	if(argc < 2)
	{
		printf("Bonjour ! Je prends un argument, merci. Connard.\n");
		return EXIT_FAILURE;
	}

	/*** Chemin du fichier et Ouverture ***/
	printf("Chargement de la carte...");
	strcpy(nomDonnees, argv[1]);
	sprintf(cheminDonnees,"%s/%s",REP_DONNEES_CARTE, nomDonnees);
	FILE *fichierCarte = fopen(cheminDonnees,"r");
	if( !fichierCarte )
	{
		printf("Échec d'ouverture des données au chemin : %s\n",cheminDonnees);
		return EXIT_FAILURE;
	}
	printf(" Fait !\n");
	/*Lecture du fichier carte*/
	printf("Lecture des données carte...");
	if( !lireCarte(fichierCarte, carte) )
		return EXIT_FAILURE;
	fclose(fichierCarte); /*On ferme le fichier lu, plus besoin.*/
	printf(" Fait !\n");
	/*****/

	/*Affichage de la carte (console)*/
	/*afficherCarte(carte);*/

	/*** Chargement de l'image associée ***/
	printf("Chargement de l'image...");
	sprintf(cheminImage, "%s/%s", REP_DONNEES_IMAGE, carte->nomImage);
	FILE *imageCarte = fopen(cheminImage, "r");
	if( !imageCarte )
	{
		printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
		return EXIT_FAILURE;
	}
	printf(" Fait !\n");
	/*Lecture de l'image associée*/
	printf("Lecture des données image...");
	if( !PPM_lireImage(imageCarte, &img) )
		return EXIT_FAILURE;
	
	fclose(imageCarte);
	printf(" Fait !\n");
	/*****/

	/*Affichage de l'image (console, crade)*/
	/*PPM_afficherImage(img);*/
	


	/*Fermeture des ressources ouvertes.*/
	printf("Libération des ressources...");
	libererCarte(carte);
	PPM_libererImage(img);
	printf(" Fait !\n");


	return EXIT_SUCCESS;
}