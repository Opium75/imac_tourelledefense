#ifndef __BOUTON
#define __BOUTON

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "point.h"
#include "gestion_affichage.h"
#include "couleur.h"
#include "jeu.h"

/* on utilisera certainement cette structure plus tard */
/* on s'en passera pour l'instant */
typedef struct {
	Point *coord;
	Dimensions *dim;
} Bouton;

void drawSquare(int filled);
void isItButton(SDL_Event e, Dimensions *dimImage);

bool texteEstClique(Point *coordTexte, Dimensions *dimTexte, Point *coordClique, Dimensions *dimImage);

bool boutonEstClique(Point *coordBouton, Dimensions *dimBouton, Point *coordClique, Dimensions *dimImage);
bool boutonEstSurvole(Point *coordBouton, Dimensions *dimBouton, Point *coordSouris, Dimensions *dimImage);

#endif