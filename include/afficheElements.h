#ifndef __AFFICHE_ELEMENTS
#define __AFFICHE_ELEMENTS

#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>


/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 600;
static const unsigned int WINDOW_HEIGHT = 600;

/* Espace fenetre virtuelle */
static const float GL_VIEW_WIDTH = 200.;
static const float GL_VIEW_HEIGHT = 200.;

/* Nombre de bits par pixel de la fenetre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void matrixAffich(GLuint idElements, int posX, int posY);
int afficheElements(void);

#endif