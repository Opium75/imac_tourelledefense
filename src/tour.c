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
	tour->type = type;

	tour->puissance = calculerPuissance(type);
	tour->cadence = calculerCadence(type);
	tour->portee = calculerPortee(type);
	tour->coutAchat = calculerCout(type);

	tour->coord = creerPoint(x,y);

	tour->cible = NULL;
	return tour;
}


bool ciblerMonstre(Tour *tour, Monstre **monstres, int nombreMonstres)
{
	int nombreMonstresCibles, k, j, d;
	int indiceCible;
	int *indicesMonstres, *distancesMonstres;
	Point *coordMonstre = creerPoint(0,0);
	/* indexer les monstres pouvant être ciblés */
	nombreMonstresCibles = 0;
	for( k=0; k<nombreMonstres; k++ )
	{
		calculerPositionMonstre(monstres[k], coordMonstre);
		if( calculerDistance(tour->coord, coordMonstre) <= (int)tour->portee )
			nombreMonstresCibles++;
	}
	/* Si aucun monstre n'est à la portée de la tour
	* on renvoie faux, la tour n'a pas de cible
	* elle cible donc sur NULL
	*/
	if( nombreMonstresCibles == 0)
	{
		tour->cible = NULL;
		return false;
	}
	/* on en connaît maintenant le nombre, on recommence en enregistrant */
	indicesMonstres = creerVecteurEntier(nombreMonstres, 0);
	distancesMonstres = creerVecteurEntier(nombreMonstres, 0);
	j=0;
	for( k=0; k<nombreMonstres; k++ )
	{
		calculerPositionMonstre(monstres[k], coordMonstre);
		if( ( d = calculerDistance(tour->coord, coordMonstre) ) <= (int)tour->portee )
		{
			indicesMonstres[j] = k;
			distancesMonstres[j] = d;
			j++;
		}
	}
	if( j!= nombreMonstresCibles )
	{
		printf("Tour -- J'ai un serpent dans ma botte !\n");
		exit(EXIT_FAILURE);
	}
	/* On calcule l'indice du monstre ciblé */
	indiceCible = calculerIndiceCible(indicesMonstres, distancesMonstres, nombreMonstresCibles);
	/* la tour cible le monstre choisi */
	tour->cible = monstres[indiceCible];
	/* On libère les vecteurs utilisés */
	libererVecteurEntier(indicesMonstres);
	libererVecteurEntier(distancesMonstres);
	/* La tour a trouvé une cible. */
	return true;
}

int calculerIndiceCible(int *indicesMonstres, int *distancesMonstres, int nombreMonstresCibles)
{
	int j;
	/* On choisira de cibler le monstre le plus proche. */
	int indiceMin = 0, minDistance = distancesMonstres[0];
	for( j=0; j<nombreMonstresCibles; j++ )
	{
		if( distancesMonstres[j] < minDistance )
		{
			indiceMin = indicesMonstres[j];
			minDistance = distancesMonstres[j];
		}
	}
	return indiceMin;
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
