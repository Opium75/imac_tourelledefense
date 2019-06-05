#include "../include/main.h"


int main(int argc, char *argv[])
{


	/*vérif rapide.*/
	if(argc < 2)
	{
		printf("Si je pouvaiiis avoir un argument.\n");
		return EXIT_FAILURE;
	}
		
	/*** JEU ***/
	Jeu *jeu;
	/*Données carte*/
	jeu = allouerJeu();

	

	/** TEMPS **/
	/* on initialise la graine pour le tirage pseudo-aléatoire */
	srand(time(NULL));

	/*****/
	if( !preparerJeu(jeu, argv[1]) )
	{
		printf("Jeu -- Échec de préparation.\n");
		return EXIT_FAILURE;
	}
	//terminalCarte(jeu->carte);
	/***Lancement du jeu ***/
	printf("Lancement du jeu...");
	lancerJeu(jeu);
	//terminalVague(jeu->chaine);
	printf(" Fait !\n");
	printf("\n--- TRAITEMENT ---\n");
	/*printf("Il était une fois\n");
	printf("Une petite erreur de segmentation \n");
	printf("L'étudiant voulait se débarrasser de cette erreur de segmentation.\n");
	printf("Il y parvint finalement au prix de lourds efforts.\n");*/
	boucleJeu(jeu);
	printf("\n--- FIN TRAITEMENT ---\n");



	/*Fermeture des ressources ouvertes.*/
	printf("Libération des ressources...");
	quitterJeu(jeu);
	printf(" Fait !\n");
	return EXIT_SUCCESS;
}