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

#include "gestion_affichage.h"

#define MAX_TAILLE_TEXTE 50

static const Point POSITION_TEXTE_TOUCHE = {-95, 65};

static const char TEXTE_TOUCHE[] = "Touche";

//#include "commun.h"
void afficherCarte(GLuint idAffichage, Dimensions *dimImage);

void afficherVague(Vague *vague, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherChaine(Chaine chaine, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage);

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherCite(Cite *cite, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);

void afficherMonstre(Monstre *monstre, GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherTour(Tour *tour,  GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherElement(GLuint idAffichage, Dimensions *dimImage);


void afficherTouche(char toucheCode);
void afficherTexte(char texte[], Point *origine, Dimensions *dimImage);

void vBitmapOutput(char *chaine, Point *origine, void *police);
void vStrokeOutput(char *chaine, Point *origine, void *police, Dimensions *dimImage);




#endif