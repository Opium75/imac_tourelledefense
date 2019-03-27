#ifndef __FONCTIONS_POINT

#define __FONCTIONS_POINT

#include <stdio.h>
#include <stdlib.h>


#define NB_DIMENSIONS 2

typedef struct {
	unsigned int x, y;
} Point, *ListePoint, Vecteur;

/*points*/
Point* creerPoint(unsigned int x, unsigned int y);
void libererPoint(Point *point);


/*liste de points*/
/* allocation d'une liste de points initialisés à (0, 0).*/
ListePoint* allouerListePoint(int nombrePoints);
void libererListePoint(int nombrePoints, ListePoint *listePoint);

#endif