#ifndef __FONCTIONS_TOUR

#define __FONCTIONS_TOUR

#include <stdio.h>
#include <stdlib.h>

#include "point.h"

typedef enum {T_rouge, T_verte, T_bleue, T_jaune} TypeTour;

#define NB_TYPES_TOUR 4

/*attibuts unitaires*/
#define PUISSANCE_BASE 1 /*relatif aux points de vie et résistance des monstres.*/
#define CADENCE_BASE 1 /*en dixième de secondes*/
#define PORTEE_BASE 1 /*en pixels*/
#define COUT_BASE 1 /*PIÈCES*/
static const unsigned int PUISSANCE_TYPE[NB_TYPES_TOUR] = {4, 1, 2, 2};
static const unsigned int PORTEE_TYPE[NB_TYPES_TOUR] = {2, 1, 8, 2};
static const unsigned int CADENCE_TYPE[NB_TYPES_TOUR] = {1, 8, 2, 2};
static const unsigned int COUT_TYPE[NB_TYPES_TOUR] = {2, 2, 2, 2};


typedef struct {
	unsigned int puissance, cadence, portee, coutAchat;
	Point *coord;
	TypeTour type;
	/*Ici, c'est tous les champs que t'as mis, sans les fonctions
	c-à-d énergie, puissance, etc.
	Mais, en plus :*/
} Tour;



Tour* creerTour(TypeTour type, unsigned int x, unsigned int y);
void libererTour(Tour *tour);

unsigned int calculerPuissance(TypeTour type);
unsigned int calculerCadence(TypeTour type);
unsigned int calculerPortee(TypeTour type);
unsigned int calculerCout(TypeTour type);

#endif