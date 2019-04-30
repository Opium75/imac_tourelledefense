#include "../include/monstre.h"

Monstre* creerMonstre(TypeMonstre type, Noeud *depart) 
{
	Monstre *monstre = malloc( sizeof(Monstre) );
	if( !monstre )
	{
		printf("Monstre - Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	monstre->vieMax = calculerVie(type);
	monstre->vie = monstre->vieMax;
	monstre->vitesse = calculerVitesse(type);
	calculerResistances(monstre->resistances, type);

	monstre->type = type;

	monstre->depart = depart;
	monstre->arrivee = NULL;
	monstre->avancement = 0.;

	monstre->parcours = NULL;
	monstre->nombreEtapes = 0;
	monstre->indiceEtape = -1;

	return monstre;
}

void libererMonstre(Monstre *monstre)
{
	if( monstre->parcours )
		libererVecteurEntier(monstre->parcours);
	free(monstre);
}


void calculerPositionMonstre(Monstre *monstre, Point *coord)
{
	unsigned int x,y;
	x = valeurAffine(monstre->depart->coord->x, monstre->arrivee->coord->x, monstre->avancement);
	y = valeurAffine(monstre->depart->coord->y, monstre->arrivee->coord->y, monstre->avancement);
	coord->x = x;
	coord->y = y;
}

void afficherMonstre(Monstre *monstre)
{
	int j;
	printf("Monstre -> type : %s, vie : %d/%u, vitesse : %u\n", TYPE_MONSTRE[monstre->type], monstre->vie, monstre->vieMax, monstre->vitesse);
	printf("		-> résistances : ");
	for( j=0; j<NB_TYPES_TOUR; j++ )
	{
		printf("%d, ", monstre->resistances[j]);
	}
	printf("\n");
	Point positionMonstre;
	calculerPositionMonstre(monstre, &positionMonstre);
	printf("		-> position (%u,%u) _ %d::%d, %lf % \n", positionMonstre.x, positionMonstre.y, monstre->depart->indice, monstre->arrivee->indice, monstre->avancement*100);
}


unsigned int calculerVie(TypeMonstre type)
{
	unsigned int vie = VIE_BASE*VIE_TYPE[type];
	return vie;
}

unsigned int calculerVitesse(TypeMonstre type)
{
	unsigned int vitesse = VITESSE_BASE*VITESSE_TYPE[type];
	return vitesse;
}

void calculerResistances(unsigned int resistances[], TypeMonstre type)
{
	int i;
	for( i=0; i<NB_TYPES_TOUR; i++)
	{
		resistances[i] = RESISTANCE_BASE*RESISTANCES_TYPE[type][i];
	}
}

TypeMonstre calculerTypeMonstre()
{
	/* J'aimerai donner des arguments à la fonction,
	* histoire qu'on ait pas une probabilité uniforme/
	/* on pourrait améliorer l'aléatoire (rand() = caca) */
	int type = rand()%NB_TYPES_MONSTRE;
	return (TypeMonstre) type;
}

Noeud* calculerEntreeMonstre(Noeud **entrees, int nombreEntreesVague)
{
	Noeud *entree = entrees[rand()%nombreEntreesVague];
	return entree;
}

unsigned int valeurAffine(unsigned int a, unsigned int b, double avancement)
{
	unsigned int c = (unsigned int)((b - a)*avancement) + a;
	return c;
}
