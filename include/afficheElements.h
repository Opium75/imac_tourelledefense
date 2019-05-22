#ifndef __AFFICHE_ELEMENTS
#define __AFFICHE_ELEMENTS

#include <GL/glu.h>
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
#include "afficheCarte.h"

#include "commun.h"

#define NB_LUTINS_MONSTRE 2
#define NB_LUTINS_TOUR 4
#define NB_LUTINS NB_LUTINS_MONSTRE + NB_LUTINS_TOUR

#define BASE_TAILLE_TOUR 50 /* EN PIXELS */
#define BASE_TAILLE_MONSTRE 40 /* IDEM */

static const unsigned char COULEUR_PARDEFAUT[NB_COULEURS] = {MAX_VAL_COULEUR, MAX_VAL_COULEUR, MAX_VAL_COULEUR};

static const unsigned char COULEUR_MONSTRE[NB_TYPES_MONSTRE][NB_COULEURS] = { {COULEUR_PARDEFAUT[0],COULEUR_PARDEFAUT[1],COULEUR_PARDEFAUT[2]}, {0, 169, 122} };


typedef struct {
	enum { LUT_tour, LUT_monstre } nature;
	union
	{
		TypeTour typeTour;
		TypeMonstre typeMonstre;
	};
} TypeLutin;

static const char *REP_LUTIN = "images/lutin/";
static const char *CHEMIN_IMAGE_TOUR = "tour/";
static const char *CHEMIN_IMAGE_MONSTRE = "monstre/";

/** ATTENTION devra correspondre à l'ordre des TypeTour **/
static const char *NOM_IMAGE_TOUR[] = { "tourR", "tourV", "tourB", "tourJ"};

static const char *NOM_IMAGE_MONSTRE[] = {"virus"};

static const char EXTENTION[] = ".png";



/* Espace fenêtre virtuelle */
static const float GL_VIEW_WIDTH = 200.;
static const float GL_VIEW_HEIGHT = 200.;






void afficherVague(Vague *vague, GLuint banqueAffichage[], GLuint banqueTextures[],  Dimensions *dimImage);
void afficherChaine(Chaine chaine, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);
void afficherCite(Cite *cite, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);

void afficherMonstre(Monstre *monstre, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);
void afficherTour(Tour *tour, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);
void afficherElement(TypeLutin *type, Point *coord, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);

void calculerCoordonneesVirtuelles(Point *coord, double *posX, double *posY, Dimensions *dimImage);
void calculerCoordonneesEchelle(Point *cood, int x, int y, Dimensions *dimImage);


/*** RESSOURCES D'AFFICHAGE ***/
/* remplir la liste d'affichage à partir des textures des lutins*/

void chargerRessourcesAffichage(SDL_Surface *lutins[], GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);
void libererRessourcesAffichage(SDL_Surface *lutins[],  GLuint banqueAffichage[], GLuint banqueTextures[]);

void remplirListeType(TypeLutin listeType[]);
void remplirListeDimensions(Dimensions listeDim[], TypeLutin listeType[]);
void remplirBanqueAffichage(GLuint banqueAffichage[], GLuint banqueTextures[], TypeLutin listeType[], Dimensions listeDim[],  Dimensions *dimImage);

/* on chargera  toutes les textures deau début du programme */
void chargerToutesTexturesLutins(SDL_Surface *lutins[], GLuint banqueTextures[]);
SDL_Surface* chargerTextureLutin(GLuint idTexture, TypeLutin *type);


void libererToutesTexturesLutins(GLuint banqueTextures[]);
void libererToutesImagesLutins(SDL_Surface *lutins[]);
/*** ***/

/* le dessin même à partir des textures */
void  dessinerLutinEchelle(GLuint idTexture, TypeLutin *type, Dimensions *dimLutin);
void dessinerLutin(GLuint idTexture, TypeLutin *type);


/* deux fonctions, liées par une relation de bijection réciproque */
TypeLutin correspondanceTypeLutin(int indice);
int correspondanceIndiceLutin(TypeLutin *type);



void correspondanceCheminLutin(char *cheminLutin, TypeLutin *type);

void matrixAffich(GLuint idElements, int posX, int posY);
int afficheElements(void);

#endif