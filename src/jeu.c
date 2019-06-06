#include "../include/jeu.h"

/*char txtP[25];
char touchecode = '\0';

int taille = 1;
char* vecteur[] = {"Cest le tableau pour mettre la taille des textes a afficher sur l ecran"};
glutInit(taille, vecteur);*/

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

	/* on ne l'a pas encore lancé */
	jeu->etat = nonLance;
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

void reinitialiserJoueur(Joueur *joueur)
{
	/* valeurs de départ */
	joueur->pointage = 0;
	joueur->argent = ARGENT_DEPART;
	joueur->memTouche = '\0';

}

void recommencerJeu(Jeu *jeu)
{
	

	/* On repart au premier niveau */
	jeu->niveau = 0;
	/* on réinitialise le joueur */
	reinitialiserJoueur(jeu->joueur);
	/* oN RÉALLOUE LA CITÉ */
	jeu->cite = allouerCite();
	/* on revient au menu */
	jeu->etat = nonLance;
	/** affichage **/
	////
}

void lancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	/* première itération */
	Vague *vague = creerVague(jeu->niveau, jeu->carte);
	jeu->chaine = vague;
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);
	/* */
	jeu->etat = lance;
}

void chargerJeu(Jeu *jeu)
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

	
	/* chargement des ressources */
	char *nomDecor = jeu->carte->nomDecor;
	bool possedeDecor = jeu->carte->possedeDecor;
	chargerRessourcesAffichage(jeu->ressources, jeu->image->dim, possedeDecor, nomDecor);
}

void quitterJeu(Jeu *jeu)
{
	libererJeu(jeu);
}

void finirJeu(Jeu *jeu)
{
	/* On libère les vagues et la cité */
	libererChaine(jeu->chaine, NULL);
	libererCite(jeu->cite);
	/* ON OUBLIE PAS DE RÉINITIALISER LES POINTEURS LIBÉRÉS*/
	jeu->chaine = NULL;
	jeu->cite = NULL;
	/* */
	jeu->etat = fini;
}

void traitementJeu(Jeu* jeu, time_t deltaT)
{
	bool fin;
	int gainPoints, gainArgent;
	int pertePoints, perteArgent;
	gainPoints = gainArgent = pertePoints = perteArgent = 0;
	/* ici, on fait tout les calculs et changements,
	* la fonction est appelée à chaque itération du jeu
	*/
	switch( jeu->etat )
	{
		case nonLance :
			break;
		case lance :
			/* les vagues de monstres */
			traitementChaine(&(jeu->chaine), deltaT, jeu->carte, jeu->cite, &(jeu->niveau), &pertePoints, &perteArgent);
			/* la vague, les monstres ont été mis à jour.
			* On traite maintenant la cité.
			*/
			traitementCite(jeu->cite, deltaT, jeu->carte, jeu->chaine->monstres, jeu->chaine->nombreMonstres, &gainPoints, &gainArgent);
			/* on rajoute, soustrait des points, de l'argent, au joueur */
			fin = false;
			fin = traitementJoueur(jeu->joueur, gainPoints, gainArgent, pertePoints, perteArgent);
			if( fin )
				finirJeu(jeu);
			break;
		case enPause :
			break;
		case fini :
			break;
	}
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
	bool boucle;
	clock_t deltaT, tempsDebut, tempsEcoule;
	Uint32 tempsEcoule_SDL;
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
	while(boucle)
	{
		/*récup temps au début de la boucle*/
		tempsEcoule_SDL = SDL_GetTicks();
		deltaT = clock() - tempsEcoule;
		tempsEcoule = clock();
		
		/*** TRAITEMENT ***/
		traitementJeu(jeu, deltaT);

		/** AFFICHAGE ***/
		afficherJeu(jeu);

		boucle = interfaceJeu(jeu);
		/* si l'on a pas encore fini le jeu (perdu) */
		
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
        switch(e.type) 
        {
            /* Redimensionnement fenetre */
			case SDL_VIDEORESIZE :
				/* non. */
                break;

            /* Clic souris */
            case SDL_MOUSEBUTTONUP :
            	gestionClic(jeu, &e);
                break;
            
            /* Touche clavier */
            case SDL_KEYDOWN:
            	gestionTouche(jeu, &e);
            	//touchecode = e.key.keysym.sym;
                break;
                
            default:
                break;
        }
	}
	return boucle;
}

void gestionTouche(Jeu *jeu, SDL_Event *e)
{
	
	switch( jeu->etat )
	{
		case nonLance :
			break;
		case lance :
			/* est-ce que l'on met en pause ?*/
			if( e->key.keysym.sym == TOUCHE_PAUSE )
			{
				jeu->etat = enPause;
			}
			else
			{
				/* on enregistre la touche pressée
				* dans la mémoire tampon du joueur
				*/
				jeu->joueur->memTouche = e->key.keysym.sym;
			}
			break;
		case enPause :
			/* si le jeu est en pause et on rappuie sur la touche
			* on sort de la pause
			*/
			if( e->key.keysym.sym == TOUCHE_PAUSE )
			{
				jeu->etat = lance;
			}
			break;
		case fini :
			break;
	}

}

void gestionClic(Jeu *jeu, SDL_Event *e)
{
	Point coordClique;
	calculerCoordonneesEchelle(&coordClique, e->button.x, e->button.y, jeu->image->dim);
	switch( jeu->etat )
	{
		case nonLance :
			/* si l'on a appuyé sur le bouton ? */
			gestionMenu(jeu, &coordClique);
			break;
		case lance :
			/* on gère la construction */
			gestionConstruction(jeu->joueur, jeu->cite, jeu->carte, &coordClique);
			break;
		case enPause :
			/* on autorisera pas la construction pendant la pause ? */
			break;
		case fini :
			gestionFin(jeu, &coordClique);
			break;
	}
	/* */
}

void gestionMenu(Jeu *jeu, Point *coordClique)
{
	/* le bouton correspondra au texte */
	bool estClique;
	Point coordBouton;
	Dimensions dimBoutonMenu;
	/* premier calcul de la position du bouton de menu */
	calculerCoordonneesPourcentage(&coordBouton, &POSITION_BOUTON_MENU, jeu->image->dim);
	calculerDimensionsPourcentage(&dimBoutonMenu, &DIM_BOUTON_MENU, jeu->image->dim);
	/* est-ce que l'on a cliqué sur le bouton ?*/
	estClique = boutonEstClique(&coordBouton, &dimBoutonMenu, coordClique, jeu->image->dim);
	if( estClique )
	{
		/* on lance le jeu */
		lancerJeu(jeu);
	}
}

void gestionFin(Jeu *jeu, Point *coordClique)
{
	/* le bouton correspondra au texte */
	bool estClique;
	Point coordBouton;
	Dimensions dimBoutonRecom;
	/* premier calcul de la position du bouton de menu */
	calculerCoordonneesPourcentage(&coordBouton, &POSITION_BOUTON_RECOM, jeu->image->dim);
	calculerDimensionsPourcentage(&dimBoutonRecom, &DIM_BOUTON_RECOM, jeu->image->dim);
	/* est-ce que l'on a cliqué sur le bouton ?*/
	estClique = boutonEstClique(&coordBouton, &dimBoutonRecom, coordClique, jeu->image->dim);
	if( estClique )
	{
		/* on revient au  */
		recommencerJeu(jeu);
	}
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
	if( rang < 0 )
		rang = 0;
	return rang;
}

void afficherJoueur(Joueur *joueur, EtatJeu etat, Dimensions *dimImage)
{
	switch(etat)
	{
		case nonLance :
			break;
		case lance : case enPause :
			afficherPointage(joueur->pointage, dimImage);
			afficherArgent(joueur->argent, dimImage);
			afficherTouche(joueur->memTouche, dimImage);
			break;
		case fini :
			/* pour l'écran de fin */
			afficherPointage(joueur->pointage, dimImage);
	}
	
}

void afficherArrierePlan(EtatJeu etat, Ressources *ressources, Dimensions *dimImage)
{
	/* on détermine l'arrière-plan à afficher */
	int indice = correspondanceEtatJeuArrierePlan(etat);
	if( indice == -1 )
	{
		/* on affiche rien */

		return;
	}
	GLuint affichageArrierePlan = ressources->arrierePlanAffichage[indice];
    /* l'arrière-plan remplace la carte */
    afficherElement(affichageArrierePlan);
}

void afficherMenu(Ressources *ressources, Dimensions *dimImage)
{
	/* les textes */
	afficherTitreMenu(dimImage);
	afficherCredits(dimImage);
	/* le bouton de menu */
  	afficherBouton(BOUT_menu, ressources->boutonAffichage, dimImage);
}

void afficherFin(Jeu *jeu)
{
	int rang;
	Ressources *ressources = jeu->ressources;
	rang = calculerRang(jeu->joueur);
	afficherNiveau(jeu->niveau, jeu->image->dim);
	/* LE TEXTE DU RANG ET SON IMAGE */
	afficherImageRang(rang, ressources->rangAffichage[rang], &POSITION_IMAGE_RANG, &DIM_IMAGE_RANG, jeu->image->dim);
	afficherTexteRang(rang, jeu->image->dim);
	/* */
	afficherJoueur(jeu->joueur, jeu->etat, jeu->image->dim);
	afficherCredits(jeu->image->dim);
	/* le bouton pour recommencer */
	afficherBouton(BOUT_redem, ressources->boutonAffichage, jeu->image->dim);
}


void afficherJeu(Jeu *jeu)

{
	Ressources *ressources = jeu->ressources;
	/* on efface l'écran précédent */
	glClear(GL_COLOR_BUFFER_BIT);
 	
 	/* l'arrière-plan en premier */
 	afficherArrierePlan(jeu->etat, jeu->ressources, jeu->image->dim);
  	switch( jeu->etat )
  	{
  		case nonLance :
  			afficherMenu(ressources, jeu->image->dim);
  			break;
  		case lance :
  			/* l'arrière-plan s'il existe */
			if( jeu->carte->possedeDecor )
			{
				afficherCarte(ressources->affichageDecor, jeu->image->dim);
			}
  			/* on afficher la cité, la chaîne de monstres */
		    afficherCite(jeu->cite, ressources->banqueAffichage, ressources->listeDim, jeu->image->dim);

		    afficherChaine(jeu->chaine, ressources->banqueAffichage, ressources->listeDim, jeu->image->dim);

		    /* le joueur, et les textes associés */
			afficherNiveau(jeu->niveau, jeu->image->dim);
		    afficherJoueur(jeu->joueur, jeu->etat, jeu->image->dim);
		    break;
		case enPause :
			/* on affiche l'aide */
			afficherNiveau(jeu->niveau, jeu->image->dim);
		    afficherJoueur(jeu->joueur, jeu->etat, jeu->image->dim);
			break;
		case fini :
			afficherFin(jeu);
			break;
  	}
  	/* on affiche l'état du jeu quoi qu'il arrive */
  	afficherEtatJeu(jeu->etat, jeu->image->dim);
}


void libererJeu(Jeu *jeu)
{
	/* ICI ON NE SE PRÉOCCUPE PAS DE RÉINITIALISER LES CIBLES
	* ON DONNE DONC NULL À libererChaine
	*/
	
	libererChaine(jeu->chaine, NULL);
	if( jeu->cite )
		libererCite(jeu->cite);

	/* */

	/* on libère tout */
	libererJoueur(jeu->joueur);
	libererCarte(jeu->carte);
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

int correspondanceEtatJeuArrierePlan(EtatJeu etat)
{
	int type;
	switch(etat)
	{
		case nonLance :
			type = ARR_menu;
			break;
		case enPause :
			type = ARR_aide;
			break;
		case fini :
			type = ARR_fin;
			break;
		case lance :
		default :
			/* on affiche alors rien */
			type = -1;
			break;

	}
	return type;
}