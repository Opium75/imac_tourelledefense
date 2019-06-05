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
#include "commun.h"
#include "jeu.h"

void drawSquare(int filled);
void isItButton(SDL_Event e, Dimensions *dimImage);

#endif