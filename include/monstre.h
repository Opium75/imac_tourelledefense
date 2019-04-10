#ifndef __FONCTIONS_MONSTRE

#define __FONCTIONS_MONSTRE

#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"
#include "tour.h"

#define NB_TYPES_MONSTRE

/*attributs unitaires*/
#define VIE_BASE 1
#define VITESSE_BASE 1
#define RESISTANCE_BASE 1
static const unsigned int VIE_TYPE[NB_TYPES_MONSTRE] = {1, 2};
static const unsigned int VITESSE_TYPE[NB_TYPES_MONSTRE] = {2, 1};
static const unsigned int RESISTANCES_TYPE[NB_TYPES_MONSTRE][NB_TYPES_TOUR] = { {4, 4, 4, 4},
																				 {2, 4, 1, 8}
																				};

typedef enum{M1, M2} TypeMonstre;

typedef struct {
	unsigned int ptVie, vitesse;
	unsigned int  resistances[NB_TYPES_TOUR];
	TypeMonstre type;
	/*un montre se trouve dans un segment -> entre deux noeuds*/
	Noeud *depart, *arrivee;
	/* à un pourcentage d'avancement sur ce segment
	* c-à-d ses cordonnées sont déterminés par 
	l'origine (coord du noeud de départ)
	le coeff directeur, et ce pourcentage*/
	double avancement;
} Monstre;

Monstre* creerMonstre(TypeMonstre type, Noeud *depart, Noeud *arrivee);
void libererMonstre( Monstre *monstre);

unsigned int calculerVie(TypeMonstre type);
unsigned int calculerVitesse(TypeMonstre type);
void calculerResistances(unsigned int resistances[], TypeMonstre type);


#endif