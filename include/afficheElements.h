#ifndef __AFFICHE_ELEMENTS
#define __AFFICHE_ELEMENTS

#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>


/* Espace fenetre virtuelle */
static const float GL_VIEW_WIDTH = 200.;
static const float GL_VIEW_HEIGHT = 200.;

void matrixAffich(GLuint idElements, int posX, int posY);
int afficheElements(void);

#endif