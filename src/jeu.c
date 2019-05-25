#include "../include/jeu.h"

Jeu* creerJeu(unsigned char niveau, int pointage, int argent, Carte *carte, Cite *cite, Vague *chaine)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = niveau;
	jeu->pointage = pointage;
	jeu->argent = argent;
	jeu->carte = carte;
	jeu->cite = cite;
	jeu->chaine = chaine;
	return jeu;
}

Jeu* allouerJeu(void)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = 0;
	jeu->pointage = 0;
	jeu->argent = 0;
	jeu->carte = allouerCarte();
	jeu->cite = allouerCite();
	jeu->chaine = NULL;
	jeu->image = NULL;

	/** on initialise les ressources de l'affichage ailleurs **/
	jeu->scene = NULL;
	return jeu;
}

void GEN_jeu(Jeu *jeu)
{
	// que faire ?
}

void relancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);

	/** affichage **/
	////
}

void lancerJeu(Jeu *jeu)
{
	int i, id;
	/** VAGUE DE MONSTRES **/
	/* première itération */
	Vague *vague = creerVague(jeu->niveau, jeu->carte);
	jeu->chaine = vague;
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);

	/** AFFICHAGE **/
	/** LANCER LE CONTEXTE OPENGL D'AFFICHAGE
	*** AVANT DE CHARGER LES RESSOURCES !!!
	***/
	jeu->scene = lancerAffichage();
	chargerRessourcesAffichage(jeu->lutins, jeu->banqueAffichage, jeu->banqueTextures, jeu->image->dim);
	
}

void quitterJeu(Jeu *jeu)
{
	
	libererJeu(jeu);

}

void traitementJeu(Jeu* jeu, time_t deltaT)
{
	/* ici, on fait tout les calculs et changements,
	* la fonction est appelée à chaque itération du jeu
	*/
	int k;
	
	/* les vagues de monstres */

	traitementChaine(&(jeu->chaine), deltaT, jeu->carte, jeu->cite, &(jeu->niveau), &(jeu->pointage), &(jeu->argent) );

}

void boucleJeu(Jeu *jeu)
{
	bool boucle;
	clock_t deltaT, tempsDebut, tempsEcoule;
	Uint32 tempsDebut_SDL, tempsEcoule_SDL;

	/* temps au début de la boucle */
	tempsDebut = clock();
	tempsEcoule  = 0;
	/** BOUCLE **/
	boucle = true;
	while(boucle)
	{
		/*récup temps au début de la boucle*/
		tempsDebut_SDL = SDL_GetTicks();
		deltaT = clock() - tempsEcoule;
		tempsEcoule = clock() - tempsDebut;

		/*** TRAITEMENT ***/
		traitementJeu(jeu, deltaT);
		
		//terminalVague(jeu->chaine);

		glClear(GL_COLOR_BUFFER_BIT);
		/** AFFICHAGE ***/
		glLoadIdentity();

		afficherJeu(jeu);
		
		//afficherCarte();

		boucle = interfaceJeu(jeu);

		

		/*Échange du tampon arrière et avant -> mise à jour fenêtre*/
		SDL_GL_SwapBuffers();

		/** *ÉVÉNEMENTS -- INTERFACE  */
		// autre fonction

		/* Calcul du temps écoulé*/
		tempsEcoule_SDL = SDL_GetTicks() - tempsEcoule_SDL;
		/*Pause éventuell du programme si trop peu de temps écoulé*/
		if( tempsEcoule_SDL < FRAMERATE_MILLISECONDS)
		{
			SDL_Delay(FRAMERATE_MILLISECONDS - tempsEcoule_SDL);
		}
	}
	SDL_Quit();
}

bool interfaceJeu(Jeu *jeu)
{
	/* ÉVÉNEMENTS SDL */
	SDL_Event e;
	bool boucle = true;
	while(SDL_PollEvent(&e))
	{
		 /* L'utilisateur ferme la fenetre : */
		if(e.type == SDL_QUIT) 
		{
			boucle = false;
			break;
		}
	
		if(	e.type == SDL_KEYDOWN 
			&& (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
		{
			boucle = false; 
			break;
		}
		/* Quelques exemples de traitement d'evenements : */
        switch(e.type) 
        {
            /* Redimensionnement fenetre */
			case SDL_VIDEORESIZE:
                //reshape(&surface, e.resize.w, e.resize.h);
                //glMatrixMode(GL_MODELVIEW);
                break;

            /* Clic souris */
            case SDL_MOUSEBUTTONUP:
            	gestionClic(jeu, &e);
                printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                break;
            
            /* Touche clavier */
            case SDL_KEYDOWN:
                printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                break;
                
            default:
                break;
        }
	}
	return boucle;
}

void gestionTouche()
{
 
}

void gestionClic(Jeu *jeu, SDL_Event *e)
{
	Point coordClique;
	calculerCoordonneesEchelle(&coordClique, e->button.x, e->button.y, jeu->image->dim);
	afficherPoint(jeu->image->dim);
	Tour *tour = creerTour(T_rouge, coordClique.x, coordClique.y);
	 printf("%u, %u | %u, %u  :: (%d, %d)|(%u, %u) \n", jeu->image->dim->x, jeu->image->dim->y, LARGEUR_FENETRE, HAUTEUR_FENETRE, e->button.x, e->button.y, tour->coord->x, tour->coord->y);
	ajouterTourCite(tour, jeu->cite);
}

void afficherJeu(Jeu *jeu)
{
    /* on afficher la cité, la chaîne de monstres */
    afficherCarte(); //à redimensionner
    //peut-être mettre en arguments le score et l'argent et changer le type d'argu que la fonction attend : afficher Carte(jeu->pointage, jeu->argent);
  	 
    afficherCite(jeu->cite, jeu->banqueAffichage, jeu->banqueTextures, jeu->image->dim);
   
    afficherChaine(jeu->chaine, jeu->banqueAffichage, jeu->banqueTextures, jeu->image->dim);
}

void libererJeu(Jeu *jeu)
{
	/* on libère tout */
	libererCarte(jeu->carte);
	libererCite(jeu->cite);

	libererChaine(jeu->chaine);
	PPM_libererImage(jeu->image);
	/** l'affichage aussi **/
	libererRessourcesAffichage(jeu->lutins, jeu->banqueAffichage, jeu->banqueTextures);
	fermerAffichage(jeu->scene);
	free(jeu);
}