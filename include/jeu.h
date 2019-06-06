#ifndef __FONCTIONS_JEU

#define __FONCTIONS_JEU

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"

#include "point.h"
#include "./PPM_image/PPM_image.h"
#include "carte.h"
#include "lecture_fichier_carte.h"
#include "verif_carte.h"
#include "cite.h"
#include "vague.h"
#include "gestion_affichage.h"
#include "affichage_element.h"

#include "bouton.h"
#include "rang.h"

/* l'argent dont dispose le joueur en début de partie */
#define ARGENT_DEPART 50


typedef struct {
	int pointage, argent;
	char memTouche;
} Joueur;

typedef enum {nonLance, lance, enPause, fini} EtatJeu;

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
	EtatJeu etat;
} Jeu;



static const char TOUCHES_TOUR[] = {'r', 'v', 'b', 'j'};

static const char TOUCHE_PAUSE = 'p';



/** OPENGL **/
void afficherJeu(Jeu *jeu);
void afficherJoueur(Joueur *joueur, EtatJeu etat, Dimensions *dimImage);

void afficherMenu(Ressources *ressources, Dimensions *dimImage);
void afficherFin(Jeu *jeu);
/** **/

void boucleJeu(Jeu *jeu);

Jeu* allouerJeu(void);

/* LECTURE DES FICHIERS ETC AVANT JEU */
bool preparerJeu(Jeu *jeu, char *nomDonnees);

void chargerJeu(Jeu *jeu);
void lancerJeu(Jeu *jeu);
void finirJeu(Jeu *jeu);
void recommencerJeu(Jeu *jeu);
void quitterJeu(Jeu *jeu);

/** JOUEUR **/
Joueur* allouerJoueur(void);
void reinitialiserJoueur(Joueur *joueur);
void libererJoueur(Joueur *joueur);

int calculerRang(Joueur *joueur);
/** **/

void afficherArrierePlan(EtatJeu etat, Ressources *ressources, Dimensions *dimImage);
/* pour savoir quel arrière-plan afficher */
int correspondanceEtatJeuArrierePlan(EtatJeu etat);

/** SDL **/
/**/

int toucheVersTypeTour(char touche);

void gestionClic(Jeu *jeu, SDL_Event *e);
void gestionConstruction(Joueur *joueur, Cite *cite, Carte *carte, Point *coordClique);
void gestionTouche(Jeu *jeu, SDL_Event *e);

void gestionMenu(Jeu *jeu, Point *coordClique);
void gestionFin(Jeu *jeu, Point *coordClique);

/** TRAITEMENT **/
void traitementJeu(Jeu* jeu, time_t deltaT);

/* renvoie vrai si on a atteint la fin (le joueur est vaincu), faux sinon */
bool traitementJoueur(Joueur *joueur, int gainPoints, int gainArgent, int pertePoints, int perteArgent);

void boucleJeu(Jeu *jeu);
/* renvoie vrai tant que l'on a pas quitté le jeu */
bool interfaceJeu(Jeu *jeu);

void libererJeu(Jeu *jeu);



void debug(void);

#endif