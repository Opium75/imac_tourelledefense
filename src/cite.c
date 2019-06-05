#include "../include/cite.h"


Cite* allouerCite(void) 
{
	Cite *cite = malloc( sizeof(Cite) );
	if( !cite )
	{
		printf("Cité -- Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	cite->listeTour = NULL;
	cite->modif_INDIC = 0;
	return cite;
}

void libererCite(Cite *cite)
{
	if(cite->listeTour)
		libererListeTour(cite->listeTour);
	free(cite);
}

bool construireTour(Cite *cite, Graphe *chemins, int nombreNoeuds, TypeTour type, Point *coordClique)
{
	/** VÉRIFICATIONS DE L'EMPLACEMENT **/
	if( !verifierEmplacement(cite, chemins, nombreNoeuds, coordClique) )
		return false;
	/*  */
	Tour *tour = creerTour(type, coordClique->x, coordClique->y);
	ajouterTourCite(tour, cite);
	return true;
}

bool verifierEmplacement(Cite *cite, Graphe *chemins, int nombreNoeuds, Point *coordClique)
{
	bool valide = true;
	/* on vérifie si le point est trop proche d'une tour, d'un chemin... */
	//valide &=verifierEmplacementTour(cite->listeTour, coordClique);
	if( !valide )
		return false;
	valide &= verifierEmplacementChemins(chemins, nombreNoeuds, coordClique);
	return valide;
}

bool verifierEmplacementChemins(Graphe *chemins, int nombreNoeuds, Point *coordClique)
{
	int i, j;
	int distanceSeg;
	Noeud *noeud, *voisin;
	for( i=0; i<nombreNoeuds; i++ )
	{
		noeud = chemins[i];
		for( j=0; j<noeud->nombreSuccesseurs; j++ )
		{
			voisin = noeud->successeurs[j];
			distanceSeg = calculerDistanceSegment(coordClique, noeud->coord, voisin->coord);
			if(	estProjeteSegment(coordClique, noeud->coord, voisin->coord) )
			{
				
				if( distanceSeg <= RAYON_TOUR + LARGEUR_CHEMIN )
				{
					return false;
				}
			}
		}
	}
	return true;
}

void traitementCite(Cite *cite, clock_t deltaT, Carte *carte, Monstre **monstres, int nombreMonstres)
{
	/* on va s'occuper d'abord des tours */
	ListeTour *liste = &(cite->listeTour);
	traitementListe(liste, deltaT, monstres, nombreMonstres);
}

void modifierCite(Cite *cite); // ?? que faire ??aaaaaaaa

void ajouterTourCite(Tour *tour, Cite *cite)
{
	if( cite->listeTour )
		ajouterTour(tour, cite->listeTour);
	else
		cite->listeTour = tour;
	cite->modif_INDIC++;
}

void enleverTourCite(int indiceTour, Cite *cite)
{
	if( indiceTour == 0)
		enleverPremiereTour( &(cite->listeTour) );
	else
		enleverTour(indiceTour, cite->listeTour);
	cite->modif_INDIC++;
}