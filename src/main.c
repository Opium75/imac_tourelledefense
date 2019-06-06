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
	printf("Chargement du jeu...");
	chargerJeu(jeu);

	////* Musique *////

	Mix_Music* musicJeu;

    /* On initialise le son */
    SDL_Init(SDL_INIT_AUDIO);

    /* On ouvre un device audio */
    //Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,1024);

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
      printf("%s", Mix_GetError());
    }

    /* On charge un fichier musical */
    musicJeu = Mix_LoadMUS("./images/music.mp3");
    printf("musiiiiiiic\n");

    Mix_PlayMusic(musicJeu, 1);

	/////* fin musique *////


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