#include "../include/tour.h"

Tour* creerTour(TypeTour type, unsigned int x, unsigned int y)
{
	Tour *tour;
	tour = malloc( sizeof(Tour) );
	if( !tour )
	{
		printf("Tour -- Échec d'allocation dynamique.\n");
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

	tour->suivante = NULL;
	return tour;
}

void terminalTour(Tour *tour)
{
	printf("Tour -> type : %d\n", tour->type);
	printf("		-> position :");
	afficherPoint(tour->coord);
	if( tour->cible )
	{
		printf("		-> cible :\n");
		terminalMonstre(tour->cible);
	}
}

void terminalListe(ListeTour liste)
{
	printf("Liste de tours : \n");
	while( liste )
	{
		terminalTour(liste);
		liste = liste->suivante;
	}
	printf("\n");
}

bool recevoirDegats(Monstre *monstre, Tour *tour, int *gainPoints, int *gainArgent)
{
	/* renvoie vrai si le monstre est vaincu, faux sinon */
	int degats = 1 + tour->puissance/(1+monstre->resistances[tour->type]) ;
	/* on inflige les dégâts */
	monstre->vie -= degats;
	if( monstre->vie <= 0 )
	{
		/* le monstre est occis */
		monstre->etat = estVaincu;
		/* on gagne des points, de l'argent */
		calculerGains(monstre, gainPoints, gainArgent);
		return true;
	}
	//printf("D'acc mais %d/%u, %d \n", monstre->vie, monstre->vieMax, degats);
	return false;
}

void attaquerMonstres(ListeTour liste, clock_t deltaT, Monstre **monstres, int nombreMonstres, int *gainPointsTotal, int *gainArgentTotal)
{
	int gainPoints, gainArgent;
	int totalPoints, totalArgent;
	totalPoints = totalArgent = 0;
	Tour *tour = liste;
	gainPoints = gainArgent = 0;
	while( tour )
	{
		gainPoints = gainArgent = 0;
		attaquerCible(tour, deltaT, monstres, nombreMonstres, &gainPoints, &gainArgent);
		totalPoints += gainPoints;
		totalArgent += gainArgent;
		tour = tour->suivante;
	}
	*gainPointsTotal = totalPoints;
	*gainArgentTotal = totalArgent;
}

void attaquerCible(Tour *tour, clock_t deltaT, Monstre **monstres, int nombreMonstres, int *gainPoints, int *gainArgent)
{
	bool possedeCible, cibleVaincue;
	/** Possède-t-elle une cible ? Doit-elle en changer ? **/
	possedeCible = ( tour->cible && tour->cible->etat == enMouvement );
	if( doitChangerCible(tour) )
	{
		possedeCible = ciblerMonstre(tour, monstres, nombreMonstres);
		tour->tempsTir_acc = 0;
	}
	/** **/
	/** Est-il temps de tirer ? **/
	if( possedeCible )
	{
		if( deltaT + tour->tempsTir_acc > tour->tempsTir )
		{
			/* On attaque la cible */
			cibleVaincue = recevoirDegats(tour->cible, tour, gainPoints, gainArgent);
			/** Si la cible est vaincue**/
			if( cibleVaincue )
			{
				/*et on ne la cible plus */
				tour->cible = NULL;
			}
			/* On oublie pas de réinitialiser l'accumulateur */
			tour->tempsTir_acc = 0;
		}
		else
			tour->tempsTir_acc += deltaT;
	}
}

void traitementListe(ListeTour *liste, clock_t deltaT, Monstre **monstres, int nombreMonstres, int *gainPointsTotal, int *gainArgentTotal)
{
 	/** **/
 	/** On cible et attaque les monstres **/
 	attaquerMonstres(*liste, deltaT, monstres, nombreMonstres, gainPointsTotal, gainArgentTotal);
}

bool verifierEmplacementTour(ListeTour liste, Point *coordClique)
{
	Tour *tour = liste;
	while( tour )
	{
		if( calculerDistance(tour->coord, coordClique) <= 2*RAYON_TOUR )
			return false;
		tour = tour->suivante;
	}
	return true;
}



void reinitialiserCibles(ListeTour liste)
{
	/* on abandonne toutes les cibles 
	* quand on change de vague 
	* car les cibles sont des duplicata
	* de pointeurs libérés (monstres)
	*/
	while( liste )
	{
		liste->cible = NULL;
		liste = liste->suivante;
	}
}


bool estAPortee(Tour *tour, Monstre *monstre)
{
	/* ne fonctionne que si tour cible un monstre */
	/* Ah oui ? */
	bool aPortee;
	Point coordMonstre;
	if( monstre->etat == enMouvement )
	{
		calculerPositionMonstre(monstre, &coordMonstre);
		aPortee = ( calculerDistance(tour->coord, &coordMonstre) <= (int)tour->portee );
		return aPortee;
	}
	return false;
}



bool doitChangerCible(Tour *tour)
{
	/* on doit changer de cible si l'on en a pas */
	/* on doit vérifié que le monstre n'a pas été libéré
	* -> qu'il appartient bien à la vague actuelle.
	* En fait on règle le problème à chaque fin de vague.
	*/
	bool possedeCible = ( tour->cible && tour->cible->etat == enMouvement );
	if( possedeCible )
		return !estAPortee(tour, tour->cible);
	return true;
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
	indicesMonstres = creerVecteurEntier(nombreMonstresCibles, -1);
	distancesMonstres = creerVecteurEntier(nombreMonstresCibles, -1);
	j=0;
	for( k=0; k<nombreMonstres; k++ )
	{
		/* ici on récupère aussi la distance,
		* donc pas possible d'utiliser estAPortee
		*/
		if( monstres[k]->etat == enMouvement )
		{
			calculerPositionMonstre(monstres[k], &coordMonstre);
			d = calculerDistance(tour->coord, &coordMonstre);
			if( d <= (int)tour->portee )
			{
				indicesMonstres[j] = k;
				distancesMonstres[j] = d;
				j++;
			}
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
	/**** QU'EST-CE QUE JE SUIS BÊTE LE PROBLÈME ÉTAIT LÀ AAAAAAA
	* l'indiceMin que l'on choisi par défaut est indicesMonstres[0] PAS 0
	**** AERGA */
	int indiceMin = indicesMonstres[0];
	int minDistance = distancesMonstres[0];
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
	while( liste->suivante )
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
	Tour *suivante;
	while( liste )
	{
		suivante = liste->suivante;
		libererTour(liste);
		liste = suivante;
	}
}

void libererTour(Tour *tour)
{
	free(tour);
}
clock_t calculerTempsTir(TypeTour type)
{
	clock_t tempsTir = (clock_t)( (double)CLOCKS_PER_SEC*TEMPS_TIR_BASE*TEMPS_TIR_TYPE[type] );
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
