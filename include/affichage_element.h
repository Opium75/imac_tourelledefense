#ifndef __FONCTION_AFFICHAGE_ELEMENT
#define __FONCTION_AFFICHAGE_ELEMENT

#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "couleur.h"
#include "point.h"
#include "monstre.h"
#include "vague.h"
#include "tour.h"
#include "cite.h"

#include "bouton.h"
#include "rang.h"
#include "gestion_affichage.h"


void afficherCarte(GLuint idAffichage, Dimensions *dimImage);

void afficherBouton(TypeBouton type, GLuint boutonAffichage[], Dimensions *dimImage);

/* afficher à un pourcentage de la fenêtre */
void afficherImagePourcentage(GLuint idAffichage, Point *coordPourcentage, Dimensions *dimPourcentage, Dimensions *dimImage);

void afficherVague(Vague *vague, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherChaine(Chaine chaine, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage);

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherCite(Cite *cite, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);

void afficherMonstre(Monstre *monstre, GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherTour(Tour *tour,  GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherElement(GLuint idAffichage);

void afficherImageRang(int rang, GLuint idAffichage, Point *coordRang, Dimensions *dimRang, Dimensions *dimImage);

char *allouerTexte(int taille);
void libererTexte(char *texte);

/* le menu */
void afficherTitreMenu(Dimensions *dimImage);


void afficherNiveau(unsigned char niveau, Dimensions *dimImage);
void afficherPointage(int pointage, Dimensions *dimImage);
void afficherArgent(int argent, Dimensions *dimImage);
void afficherTouche(char toucheCode, Dimensions *dimImage);

/* afficher un texte relatif à l'état du jeu */
void afficherEtatJeu(int etatJeu, Dimensions *dimImage);

/* pour la fin, résultats */
void afficherTexteRang(int rang, Dimensions *dimImage);

/* les crédits */
void afficherCredits(Dimensions *dimImage);

void afficherTexte(char *texte, void *police, Point *origine, unsigned char couleurTexte[], Dimensions *dimImage);

void GLUT_afficherTexte(char *texte, void *police, Point *origine,  Dimensions *dimImage);

#endif