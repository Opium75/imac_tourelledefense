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
	tour->portee = calculerPortee(type);
	tour->coutAchat = calculerCout(type);

	tour->tempsTir = calculerTempsTir(type);
	tour->tempsTir_acc = 0;

	tour->coord = creerPoint(x,y);

	tour->cible = NULL;
	return tour;
}

void attaquerCible(Tour *tour)
{
	Monstre *monstre = tour->cible;
	int degats = tour->puissance/monstre->resistances[tour->type] ;
	monstre->vie -= plancher(degats);
}

void attaquerMonstres(ListeTour liste, clock_t deltaT, Monstre **monstres, int nombreMonstres)
{
	Tour *tour;
 	bool possedeCible;
	while( liste )
	{
		tour = liste;
		/*On change de cible si nécessaire */
		possedeCible = tour->cible ? true : false;
		if( doitChangerCible(tour) )
		{
			possedeCible = ciblerMonstre(tour, monstres, nombreMonstres);
		}
		if( deltaT + tour->tempsTir_acc > tour->tempsTir )
		{
			if( possedeCible )
			{
				/* on attaque la cible */
				attaquerCible(tour);

			}
			tour->tempsTir_acc = 0;
		}
		else
		{
			tour->tempsTir_acc += deltaT;
		}
	}
}

void traitementListe(ListeTour *liste, clock_t deltaT, Monstre **monstres, int nombreMonstres)
{
 	Tour *tour;
 	/**

 	/** On cible et attaque les monstres **/
 	attaquerMonstres(&liste, deltaT, monstres, nombreMonstres);
}


bool estAPortee(Tour *tour, Monstre *monstre)
{
	/* ne fonctionne que si tour cible un monstre */
	bool aPortee;
	Point coordMonstre;
	calculerPositionMonstre(monstre, &coordMonstre);
	aPortee = ( calculerDistance(tour->coord, &coordMonstre) <= (int)tour->portee );
	return aPortee;
		

}



bool doitChangerCible(Tour *tour)
{
	/* on doit changer de cible si l'on en a pas */
	return( !tour->cible || !estAPortee(tour, tour->cible) );
}


bool ciblerMonstre(Tour *tour, Monstre **monstres, int nombreMonstres)
{
	int nombreMonstresCibles, k, j, d;
	int indiceCible;
	int *indicesMonstres, *distancesMonstres;
	Point coordMonstre;
	/* indexer les monstres pouvant être ciblés */
	nombreMonstresCibles = 0;
	for( k=0; k<nombreMonstres; k++ )
	{
		if( estAPortee(tour, monstres[k]) )
			nombreMonstresCibles++;
	}
	/* Si aucun monstre n'est à la portée de la tour
	* on renvoie faux, la tour n'a pas de cible
	* elle cible donc sur NULL
	*/
	if(nombreMonstresCibles == 0)
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
		/* ici on récupère aussi la distance,
		* donc pas possible d'utiliser estAPortee
		*/
		calculerPositionMonstre(monstres[k], &coordMonstre);
		if( ( d = calculerDistance(tour->coord, &coordMonstre) ) <= (int)tour->portee )
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

Tour* accesTourIndice(int indiceTour, ListeTour liste)
{
	/* ne vérifie pas que l'indice est correct */
	int j = 0;
	Tour *tour;
	while( j < indiceTour && liste )
		liste = liste->suivante;
	tour = liste;
	return tour;
}

int trouverIndiceTour(ListeTour liste, unsigned int x, unsigned int y)
{
	int indiceTour;
	bool trouve;
	Tour *tour;
	/* initialisation */
	indiceTour = 0;
	trouve = false;
	tour = liste;
	while( tour )
	{
		if( tour->coord->x == x && tour->coord->y == y)
		{
			trouve = true;
			break;
		}
		indiceTour++;
		tour = tour->suivante;
	}
	if( trouve )
		return indiceTour;
	else 
		return -1;
}

int trouverIndiceTourRayon(ListeTour liste, unsigned int x, unsigned int y, int rayon)
{
	/* grosso modo le même algorithme que pour une cible
	* On fait deux fois les mêmes calculs.
	*/
	int *indicesTours, nombreToursTrouvees;
	int *distancesTours, d;
	int j, k, indiceTourChoisie;
	Tour *tour;
	Point coordClique;
	coordClique.x = x; coordClique.y = y;
	/* */
	nombreToursTrouvees = 0;
	tour = liste;
	while( tour )
	{
		if( calculerDistance(tour->coord, &coordClique) <= rayon )
			nombreToursTrouvees++;
	}
	if( nombreToursTrouvees == 0)
		return -1;
	/* on recommence */
	indicesTours = creerVecteurEntier(nombreToursTrouvees,0);
	distancesTours = creerVecteurEntier(nombreToursTrouvees, 0);
	j=0;
	for( k=0; k<nombreToursTrouvees; k++ )
	{
		if( (d = calculerDistance(tour->coord, &coordClique) ) <= rayon)
		{
			indicesTours[j] = k;
			distancesTours[j] = d;
			j++;
		}
	}
	if( j!= nombreToursTrouvees )
	{
		printf("Tour -- J'ai un hyperplan dans mon tesseracte !\n");
		exit(EXIT_FAILURE);
	}

	indiceTourChoisie = calculerIndiceClique(indicesTours, distancesTours,nombreToursTrouvees);
	libererVecteurEntier(indicesTours);
	libererVecteurEntier(distancesTours);
	return indiceTourChoisie;
}

int calculerIndiceClique(int *indicesTours, int *distancesTours, int nombreToursTrouvees)
{
	/* comme on fait ici sensiblement la même chose
	* qu'avec les monstres, on renvoie à la fonction de la cible.
	* On pourra toujours changer plus tard.
	*/
	return calculerIndiceCible( indicesTours, distancesTours, nombreToursTrouvees);
}

int longueurListe(ListeTour liste )
{
	int nombreTours = 0;
	while( liste )
	{
		nombreTours++;
		liste = liste->suivante;
	}
	return nombreTours;
}


void ajouterTour(Tour *tour, ListeTour liste)
{
	while( liste )
		liste = liste->suivante;
	liste->suivante = tour;
}

void enleverPremiereTour(ListeTour *liste)
{
	Tour *suivante = (*liste)->suivante;
	libererTour(*liste);
	*liste = suivante;
}

void enleverTour(int indiceTour, ListeTour liste)
{
	/* même chose que les vagues */
	int j;
	Tour *celleDapres;
	for( j=0; j<indiceTour-1; j++ )
		liste = liste->suivante;
	if( liste->suivante )
		celleDapres = (liste->suivante)->suivante;
	else
		celleDapres = NULL;
	libererTour(liste->suivante);
	liste->suivante = celleDapres;

}

void libererListeTour(ListeTour liste)
{
	while( liste )
		libererTour(liste);
}

void libererTour(Tour *tour)
{
	free(tour);
}

time_t calculerTempsTir(TypeTour type)
{
	time_t tempsTir = CLOCKS_PER_SEC*TEMPS_TIR_BASE*TEMPS_TIR_TYPE[type];
	return tempsTir;
}

unsigned int calculerPuissance(TypeTour type)
{
	unsigned int puissance = PUISSANCE_BASE*PUISSANCE_TYPE[type];
	return puissance;
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
