#ifndef __FONCTIONS_GESTION_AFFICHAGE
#define __FONCTIONS_GESTION_AFFICHAGE

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "couleur.h"
#include "point.h"
#include "monstre.h"
#include "tour.h"
#include "rang.h"

/* Dimensions initiales et titre de la fenetre */
static const unsigned int LARGEUR_FENETRE = 800;
static const unsigned int HAUTEUR_FENETRE = 600;
static const char TITRE_FENETRE[] = "TOURELLE DÉFENSE !!";


/* Espace fenêtre virtuelle */
static const float GL_VUE_LARGEUR = 200.;
static const float GL_VUE_HAUTEUR = 200.;


/* Nombre de bits par pixel de la fenetre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

#define NB_ARRIERE_PLAN 1
#define NB_LUTINS_MONSTRE 2
#define NB_LUTINS_TOUR 4
#define NB_LUTINS NB_LUTINS_MONSTRE + NB_LUTINS_TOUR
/* TOTAL */


typedef struct {
	/* menu de démarrage */
	SDL_Surface *menu;
	GLuint textureMenu;
	GLuint affichageMenu;
	/* arrière-plan */
	SDL_Surface *arrierePlan;
	GLuint textureArrierePlan;
	GLuint affichageArrierePlan;
	/* lutin */
	SDL_Surface *lutins[NB_LUTINS];
	Dimensions listeDim[NB_LUTINS];
	GLuint banqueTextures[NB_LUTINS];
	GLuint banqueAffichage[NB_LUTINS];
	/* rang */
	SDL_Surface *rangs[NB_RANGS];
	GLuint rangTextures[NB_RANGS];
	GLuint rangAffichage[NB_RANGS];
	/* aide */
	SDL_Surface *aide;
	GLuint textureAide;
	GLuint affichageAide;
} Ressources;



#define BASE_TAILLE_TOUR 30 /* EN PIXELS */
#define BASE_TAILLE_MONSTRE 30 /* IDEM */

//static const unsigned char MAX_VAL_COULEUR[NB_COULEURS] = {MAX_VAL_COULEUR, MAX_VAL_COULEUR, MAX_VAL_COULEUR};
//static const unsigned char COULEUR_MONSTRE[NB_TYPES_MONSTRE][NB_COULEURS] = { {MAX_VAL_COULEUR, MAX_VAL_COULEUR, MAX_VAL_COULEUR}, {0, 169, 122} };
static const unsigned char COULEUR_PARDEFAUT[NB_COULEURS] = {MAX_VAL_COULEUR, MAX_VAL_COULEUR, MAX_VAL_COULEUR};

static const unsigned char COULEUR_MONSTRE[NB_TYPES_MONSTRE][NB_COULEURS] = { {MAX_VAL_COULEUR, MAX_VAL_COULEUR, MAX_VAL_COULEUR}, {40, 239, 222} };

static const unsigned char COULEUR_TRAIT[NB_TYPES_TOUR][NB_COULEURS] = { 
															{MAX_VAL_COULEUR, 0, 0}, 
															{0, MAX_VAL_COULEUR, 0}, 
															{0, 0, MAX_VAL_COULEUR}, 
															{MAX_VAL_COULEUR, MAX_VAL_COULEUR, 0} 
														};

typedef struct {
	enum { LUT_tour, LUT_monstre } nature;
	union
	{
		TypeTour typeTour;
		TypeMonstre typeMonstre;
	};
} TypeLutin;

static const char REP_ARRIEREPLAN_CARTE[] = "images/arriere_plan";

static const char REP_RANG[] = "images/rang/";

static const char *REP_LUTIN = "images/lutin/";
static const char *CHEMIN_IMAGE_TOUR = "tour/";
static const char *CHEMIN_IMAGE_MONSTRE = "monstre/";



/** ATTENTION devra correspondre à l'ordre des TypeTour **/
static const char *NOM_IMAGE_TOUR[] = { "tourR", "tourV", "tourB", "tourJ"};

static const char *NOM_IMAGE_MONSTRE[] = {"virus"};

static const char NOM_ARRIEREPLAN_AIDE[] = "aide.png";

static const char NOM_IMAGE_MENU[] = "imac.png";


static const char *NOM_IMAGE_RANG[] = {"norton", "bitdefender", "avast", "avg", "mcafee", "avira", "kaspersky", "clamav"};

static const char EXTENSION[] = ".png";

Ressources* allouerRessources();
void libererRessources(Ressources *ressources);

void lancerAffichage(SDL_Surface **scene);
void fermerAffichage(SDL_Surface *scene);

void calculerCoordonneesVirtuelles(Point *coord, double *posX, double *posY, Dimensions *dimImage);
void calculerCoordonneesEchelle(Point *cood, int x, int y, Dimensions *dimImage);
/* par rapport aux dimensions de la fenêtre */
void calculerDimensionsEchelle(double *propX, double *propY, Dimensions *dimElement);
/* par rapport au dimensions de l'image spécifiée */
void calculerDimensionsEchelleImage(double *propX, double *propY, Dimensions *dimElement, Dimensions *dimImage);

void GL_changerCouleurTrait(const unsigned char couleur[]);


void dessinerSegment(double x1, double y1, double x2, double y2, unsigned char couleur[NB_COULEURS]);
void calculerCouleurTir(unsigned char couleurTir[], Tour *tour);

/*** RESSOURCES D'AFFICHAGE ***/
/* remplir la liste d'affichage à partir des textures des lutins*/

void chargerRessourcesAffichage(Ressources *ressources, Dimensions *dimImage, bool possedeArrierePlan, char nomArrierePlan[]);

void remplirListeType(TypeLutin listeType[]);
void remplirListeDimensions(Dimensions listeDim[], TypeLutin listeType[]);

/* on chargera  toutes les textures deau début du programme */

/** Les deux fonctions principales **/
void chargerTexture( SDL_Surface **image,  GLuint *idTexture, char *chemin);
void chargerAffichage(GLuint *idAffichage, GLuint idTexture);
/** **/

void chargerTextureLutins(SDL_Surface *lutins[], GLuint banqueTextures[]);
SDL_Surface* chargerTextureLutin(GLuint *idTexture, TypeLutin *type);
void chargerAffichageLutins(GLuint banqueAffichage[], GLuint banqueTextures[]);


void chargerTextureArrierePlan(SDL_Surface **arrierePlan, GLuint *idTexture, char nomArrierePlan[]);
void  chargerAffichageArrierePlan(GLuint *idAffichage, GLuint idTexture, Dimensions *dimImage);

void chargerTextureRangs(SDL_Surface *rangs[], GLuint rangTextures[]);
SDL_Surface* chargerTextureRang(GLuint *idTexture, int indice);
void chargerAffichageRangs(GLuint rangAffichage[], GLuint rangTextures[]);



void libererRessourcesAffichage(Ressources *ressources);
/*** ***/

/* le dessin même à partir des textures */
/* ON DESSINE L'IMAGE CENTRÉE À L'ORIGINE */
void dessinerTexture(GLuint idTexture);
/* */

/** correspondances **/
/* deux fonctions, liées par une relation de bijection réciproque */
TypeLutin correspondanceTypeLutin(int indice);
int correspondanceIndiceLutin(TypeLutin *type);
void correspondanceCheminLutin(char *cheminLutin, TypeLutin *type);

/* pour les rangs */
void correspondanceCheminRang(char *cheminRang, int indice);

void redimensionner(SDL_Surface** surface, unsigned int largeur, unsigned int hauteur);


#endif