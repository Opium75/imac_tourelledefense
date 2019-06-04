#ifndef __FONCTIONS_CITE
#define __FONCTIONS_CITE

#include <stdio.h>
#include <stdlib.h>

#include "tour.h"


/* Bon alors, cette structure est un peu étrange.
* Je l'ai rajoutée pour avoir accès un nombre modif_INDIC
* qui s'incrémentera à chaque ajout ou suppression d'une tour.
*
* De telle sorte que si un monstre à enregistré modif_INDIC
* la dernière fois qu'il a choisi son chemin
* s'il trouve la même valeur la prochaine fois, il ne le recalcule pas.
* À condition qu'il n'y ait pas eu exactement 255 modifications,
* le nombre sera différent. (On aurait aussi pu mettre un entier non algébrique).
*/
/* On pourra ensuite rajouter les bâtiments à cette structure */

typedef struct {
	ListeTour listeTour;
	/* à rajouter :  tous les bâtiments */
	unsigned char modif_INDIC;
} Cite;

void traitementCite(Cite *cite, clock_t deltaT, Carte *carte, Monstre **monstres, int nombreMonstres, int *gainPointsTotal, int *gainArgentTotal);

void ajouterTourCite(Tour *tour, Cite *cite);
void enleverTourCite(int indiceTour, Cite *cite);

bool verifierEmplacement(Cite *cite, Graphe *chemins, int nombreNoeuds, Point *coordClique);
bool verifierEmplacementChemins(Graphe *chemins, int nombreNoeuds, Point *coordClique);

bool construireTour(Cite *cite, Graphe *graphe, int nombreNoeuds, TypeTour type, Point *coordClique);


/*** CITÉ ***/
/* Typiquement des fonctions qu'on ne va appeler qu'une fois*/
Cite *allouerCite(void);
/* libère la cité avec tous ces constituants (tours, etc) */
void libererCite(Cite *cite);

void modifierCite(Cite *cite); // ?? que faire ??aaaaaaaa


#endif