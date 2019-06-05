#ifndef __FONCTIONS_MONSTRE
#define __FONCTIONS_MONSTRE


#include <stdio.h>
#include <stdlib.h>
/* pour la génération pseudo-aléatoire */
#include <time.h>

#include "couleur.h"
#include "point.h"
#include "graphe.h"

typedef enum {M1, M2} TypeMonstre;

typedef enum {estVaincu, estSorti, enAttente, enMouvement} EtatMonstre;

#define NB_TYPES_MONSTRE 2
#define NB_ETATS_MONSTRE 4

#define NB_TYPES_TOUR 4

static const char *TYPE_MONSTRE[] = {"M1", "M2"};

static const char *ETAT_MONSTRE[] = {"Vaincu", "Sorti", "En attente", "En mouvement"};



/*attributs unitaires*/
#define VIE_BASE 3
#define VITESSE_BASE 5000 /* en pixels par seconde ? */
#define RESISTANCE_BASE 1
#define ATTAQUE_BASE 1
static const unsigned int VIE_TYPE[NB_TYPES_MONSTRE] = {1, 2};
static const unsigned int VITESSE_TYPE[NB_TYPES_MONSTRE] = {2, 1};
static const unsigned int RESISTANCES_TYPE[NB_TYPES_MONSTRE][NB_TYPES_TOUR] = { {4, 4, 4, 4},
																				 {2, 4, 1, 8} };
static const unsigned int ATTAQUE_TYPE[NB_TYPES_TOUR] = {1, 2};


typedef struct Monstre {
	unsigned int vieMax;
	/*
	* plutôt unité de longueur  PAR un temps (en tours de processeur)
	*/
	int vitesse;
	int vie, attaque;
	unsigned int  resistances[NB_TYPES_TOUR];
	TypeMonstre type;
	/* rajouté pour connaître son état à un instant donné */
	EtatMonstre etat;
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
	/* cella là c'est pour savoir
	* si l'on doit recalculer le chemin à une intersection.
	*/
	int indiceEntree;
	/* il est utile dans le calcul du plus cours chemin
	* de savoir par quelle entrée de la carte le monstre est arrivé
	*/
	unsigned char modif_INDIC_mem;
} Monstre;

Monstre* creerMonstre(TypeMonstre type, unsigned char niveau, Noeud *depart);
void libererMonstre( Monstre *monstre);
void terminalMonstre(Monstre *monstre);

unsigned int calculerVie(TypeMonstre type, unsigned char niveau);
int calculerVitesse(TypeMonstre type, unsigned char niveau);
int calculerAttaque(TypeMonstre type, unsigned char niveau);
void calculerResistances(unsigned int resistances[], TypeMonstre type);

void attaquerJoueur(Monstre *monstre, int *pointage, int *argent);

/*correspondance avec les coordonnées du monstre */
void calculerPositionMonstre(Monstre *monstre, Point *coord);

/* utilisées dans les Vagues */
TypeMonstre calculerTypeMonstre(void);
Noeud* calculerEntreeMonstre(Noeud **entrees, int nombreEntreesVague);

#endif