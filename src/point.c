#include "../include/point.h"

Point* creerPoint(unsigned int x, unsigned int y)
{
	Point *point = malloc(sizeof(Point));
	if( !point )
	{
		printf("Échec d'allocation dynamique d'un point. (%u, %u) \n", x, y);
		exit(EXIT_FAILURE);
	}
	point->x = x;
	point->y = y;
	return point;
}

void libererPoint(Point *point)
{
	free(point);
}

ListePoint* allouerListePoint(int nombrePoints)
{
	int i;
	ListePoint *listePoint = malloc(nombrePoints * sizeof(Point*));
	if( !listePoint )
	{
		printf("Échec d'allocation dynamique d'une liste de points. (nombre de points : %d) \n", nombrePoints);
		exit(EXIT_FAILURE);
	}
	for(i=0; i<nombrePoints; i++)
	{
		listePoint[i] = creerPoint(0,0); /*par défaut*/
	}
	return listePoint;
}

void libererListePoint(int nombrePoints, ListePoint *listePoint)
{
	int i;
	for(i=0; i<nombrePoints; i++)
	{
		free(listePoint[i]);
	}
	free(listePoint);
}