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

//#include "commun.h"

void afficherVague(Vague *vague, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherChaine(Chaine chaine, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage);

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);
void afficherCite(Cite *cite, GLuint banqueAffichage[],  Dimensions listeDim[], Dimensions *dimImage);

void afficherMonstre(Monstre *monstre, GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherTour(Tour *tour,  GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);
void afficherElement(GLuint idAffichage,  Dimensions *dimLutin, Dimensions *dimImage);

int afficherCarte(void);

void vBitmapOutput(int x, int y, char *string, void *font);
void vStrokeOutput(GLfloat x, GLfloat y, char *string, void *font);
void afficheTouche(char txtP[25], char touchecode);








#endif