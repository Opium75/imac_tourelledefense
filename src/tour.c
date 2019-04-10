#include "../include/tour.h"



Tour* creerTour(TypeTour type, unsigned int x, unsigned int y)
{
	Tour *tour;
	tour = malloc( sizeof(Tour) );
	if( !tour )
	{
		printf("Tour - Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	tour->puissance = calculerPuissance(type);
	tour->cadence = calculerCadence(type);
	tour->portee = calculerPortee(type);
	tour->coutAchat = calculerCout(type);

	tour->type = type;
	tour->coord = creerPoint(x,y);
	return tour;
}

void libererTour(Tour *tour)
{
	free(tour);
}




unsigned int calculerPuissance(TypeTour type)
{
	unsigned int puissance = PUISSANCE_BASE*PUISSANCE_TYPE[type];
	return puissance;
}

unsigned int calculerCadence(TypeTour type)
{
	unsigned int cadence = CADENCE_BASE*CADENCE_TYPE[type];
	return cadence;
}

unsigned int calculerPortee(TypeTour type)
{
	unsigned int portee = PORTEE_BASE*PORTEE_TYPE[type];
	return portee;
}

unsigned int calculerCout(TypeTour type)
{
	unsigned int cout = COUT_BASE*COUT_TYPE[type];
	return cout;
}