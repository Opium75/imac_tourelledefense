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

Point* copiePoint(Point *point)
{
	return ( creerPoint(point->x, point->y) ) ;
}

int calculerDistance(Point *p1, Point *p2)
{
	int distance;
	distance = (int)sqrt( pow(((int)p2->x - (int)p1->x),2) + pow(((int)p2->y - (int)p1->y),2) );
	return distance;
}

int calculerDistanceCarree(Point *p1, Point *p2)
{
	int distanceCarree;
	distanceCarree = (int)(pow( (int)p2->x - p1->x, 2 ) + pow( (int)p2->y - p1->y, 2 ) );
	return distanceCarree;
}

Point* creerProjeteOrtho(Point *point, Point *segP1, Point *segP2)
{
	double xProj, yProj;
	int deltaX = (int)segP2->x - (int)segP1->x;
	int deltaY = (int)segP2->y - (int)segP1->y;
	int orgineX = (int)point->x - (int)segP1->x;
	int origineY = (int)point->y - (int)segP1->y;
	bool renverser = ( abs(deltaX) < abs(deltaY) );
	double c = renverser ? deltaX/(double)deltaY : deltaY/(double)deltaX;
	double norme = sqrt( (double)pow(deltaY,2) + pow(deltaX,2) );
	double prodScal = deltaX*orgineX + deltaY*origineY;
	xProj = prodScal/norme;
	yProj = prodScal/norme;
	if( renverser )
		xProj *= c;
	else
		yProj *= c;
	xProj += segP1->x;
	yProj += segP1->y;
	Point *projeteOrtho = creerPoint( (unsigned int)xProj, (unsigned int)yProj );
	return projeteOrtho;
}

int calculerDistanceSegment(Point *point, Point *segP1, Point *segP2)
{
	Point *projeteOrtho = creerProjeteOrtho(point, segP1, segP2);
	int distanceSegment = calculerDistance(point, projeteOrtho);
	libererPoint(projeteOrtho);
	return distanceSegment;
}

bool estProjeteSegment(Point *point, Point *segP1, Point *segP2)
{
	bool estProjete;
	int minX, maxX, minY, maxY;

	minX = min((int)segP1->x, (int)segP2->x);
	maxX = max((int)segP1->x, (int)segP2->x);
	minY = min((int)segP1->y, (int)segP2->y);
	maxY = max((int)segP1->y, (int)segP2->y);
	/* vérifie si le projeté est sur le segment (et pas seulement sur la droite. */
	estProjete = (point->x > minX) && (point->x <  maxX);
	estProjete |= (point->y > minY) && (point->y < maxY);
	return estProjete;
}

void calculerCoordonneesPourcentage(Point *coordEchelle, Point *coord, Dimensions *dimImage)
{
    coordEchelle->x = (unsigned int)( ((double)coord->x/100. * (double)dimImage->x) );
    coordEchelle->y =   (unsigned int)( ((double)coord->y/100. *(double)dimImage->y) );
}


unsigned int valeurAffine(unsigned int a, unsigned int b, double avancement)
{
	unsigned int c = (unsigned int)(((int)b - (int)a)*avancement) + a;
	return c;
}

double calculerAngle(Point *origine, Point *point)
{
	/* renvoie l'angle décrit par la droit, par rapport à (0,x)+ */ 
	double x = (double)( (int)point->x - (int)origine->x );
	double y = (double)( (int)point->y - (int)origine->y );
	double angle = atan2(x,y)*RADIAN_VERS_DEGRES;
	return angle;
}

void libererPoint(Point *point)
{
	free(point);
}

void afficherPoint(Point *point)
{
	printf(" (%u, %u)\n", point->x, point->y);
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


void libererListePoint(int nombrePoints, ListePoint *listePoint, bool avecContenu )
{

	int i;
	if( avecContenu )
	{
			for(i=0; i<nombrePoints; i++)
		{
			free(listePoint[i]);
		}
	}
	free(listePoint);
}