#ifndef __FONCTIONS_POINT

#define __FONCTIONS_POINT

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define max(a,b) a>b ? a : b
#define min(a,b) a>b ? b : a


#define NB_DIMENSIONS 2

#define RADIAN_VERS_DEGRES 180.0 / M_PI

typedef struct {
	unsigned int x, y;
} Point, *ListePoint, Dimensions;

/*points*/
Point* creerPoint(unsigned int x, unsigned int y);
void libererPoint(Point *point);

/* distance euclidienne entre deux points */
int calculerDistance(Point *p1, Point *p2);
/* distance au carrée -> plus rapide car pas de racine carrée */
int calculerDistanceCarree(Point *p1, Point *p2);

/* calcul du projeté orthogonal de point sur la droite (P1P2) */
Point* creerProjeteOrtho(Point *point, Point *segP1, Point *segP2);
/* utilisation du projeté pour calculer la distance du point au segment */
int calculerDistanceSegment(Point *point, Point *segP1, Point *segP2);
/* vérifie si le projeté est sur le segment (et pas seulement sur la droite. */
bool estProjeteSegment(Point *point, Point *segP1, Point *segP2);

/* utile pour afficher des éléments à un pourcentage de l'écran */
void calculerCoordonneesPourcentage(Point *coordEchelle, Point *coord, Dimensions *dimImage);
void calculerDimensionsPourcentage(Dimensions *dimEchelle, Dimensions *dim, Dimensions *dimImage);


unsigned int valeurAffine(unsigned int a, unsigned int b, double avancement);

double calculerAngle(Point *origine, Point *point);

/*Copie les coordonnées d'un point vers un nouveau point, et renvoie un pointeur vers la copie*/
Point* copiePoint(Point *point);

void afficherPoint(Point *point);


/*liste de points*/
/* allocation d'une liste de points initialisés à (0, 0).*/
ListePoint* allouerListePoint(int nombrePoints);
/*On libère la liste en libérant ou pas son contenu, suivant avecContenu*/
void libererListePoint(int nombrePoints, ListePoint *listePoint, bool avecContenu);

#endif