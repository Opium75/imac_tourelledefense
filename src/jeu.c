#include "../include/jeu.h"

Jeu* allouerJeu(void)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = 0;
	jeu->joueur = allouerJoueur();
	jeu->carte = allouerCarte();
	jeu->cite = allouerCite();
	jeu->chaine = NULL;
	jeu->image = NULL;

	/** on initialise les ressources de l'affichage ailleurs **/
	jeu->ressources = allouerRessources();
	return jeu;
}

Joueur* allouerJoueur(void)
{
	Joueur *joueur = malloc(sizeof(Joueur));
	if( !joueur )
	{
		printf("Joueur -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	joueur->pointage = 0;
	joueur->argent = ARGENT_DEPART;
	joueur->memTouche = '\0';
	return joueur;
}

void libererJoueur(Joueur *joueur)
{
	free(joueur);
}

bool preparerJeu(Jeu *jeu, char *nomDonnees)
{
	/* nom et chemin des fichiers de la carte*/
	char cheminDonnees[MAX_TAILLE_CHEMIN_FICHIER];
	char cheminImage[MAX_TAILLE_CHEMIN_FICHIER];

	int nombreModif;

	/*** Chemin du fichier et Ouverture ***/
	printf("Chargement de la carte...");
	sprintf(cheminDonnees,"%s/%s",REP_DONNEES_CARTE, nomDonnees);
	FILE *fichierCarte = fopen(cheminDonnees,"r");
	if( !fichierCarte )
	{
		printf("Échec d'ouverture des données au chemin : %s\n",cheminDonnees);
		return false;
	}
	printf(" Fait !\n");
	/*Lecture du fichier carte*/
	printf("Lecture des données carte...");
	if( !lireCarte(fichierCarte, jeu->carte) )
		return false;
	fclose(fichierCarte); /*On ferme le fichier lu, plus besoin.*/
	printf(" Fait !\n");
	/*****/

	/*Affichage de la carte (console)*/
	/*afficherCarte(carte);*/

	/*** Chargement de l'image associée ***/
	printf("Chargement de l'image...");
	sprintf(cheminImage, "%s/%s", REP_DONNEES_IMAGE, jeu->carte->nomImage);
	FILE *fichierImage = fopen(cheminImage, "r");
	if( !fichierImage )
	{
		printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
		return false;
	}
	printf(" Fait !\n");
	/*Lecture de l'image associée*/
	printf("Lecture des données image...");
	if( !PPM_lireImage(fichierImage, &(jeu->image) ) )
		return false;
	printf(" Fait ! \n");
	

	/** VÉRIFICATIONS **/
	printf("Vérification des chemins...");
	if( !validerChemins(jeu->carte, jeu->image, &nombreModif) )
	{
		printf("IMAGE INCORRECTE (modifications : %d)\n", nombreModif);
		printf("Ouverture de la modification...");
		sprintf(cheminImage, "%s/%s_mod", REP_DONNEES_IMAGE, jeu->carte->nomImage);
		FILE *fichierImageSortie = fopen(cheminImage, "w");
		if( !fichierImageSortie )
		{
			printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
			return false;
		}
		printf(" Fait !\n");
		/*Lecture de l'image associée*/
		printf("Écriture des données image...");
		if( !PPM_ecrireImage(fichierImageSortie, jeu->image ) )
			return false;
		fclose(fichierImageSortie);
		printf(" Fait !\n");
			return false;
	}
	printf(" Fait ! \n");
	/*** Chargement de l'image associée ***/
	return true;
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
	/** LANCER LE CONTEXTE OPENGL D'AFFICHAGE
	*** AVANT DE CHARGER LES RESSOURCES !!!
	***/
	/*** Pour une raison obscure, SDL_Init provoque une erreur de seg
	*** si on appelle lancerAffichage après le lancement de la vague
	*** depuis le passage à plusieurs sorties
	***/
	SDL_Surface *scene;
	lancerAffichage(&scene);
	jeu->scene = scene;
	/** VAGUE DE MONSTRES **/
	/* première itération */
	Vague *vague = creerVague(jeu->niveau, jeu->carte);
	jeu->chaine = vague;
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);
	/* */
	/* chargement des ressources */
	char *nomArrierePlan = jeu->carte->nomArrierePlan;
	bool possedeArrierePlan = jeu->carte->possedeArrierePlan;
	chargerRessourcesAffichage(jeu->ressources, jeu->image->dim, possedeArrierePlan, nomArrierePlan);
}

void quitterJeu(Jeu *jeu)
{
	libererJeu(jeu);
}

bool traitementJeu(Jeu* jeu, time_t deltaT)
{
	bool fin = false;
	int gainPoints, gainArgent;
	int pertePoints, perteArgent;
	gainPoints = gainArgent = pertePoints = perteArgent = 0;
	/* ici, on fait tout les calculs et changements,
	* la fonction est appelée à chaque itération du jeu
	*/
	/* les vagues de monstres */
	//terminalMonstre(jeu->chaine->monstres[0]);
	traitementChaine(&(jeu->chaine), deltaT, jeu->carte, jeu->cite, &(jeu->niveau), &pertePoints, &perteArgent);
	/* la vague, les monstres ont été mis à jour.
	* On traite maintenant la cité.
	*/
	traitementCite(jeu->cite, deltaT, jeu->carte, jeu->chaine->monstres, jeu->chaine->nombreMonstres, &gainPoints, &gainArgent);
	/* on rajoute, soustrait des points, de l'argent, au joueur */
	fin = traitementJoueur(jeu->joueur, gainPoints, gainArgent, pertePoints, perteArgent);
	return fin;
}

bool traitementJoueur(Joueur *joueur, int gainPoints, int gainArgent, int pertePoints, int perteArgent)
{
	bool fin = false;
	/* on modifie le total de points, argent si besoin */
	if( (gainPoints - pertePoints) )
		joueur->pointage += (gainPoints - pertePoints);
	if( (gainArgent - perteArgent) )
		joueur->argent += (gainArgent - perteArgent);
	/* le joueur a-t-il perdu ? */
	if( joueur->argent < 0 )
	{
		fin = true;
	}
	return fin;
}
void boucleJeu(Jeu *jeu)
{
	bool boucle, fin;
	clock_t deltaT, tempsDebut, tempsEcoule;
	Uint32 tempsDebut_SDL, tempsEcoule_SDL;
	/** CE QUI SUIT EST FAUX, MAIS C'EST AUSSI UNE POSSIBILITÉ
	** ON POURRAIT CHANGER SI PLUS LOGIQUE
	**/
	/* on gère le gain dans la boucle de jeu
	* et on l'applique dans le traitement
	* sachant que om est modifié par le traitement 
	* (victoire sur monstre, attaque du monstre )
	* et l'interface (constructions, destruction)
	*/
	//int gainPoints, gainArgent, pertePoints, perteArgent;

	/* temps au début de la boucle */
	tempsDebut = clock();
	tempsEcoule  = tempsDebut;
	/** BOUCLE **/
	boucle = true;
	fin = false;
	while(boucle)
	{
		/*récup temps au début de la boucle*/
		tempsDebut_SDL = SDL_GetTicks();
		deltaT = clock() - tempsEcoule;
		tempsEcoule = clock();

		
		/* si l'on a pas encore fini le jeu (perdu) */
		if( !fin )
		{
			/*** TRAITEMENT ***/
			fin = traitementJeu(jeu, deltaT);

			/** AFFICHAGE ***/
			afficherJeu(jeu);

			boucle = interfaceJeu(jeu);
		}
		else
		{
			/* on affiche l'écran de fin */
			afficherJeuFin(jeu);

			boucle = interfaceJeuFin(jeu);
		}

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
                break;

            /* Clic souris */
            case SDL_MOUSEBUTTONUP:
            	gestionClic(jeu, &e);
              	break;
            
            /* Touche clavier */
            case SDL_KEYDOWN:
            	gestionTouche(jeu, &e);
                break;
                
            default:
                break;
        }
	}
	return boucle;
}

bool interfaceJeuFin(Jeu *jeu)
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
                break;

            /* Clic souris */
            case SDL_MOUSEBUTTONUP:
                break;
            
            /* Touche clavier */
            case SDL_KEYDOWN:
                break;
                
            default:
                break;
        }
	}
	return boucle;
}

void gestionTouche(Jeu *jeu, SDL_Event *e)
{
	/* on enregistre la touche pressée
	* dans la mémoire tampon du joueur
	*/
	jeu->joueur->memTouche = e->key.keysym.sym;
}

void gestionClic(Jeu *jeu, SDL_Event *e)
{
	Point coordClique;
	calculerCoordonneesEchelle(&coordClique, e->button.x, e->button.y, jeu->image->dim);

	/* */
	gestionConstruction(jeu->joueur, jeu->cite, jeu->carte, &coordClique);
}

void gestionConstruction(Joueur *joueur, Cite *cite, Carte *carte, Point *coordClique)
{
	int type, coutAchat;
	int argentJoueur;
	int gainPoints, gainArgent, pertePoints, perteArgent;
	/* réinitialiser les gains, pertes */
	gainPoints = gainArgent = pertePoints = perteArgent = 0;
	/* copie protective */
	argentJoueur = joueur->argent;
	type = toucheVersTypeTour(joueur->memTouche);
	if( type != -1)
	{
		/* si ajout d'un bâtiment (tour pour l'instant) */
		coutAchat = calculerCout(type);
		if( argentJoueur >= (int)coutAchat )
		{
			if( construireTour(cite, carte->chemins, carte->nombreNoeuds, type, coordClique) )
			{
				perteArgent = coutAchat;
				printf("Tour construite en ");
				afficherPoint(coordClique);
			}
			else
			{
				printf("Échec de construction : Espace occupé en");
				afficherPoint(coordClique);
			}
		}
		else
		{
			printf("Échec de construction : ressources insuffisantes pour tour %d. (coût = %u)\n", type, coutAchat);
		}
	}
	else
	{
		/* modification, suppression d'un bâtiment ? */
	}
	/* on soustrait (ou ajoute, dans le cas d'une suppression) l'argent  au joueur */
	traitementJoueur(joueur, gainPoints, gainArgent, pertePoints, perteArgent);
}

int calculerRang(Joueur *joueur)
{
	int rang;
	rang = (int)joueur->pointage/PALIER_RANG;
	if( rang > NB_RANGS)
		rang = NB_RANGS-1;
	return rang;
}

void afficherJoueur(Joueur *joueur, Dimensions *dimImage)
{
	afficherPointage(joueur->pointage, dimImage);
	afficherArgent(joueur->argent, dimImage);
	afficherTouche(joueur->memTouche, dimImage);
}

void afficherJoueurFin(Joueur *joueur, Dimensions *dimImage)
{
	/* pour l'écran de fin */
	afficherPointage(joueur->pointage, dimImage);
}



void afficherJeu(Jeu *jeu)
{
	glClear(GL_COLOR_BUFFER_BIT);
	Ressources *ressources = jeu->ressources;
   /* l'arrière-plan s'il existe */
	if( jeu->carte->possedeArrierePlan )
	{
		 afficherCarte(ressources->affichageArrierePlan, jeu->image->dim);
	}
    //peut-être mettre en arguments le score et l'argent et changer le type d'argu que la fonction attend : afficher Carte(jeu->pointage, jeu->argent);
  	
  	/* on afficher la cité, la chaîne de monstres */
    afficherCite(jeu->cite, ressources->banqueAffichage, ressources->listeDim, jeu->image->dim);

    afficherChaine(jeu->chaine, ressources->banqueAffichage, ressources->listeDim, jeu->image->dim);

    /* le joueur, et les textes associés */
	afficherNiveau(jeu->niveau, jeu->image->dim);
    afficherJoueur(jeu->joueur, jeu->image->dim);
}

void afficherJeuFin(Jeu *jeu)
{
	int rang;
	Ressources *ressources = jeu->ressources;
	/* l'écran de fin */
	glClear(GL_COLOR_BUFFER_BIT);

	/* calcul du rang à chaque boucle, lourd
	* mais pas grave.
	*/
	rang = calculerRang(jeu->joueur);
	afficherNiveau(jeu->niveau, jeu->image->dim);
	/* LE TEXTE DU RANG ET SON IMAGE */
	afficherRang(rang, jeu->image->dim);
	afficherImageRang(rang, ressources->rangAffichage[rang], &POSITION_IMAGE_RANG, &DIM_IMAGE_RANG, jeu->image->dim);
	/* */
	afficherJoueurFin(jeu->joueur, jeu->image->dim);
}

void libererJeu(Jeu *jeu)
{
	/* on libère tout */
	libererJoueur(jeu->joueur);
	libererCarte(jeu->carte);
	/* LIBÉRER LA CHAÎNE AVANT LA CITÉ
	* CAR LA CHAÎNE RÉINITIALISE LES CIBLES DES TOURS
	*/
	libererChaine(jeu->chaine, jeu->cite->listeTour);
	libererCite(jeu->cite);
	PPM_libererImage(jeu->image);
	/** l'affichage aussi **/
	libererRessources(jeu->ressources);
	fermerAffichage(jeu->scene);
	free(jeu);
}



void debug(void)
{
	static int woof = 0;
	if( !(woof%6) )
		printf("\nWho let the dogs out ?\n");
	else
		printf("Woof\n");
	woof++;
}

int toucheVersTypeTour(char touche)
{
	int i;
	for( i=0; i<NB_TYPES_TOUR; i++ )
	{
		if( TOUCHES_TOUR[i] == touche )
			return (TypeTour) i;
	}
	return -1;
}