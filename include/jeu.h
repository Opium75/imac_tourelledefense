#ifndef __FONCTIONS_JEU

#define __FONCTIONS_JEU

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glu.h>
#include <SDL/SDL.h>

#include "./PPM_image/PPM_image.h"
#include "carte.h"
#include "tour.h"
#include "cite.h"
#include "vague.h"
#include "gestion_affichage.h"
#include "affichage_element.h"


typedef struct {
	int pointage, argent;
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
	GLuint banqueTextures[NB_LUTINS];
	GLuint banqueAffichage[NB_LUTINS];
	SDL_Surface *scene;
} Jeu;


/** OPENGL **/
void afficherJeu(Jeu *jeu);
/** **/

void boucleJeu(Jeu *jeu);

Jeu* creerJeu(unsigned char niveau, Joueur *joueur, Carte *carte, Cite *cite, Vague *chaine);
Jeu* allouerJeu(void);

void relancerJeu(Jeu *jeu);

/** JOUEUR **/
Joueur* allouerJoueur(void);
void libererJoueur(Joueur *joueur);
/**

void lancerJeu(Jeu *jeu);
void quitterJeu(Jeu *jeu);

/** SDL **/
void boucleJeu(Jeu *jeu);
bool interfaceJeu(Jeu *jeu);
void gestionClic(Jeu *jeu, SDL_Event *e);
/** **/
void traitementJeu(Jeu* jeu, time_t deltaT);

void libererJeu(Jeu *jeu);

#endif