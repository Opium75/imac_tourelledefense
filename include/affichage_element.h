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

#include "rang.h"
#include "gestion_affichage.h"

#define MAX_TAILLE_TEXTE 50

/* position des textes en pourcentage de l'écran, à partir du coin haut-gauche */
static const Point POSITION_TEXTE_NIVEAU = {0, 10};
static const Point POSITION_TEXTE_POINTAGE = {0, 15};
static const Point POSITION_TEXTE_ARGENT = {0, 20};
static const Point POSITION_TEXTE_TOUCHE = {0, 25}; /* POURCENTAGE DE LA DIMENSIONS DE L'IMAGE */

static const Point POSITION_TEXTE_RANG = {0, 30}; /* POURCENTAGE DE LA DIMENSIONS DE L'IMAGE */
static const Point POSITION_IMAGE_RANG = {50, 30};

static const Dimensions DIM_IMAGE_RANG = {200, 200};

/* couleur des textes */
static const unsigned char COULEUR_TEXTE_NIVEAU[NB_COULEURS] = {255, 0, 0};
static const unsigned char COULEUR_TEXTE_POINTAGE[NB_COULEURS] = {255, 0, 255};
static const unsigned char COULEUR_TEXTE_ARGENT[NB_COULEURS] = {255, 255, 0};
static const unsigned char COULEUR_TEXTE_TOUCHE[NB_COULEURS] = {0, 255, 255};

static const unsigned char COULEUR_TEXTE_RANG[NB_COULEURS] = {0, 255, 0};

static const char TEXTE_NIVEAU[] = "Niveau : ";
static const char TEXTE_POINTAGE[] = "Pointage : ";
static const char TEXTE_ARGENT[] = "Argent : ";
static const char TEXTE_TOUCHE[] = "Touche : ";

static const char TEXTE_RANG[] = "Rang :";


//#include "commun.h"
void afficherCarte(GLuint idAffichage, Dimensions *dimImage);

void afficherVague(Vague *vague, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherChaine(Chaine chaine, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage);

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherCite(Cite *cite, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);

void afficherMonstre(Monstre *monstre, GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherTour(Tour *tour,  GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherElement(GLuint idAffichage, Dimensions *dimImage);

void afficherImageRang(int rang, GLuint idAffichage, Point *coordRang, Dimensions *dimRang, Dimensions *dimImage);

char *allouerTexte(int taille);
void libererTexte(char *texte);

void afficherNiveau(unsigned char niveau, Dimensions *dimImage);
void afficherPointage(int pointage, Dimensions *dimImage);
void afficherArgent(int argent, Dimensions *dimImage);
void afficherTouche(char toucheCode, Dimensions *dimImage);

/* pour la fin, résultats */
void afficherRang(int rang, Dimensions *dimImage);

void afficherTexte(char *texte, Point *origine, unsigned char couleurTexte[], Dimensions *dimImage);


void GLUT_afficherTexte(char *texte, Point *origine, void *police, Dimensions *dimImage);
void GLUT_afficherTexte2(char *texte, Point *origine, void *police, Dimensions *dimImage);


#endif