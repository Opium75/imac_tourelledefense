#ifndef __FONCTIONS_MONSTRE
#define __FONCTIONS_MONSTRE


#include <stdio.h>
#include <stdlib.h>
/* pour la génération pseudo-aléatoire */
#include <time.h>

#include "point.h"
#include "graphe.h"

typedef enum {M1, M2} TypeMonstre; 

#define NB_TYPES_MONSTRE 2
#define NB_TYPES_TOUR 4

static const char *TYPE_MONSTRE[] = {"M1", "M2"};



/*attributs unitaires*/
#define VIE_BASE 1
#define VITESSE_BASE 1
#define RESISTANCE_BASE 1
static const unsigned int VIE_TYPE[NB_TYPES_MONSTRE] = {1, 2};
static const unsigned int VITESSE_TYPE[NB_TYPES_MONSTRE] = {2, 1};
static const unsigned int RESISTANCES_TYPE[NB_TYPES_MONSTRE][NB_TYPES_TOUR] = { {4, 4, 4, 4},
																				 {2, 4, 1, 8} };
																				
typedef struct Monstre {
	unsigned int vieMax, vitesse;
	int vie;
	unsigned int  resistances[NB_TYPES_TOUR];
	TypeMonstre type;
	/*un montre se trouve dans un segment -> entre deux noeuds*/
	Noeud *depart, *arrivee;
	/* à un pourcentage d'avancement sur ce segment
	* c-à-d ses cordonnées sont déterminés par 
	l'origine (coord du noeud de départ)
	le coeff directeur, et ce pourcentage*/
	double avancement;
	/* grâce à un calcul du plus cours chemin,
	* on peut connaître les indices des noeuds à parcourir
	*(on pourra les mettre à jour à chaque ajout de tour ?)*/
	int *parcours;
	int nombreEtapes;
	int indiceEtape;
} Monstre;

Monstre* creerMonstre(TypeMonstre type, Noeud *depart);
void libererMonstre( Monstre *monstre);
void afficherMonstre(Monstre *monstre);

unsigned int calculerVie(TypeMonstre type);
unsigned int calculerVitesse(TypeMonstre type);
void calculerResistances(unsigned int resistances[], TypeMonstre type);

/*correspondance avec les coordonnées du monstre */
void calculerPositionMonstre(Monstre *monstre, Point *coord);
unsigned int valeurAffine(unsigned int a, unsigned int b, double avancement);

/* utilisées dans les Vagues */
TypeMonstre calculerTypeMonstre();
Noeud* calculerEntreeMonstre(Noeud **entrees, int nombreEntreesVague);

#endif