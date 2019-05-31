#ifndef __FONCTIONS_JEU

#define __FONCTIONS_JEU

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glu.h>
#include <SDL/SDL.h>

#include "./PPM_image/PPM_image.h"
#include "carte.h"
#include "lecture_fichier_carte.h"
#include "verif_carte.h"
#include "tour.h"
#include "cite.h"
#include "vague.h"
#include "gestion_affichage.h"
#include "affichage_element.h"


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
	SDL_Surface *lutins[NB_LUTINS];
	Dimensions listeDim[NB_LUTINS];
	SDL_Surface *arrierePlan;
	GLuint banqueTextures[NB_RESSOURCES];
	GLuint banqueAffichage[NB_RESSOURCES];
	SDL_Surface *scene;
} Jeu;


/** OPENGL **/
void afficherJeu(Jeu *jeu);
/** **/

void boucleJeu(Jeu *jeu);

Jeu* creerJeu(unsigned char niveau, Joueur *joueur, Carte *carte, Cite *cite, Vague *chaine);
Jeu* allouerJeu(void);

/* LECTURE DES FICHIERS ETC AVANT JEU */
bool preparerJeu(Jeu *jeu, char *nomDonnees);

void lancerJeu(Jeu *jeu);
void relancerJeu(Jeu *jeu);
void quitterJeu(Jeu *jeu);

/** JOUEUR **/
Joueur* allouerJoueur(void);
void libererJoueur(Joueur *joueur);
/**

void lancerJeu(Jeu *jeu);
void quitterJeu(Jeu *jeu);

/** SDL **/
void boucleJeu(Jeu *jeu);
bool interfaceJeu(Jeu *jeu);
/**/
void gestionClic(Jeu *jeu, SDL_Event *e);
void gestionTouche(Jeu *jeu, SDL_Event *e);
/** **/
void traitementJeu(Jeu* jeu, time_t deltaT);

void libererJeu(Jeu *jeu);

#endif