#ifndef __FONCTIONS_PARCOURS

#define __FONCTIONS_PARCOURS

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "point.h"
#include "graphe.h"
#include "carte.h"
#include "monstre.h"
#include "tour.h"
#include "cite.h"

#define PROBA_ECART 0.2
#define INVERSE_PROBA_ECART (int)(1/PROBA_ECART)

/* revoie la liste des indices des noeuds à parcourir selon le plus court chemin */
int* plusCourtChemin(Monstre *monstre, Carte *carte, Cite *cite, int *nombreEtapes);
/* algorithme de dijkstra, avec les distances pondérées par les tours
* plus il y a de tours à portées du segment,
* plus la distance est considérée longue.
*/
int* dijkstra(Noeud *noeud0, Graphe *chemins, int nombreNoeuds, Tour *listeTour, int *distances);

int calculerDistancePonderee(Noeud *noeud, Noeud *voisin, Tour *listeTour);
int indiceMinDistance(int *listeVerifies, int *distances, int nombreNoeuds);


/* fait avancer le monstre sur son segment, ou lui en fait changer
* s'il le dépasse. Renvoie vrai s'il a atteint le noeud de sortie.
 deltaT est le temps écoulé depuis la dernière itération.
*/
bool avancerMonstre(Monstre *monstre, clock_t deltaT, Carte *carte, Cite *cite);
bool changerSegmentMonstre(Monstre *monstre, double reste, Carte *carte, Cite *cite);

#endif