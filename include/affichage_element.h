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

/* ETAT DU JEU */
static const Point POSITION_TEXTE_ETAT_JEU = {80, 90};



/* couleur des textes */
static const unsigned char COULEUR_TEXTE_NIVEAU[NB_COULEURS] = {255, 0, 0};
static const unsigned char COULEUR_TEXTE_POINTAGE[NB_COULEURS] = {255, 0, 255};
static const unsigned char COULEUR_TEXTE_ARGENT[NB_COULEURS] = {255, 255, 0};
static const unsigned char COULEUR_TEXTE_TOUCHE[NB_COULEURS] = {0, 255, 255};

static const unsigned char COULEUR_TEXTE_ETAT_JEU[NB_COULEURS] = {200, 255, 255};

static const char TEXTE_NIVEAU[] = "Niveau : ";
static const char TEXTE_POINTAGE[] = "Pointage : ";
static const char TEXTE_ARGENT[] = "Argent : ";
static const char TEXTE_TOUCHE[] = "Touche : ";


/* affichage de l'état du jeu */
static const char *TEXTES_ETAT_JEU[] = {"MENU", "JEU", "PAUSE", "FIN"};

/** MENU **/
static const char TEXTE_TITRE_MENU[] = {"IMAC TOURELLE D&FENSE"};
static const Point POSITION_TEXTE_TITRE_MENU = {90, 80};
static const unsigned char COULEUR_TEXTE_TITRE_MENU[NB_COULEURS] = {0, 255, 255};

static const Point POSITION_IMAGE_MENU = {70, 80};
/* en pourcentage de l'écran */
static const Dimensions DIM_IMAGE_MENU = {20, 20};

static const char TEXTE_BOUTON_MENU[] = {"C'EST PARTI"};
static const Point POSITION_TEXTE_BOUTON_MENU = {45, 75};
/* POUR LE CLIC */
static const Dimensions DIM_TEXTE_BOUTON_MENU = {20, 10};
static const unsigned char COULEUR_TEXTE_BOUTON_MENU[NB_COULEURS] = {127, 0, 255};

/*** FIN ***/
/** RANG **/
static const Point POSITION_TEXTE_RANG = {80, 80}; /* POURCENTAGE DE LA DIMENSIONS DE L'IMAGE, à l'écran */
static const Point POSITION_IMAGE_RANG = {50, 30};
static const Dimensions DIM_IMAGE_RANG = {30, 30};
static const unsigned char COULEUR_TEXTE_RANG[NB_COULEURS] = {0, 255, 0};
/** RECOMMENCER **/
static const Point POSITION_BOUTON_RECOM = {80, 80};
static const Dimensions DIM_BOUTON_RECOM = {30, 30};


/* crédits */
#define NB_CREDITS 2
#define DECALAGE_TEXTE_CREDITS 5
static const char *TEXTE_CREDITS[] = {"Flora MALLET", "Pierre THIEL"};
static const Point POSITION_CREDITS = {5, 80};
static const unsigned char COULEUR_TEXTE_CREDITS[NB_COULEURS] = {255, 127, 0};


//#include "commun.h"
void afficherCarte(GLuint idAffichage, Dimensions *dimImage);
void afficherAide(GLuint idAffichage, Dimensions *dimImage);

void afficherImageMenu(GLuint idAffichage, Point *coordMenu, Dimensions *dimMenu, Dimensions *dimImage);

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
void afficherTexteBoutonMenu(Dimensions *dimImage);


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

void afficherTexte(char *texte, Point *origine, unsigned char couleurTexte[], Dimensions *dimImage);


void GLUT_afficherTexte(char *texte, Point *origine, void *police, Dimensions *dimImage);

#endif