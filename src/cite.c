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

void traitementCite(Cite *cite, clock_t deltaT, Carte *carte, Monstre **monstres, int nombreMonstres)
{
	/* on va s'occuper d'abord des tours */
	ListeTour liste = &(cite->listeTour);
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