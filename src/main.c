#include "../include/main.h"

#include <unistd.h>

int main(int argc, char *argv[])
{
	
	/* nom et chemin des fichiers de la carte*/
	char nomDonnees[MAX_TAILLE_NOM_FICHIER];
	char cheminDonnees[MAX_TAILLE_CHEMIN_FICHIER];
	char cheminImage[MAX_TAILLE_CHEMIN_FICHIER];


	

	
	/*** JEU ***/
	Jeu *jeu;
	Carte *carte;
	/*Données carte*/
	jeu = allouerJeu();
	carte = jeu->carte;

	/*Données image*/
	PPM_Image *imageCarte;

	/*pour vérif*/
	int nombreModif;

	/** TEMPS **/
	clock_t tempsDebut = clock();
	clock_t tempsEcoule;
	clock_t deltaT;

	/* on initialise la graine pour le tirage pseudo-aléatoire */
	srand(time(NULL));

	/*vérif rapide.*/
	if(argc < 2)
	{
		printf("Si je pouvaiiis avoir un argument.\n");
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
	FILE *fichierImage = fopen(cheminImage, "r");
	if( !fichierImage )
	{
		printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
		return EXIT_FAILURE;
	}
	printf(" Fait !\n");
	/*Lecture de l'image associée*/
	printf("Lecture des données image...");
	if( !PPM_lireImage(fichierImage, &imageCarte) )
		return EXIT_FAILURE;
	

	afficheCarte();


	/*** Chargement de l'image associée ***/
	printf("Ouverture de la modification...");
	sprintf(cheminImage, "%s/%s_mod", REP_DONNEES_IMAGE, carte->nomImage);
	FILE *fichierImageSortie = fopen(cheminImage, "w");
	if( !fichierImageSortie )
	{
		printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
		return EXIT_FAILURE;
	}
	printf(" Fait !\n");
	/*Lecture de l'image associée*/
	printf("Écriture des données image...");
	if( !PPM_ecrireImage(fichierImageSortie, imageCarte) )
		return EXIT_FAILURE;
	fclose(fichierImageSortie);
	printf(" Fait !\n");
	/*****/

	/***Lancement du jeu ***/
	printf("Lancement du jeu...");
	lancerJeu(jeu);
	afficherVague(jeu->chaine);
	printf(" Fait !\n");
	printf("\n--- TRAITEMENT ---\n");
	deltaT = 0;
	tempsEcoule = 0;
	printf("Il était une fois\n");
	printf("Une petite erreur de segmentation \n");
	printf("L'étudiant voulait se débarrasser de cette erreur de segmentation.\n");
	printf("Il y parvint finalement au prix de lourds efforts.\n");
	while(1)
	{
		deltaT = clock() - tempsEcoule;
		tempsEcoule = clock() - tempsDebut;
		traitementJeu(jeu, deltaT);
		//afficherVague(jeu->chaine);
	}
	




	/*Fermeture des ressources ouvertes.*/
	printf("Libération des ressources...");
	libererJeu(jeu);
	PPM_libererImage(imageCarte);
	printf(" Fait !\n");


	return EXIT_SUCCESS;
}