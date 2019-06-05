#ifndef __FONCTIONS_JEU

#define __FONCTIONS_JEU

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./PPM_image/PPM_image.h"
#include "carte.h"
#include "lecture_fichier_carte.h"
#include "verif_carte.h"
#include "tour.h"
#include "cite.h"
#include "vague.h"
#include "gestion_affichage.h"
#include "affichage_element.h"
#include "bouton.h"
#include "point.h"
#include "jeu.h"

#include "rang.h"
/* l'argent dont dispose le joueur en début de partie */

#define ARGENT_DEPART 50

typedef struct {
	int pointage, argent;
	char memTouche;
} Joueur;

/* Mega-structure qui comprend toutes les autres */
typedef struct {
	unsigned char niveau;
	Joueur *joueur;
	Carte *carte;
	Cite *cite;
	Vague *chaine;
	PPM_Image *image;
	/** pour l'affichage **/
	SDL_Surface *scene;
	Ressources *ressources;
} Jeu;



static const char TOUCHES_TOUR[] = {'r', 'v', 'b', 'j'};


/** OPENGL **/
void afficherJoueur(Joueur *joueur, Dimensions *dimImage);
void afficherJeu(Jeu *jeu);
/** **/

void boucleJeu(Jeu *jeu);

Jeu* allouerJeu(void);

/* LECTURE DES FICHIERS ETC AVANT JEU */
bool preparerJeu(Jeu *jeu, char *nomDonnees);

void lancerJeu(Jeu *jeu);
void relancerJeu(Jeu *jeu);
void quitterJeu(Jeu *jeu);

/** JOUEUR **/
Joueur* allouerJoueur(void);
void libererJoueur(Joueur *joueur);

int calculerRang(Joueur *joueur);
/** **/

/** SDL **/
void boucleJeu(Jeu *jeu);
bool interfaceJeu(Jeu *jeu);
/**/
void gestionClic(Jeu *jeu, SDL_Event *e);
void gestionConstruction(Joueur *joueur, Cite *cite, Carte *carte, Point *coordClique);

int toucheVersTypeTour(char touche);
void gestionTouche(Jeu *jeu, SDL_Event *e);

/** **/
/* renvoie vrai si on a atteint la fin (le joueur est vaincu), faux sinon */
bool traitementJeu(Jeu* jeu, time_t deltaT);
bool traitementJoueur(Joueur *joueur, int gainPoints, int gainArgent, int pertePoints, int perteArgent);

/*** FIN DU JEU ***/
void afficherJeuFin(Jeu *jeu);
void afficherJoueurFin(Joueur *joueur, Dimensions *dimImage);
bool interfaceJeuFin(Jeu *jeu);

void libererJeu(Jeu *jeu);

void debug(void);

#endif