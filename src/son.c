#include "../include/M_son/son.h"

void SDL_initialiserAudio()
{
	/* On initialise le son */
    if( SDL_Init(SDL_INIT_AUDIO) == -1 )
    {
    	printf("SDL_Init : échec de lancement de l'audio.\n");
        printf("SDL_Init : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

}


void SON_allouerAudio()
{
	/* IL FAUT AVOIR LANCÉ SDL_Init POUR L'AUDIO AVANT */
	/* charger l'API */
	/* APPELER CETTE FONCTION AVANT D'UTILISER SDL_Mixer */
	/* pour l'instant, on prend des valeurs par défaut
	* pour la fréquence d'échantillionnage (22050 Hz)
	* Un Octet non signé pour la résolution
	* nombre de chaînes par défaut (8)
	* Un tampon  
	*/
	if(-1 == Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, 
					 AUDIO_U8, MIX_CHANNELS,
					 MIX_TAMPON_SON) 
					)
	{
		printf("SON_allouerAudio : échec de lancement de l'audio.\n");
		printf("Mix_OpenAudio : %s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
}

void SON_libererAudio()
{
	/* quitte et libère l'api */
	Mix_CloseAudio();
}

void SON_initialiserMix()
{
	/* on initialise SDL_mixer pour les formats MOD et OGG */
	int initialise = Mix_Init(MIX_FORMAT_DRAPEAUX);
	/* on s'assure du succè sde l'opération */
	/* la fonction renvoie un binaire de tous les drapeaux initialisés
	* avec succès, on s'assure que chacun a réussi 
	*/
	if( initialise & MIX_FORMAT_DRAPEAUX != MIX_FORMAT_DRAPEAUX )
	{
		printf("SON_initialiserSon : échec d'initialisation des formats requis.\n");
		printf("Mix_Init : %s\n", Mix_GetError());

		/* que faire ?*/
	}
}

void SON_quitterMix()
{
	/* on quitte le contexte SDL_mixer pour tous les formats initialisés */
	Mix_Quit();
}

Mix_Chunk* SON_chargerEchantillon( char *chemin )
{
	Mix_Chunk *echant;
	/* ATTENTION : la fonction charge les formats WAVE, AIFF, RIFF, OGG, VOC
	* le nom induit en erreur.
	*/
	echant = Mix_LoadWAV(chemin);
	if( !echant )
	{
		printf("SON_chargerEchant : échec de chargement.\n");
		printf("Mix_LoadWAV : %s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
	return echant;
}

void SON_libererEchantillion(Mix_Chunk **echant)
{
	/* Il est apparemment mieux de fixer
	* au pointeur nul après l'avoir libérer
	* dans le contexte SDL_mixer.
	*/
	Mix_FreeChunk(*echant);
	*echant = NULL;

}

Mix_Music* SON_chargerMusique(const char *chemin)
{
	Mix_Music *musique;
	musique = Mix_LoadMUS(chemin);
	if( !musique )
	{
		printf("SON_chargerMusique : échec de chargement.\n");
		printf("Mix_LoadMUS : %s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
	return musique;
}

void SON_libererMusique(Mix_Music **musique)
{
	Mix_FreeMusic(*musique);
	*musique = NULL;
}

int 
