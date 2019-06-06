#ifndef FONCTIONS__BOUTON
#define FONCTIONS__BOUTON

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "point.h"
#include "couleur.h"

#define NB_BOUTONS 2

typedef enum {BOUT_menu, BOUT_redem} TypeBouton;

#define MAX_TAILLE_TEXTE 50

/* position des textes en pourcentage de l'écran, à partir du coin haut-gauche */
static const Point POSITION_TEXTE_NIVEAU = {2, 20};
static const Point POSITION_TEXTE_POINTAGE = {2, 25};
static const Point POSITION_TEXTE_ARGENT = {2, 30};
static const Point POSITION_TEXTE_TOUCHE = {2, 35}; /* POURCENTAGE DE LA DIMENSIONS DE L'IMAGE */

/* ETAT DU JEU */
/* affichage de l'état du jeu */
static const char *TEXTES_ETAT_JEU[] = {"MENU", "JEU", "PAUSE", "FIN"};
static const Point POSITION_TEXTE_ETAT_JEU = {70, 95};
static const unsigned char COULEUR_TEXTE_ETAT_JEU[NB_COULEURS] = {200, 255, 255};



/* couleur des textes */
static const unsigned char COULEUR_TEXTE_NIVEAU[NB_COULEURS] = {255, 0, 0};
static const unsigned char COULEUR_TEXTE_POINTAGE[NB_COULEURS] = {255, 0, 255};
static const unsigned char COULEUR_TEXTE_ARGENT[NB_COULEURS] = {255, 255, 0};
static const unsigned char COULEUR_TEXTE_TOUCHE[NB_COULEURS] = {0, 255, 255};

static const char TEXTE_NIVEAU[] = "Niveau : ";
static const char TEXTE_POINTAGE[] = "Pointage : ";
static const char TEXTE_ARGENT[] = "Argent : ";
static const char TEXTE_TOUCHE[] = "Touche : ";


/*** BOUTONS ***/

/** MENU **/
//static const Point POSITION_BOUTON_MENU = {50, 70};
/* en pourcentage de l'écran */
//static const Dimensions DIM_BOUTON_MENU = {40, 20};
/* */

/** RECOMMENCER **/
//static const Point POSITION_BOUTON_RECOM = {50, 70};
//static const Dimensions DIM_BOUTON_RECOM = {20, 10};

/** TOTAL **/
static const Point POSITION_BOUTONS[NB_BOUTONS] = { {50, 70}, 
													{50, 70}
													};

static const Dimensions DIM_BOUTONS[NB_BOUTONS] = { {40, 20},
													{20, 10}
													};


/** MENU **/
static const char TEXTE_TITRE_MENU[] = {"IMAC TOURELLE DEFENSE"};
static const Point POSITION_TEXTE_TITRE_MENU = {25, 95};
static const unsigned char COULEUR_TEXTE_TITRE_MENU[NB_COULEURS] = {0, 255, 255};



/* POUR LE CLIC */
static const Dimensions DIM_TEXTE_MENU = {20, 10};
static const unsigned char COULEUR_TEXTE_MENU[NB_COULEURS] = {127, 0, 255};

/*** FIN ***/
/** RANG **/
static const Point POSITION_TEXTE_RANG = {61, 83}; /* POURCENTAGE DE LA DIMENSIONS DE L'IMAGE, à l'écran */

static const Point POSITION_IMAGE_RANG = {90, 70};
static const Dimensions DIM_IMAGE_RANG = {15, 15};
static const unsigned char COULEUR_TEXTE_RANG[NB_COULEURS] = {255, 127, 127};



/* crédits */
#define NB_CREDITS 2
#define DECALAGE_TEXTE_CREDITS 5
static const char *TEXTE_CREDITS[] = {"Flora MALLET", "Pierre THIEL"};
static const Point POSITION_CREDITS = {2, 75};
static const unsigned char COULEUR_TEXTE_CREDITS[NB_COULEURS] = {255, 127, 0};


/* on utilisera certainement cette structure plus tard */
/* on s'en passera pour l'instant */
typedef struct {
	Point *coord;
	Dimensions *dim;
} Bouton;



// void drawSquare(int filled);
// void isItButton(SDL_Event e, Dimensions *dimImage);

bool texteEstClique(Point *coordTexte, Dimensions *dimTexte, Point *coordClique, Dimensions *dimImage);

bool boutonEstClique(Point *coordBouton, Dimensions *dimBouton, Point *coordClique, Dimensions *dimImage);
bool boutonEstSurvole(Point *coordBouton, Dimensions *dimBouton, Point *coordSouris, Dimensions *dimImage);

#endif