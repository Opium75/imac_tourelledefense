#ifndef __FONCTIONS_PARCOURS

#define __FONCTIONS_PARCOURS

#include <stdio.h>
#include <stdlib.h>

#include "monstre.h"
#include "graphe.h"
#include "carte.h"

/* revoie la liste des indices des noeuds à parcourir selon le plus court chemin */
int* plusCourtChemin(Monstre *monstre, Carte *carte, Tour **tours, int nombreTours, int *nombreEtapes);
/* algorithme de dijkstra, avec les distances pondérées par les tours
* plus il y a de tours à portées du segment,
* plus la distance est considérée longue.
*/
int* dijkstra(Noeud *noeud0, Graphe *chemins, int nombreNoeuds, Tour **tours, int nombreTours);

int calculerDistancePonderee(Noeud *noeud, Noeud *voisin, Tour **tours, int nombreTours);
int indiceMinDistance(int *listeVerifies, int *distances, int nombreNoeuds);

int* creerIndicesPrecedents(int nombreNoeuds);
void libererIndicePrecedents( int *indicesPrecedents);

int* creerDistances(int nombreNoeuds);
void libererDistances(int *distances);

int* creerVecteurEntier(int taille, int initial);

void afficherVecteur(int *vecteur, int taille);

#endif