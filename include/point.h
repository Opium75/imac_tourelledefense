#ifndef __FONCTIONS_POINT

#define __FONCTIONS_POINT

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define NB_DIMENSIONS 2

typedef struct {
	unsigned int x, y;
} Point, *ListePoint, Vecteur;

/*points*/
Point* creerPoint(unsigned int x, unsigned int y);
void libererPoint(Point *point);

/*Copie les coordonnées d'un point vers un nouveau point, et renvoie un pointeur vers la copie*/
Point* copiePoint(Point *point);


/*liste de points*/
/* allocation d'une liste de points initialisés à (0, 0).*/
ListePoint* allouerListePoint(int nombrePoints);

/*On libère la liste en libérant ou pas son contenu, suivant avecContenu*/
void libererListePoint(int nombrePoints, ListePoint *listePoint, bool avecContenu);

#endif