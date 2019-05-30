#include "../include/monstre.h"

Monstre* creerMonstre(TypeMonstre type, unsigned char niveau, Noeud *depart) 
{
	Monstre *monstre = malloc( sizeof(Monstre) );
	if( !monstre )
	{
		printf("Monstre - Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	monstre->vieMax = calculerVie(type, niveau);
	monstre->vie = monstre->vieMax;
	monstre->vitesse = calculerVitesse(type, niveau);
	monstre->attaque = calculerAttaque(type, niveau);
	calculerResistances(monstre->resistances, type);

	monstre->type = type;
	monstre->etat = enAttente;

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

void terminalMonstre(Monstre *monstre)
{
	int j;
	Point positionMonstre;
	printf("Monstre -> type : %s, état : %s, vie : %d/%u, vitesse : %u\n", TYPE_MONSTRE[monstre->type], ETAT_MONSTRE[monstre->etat], monstre->vie, monstre->vieMax, monstre->vitesse);
	printf("		-> résistances : ");
	for( j=0; j<NB_TYPES_TOUR; j++ )
	{
		printf("%d, ", monstre->resistances[j]);
	}
	printf("\n");
	if( monstre->etat == enMouvement )
	{
		printf("		-> Segment  _ %d::%d, %lf % \n", monstre->depart->indice, monstre->arrivee->indice, monstre->avancement*100);
		calculerPositionMonstre(monstre, &positionMonstre);
		printf("		-> position (%u, %u)\n", positionMonstre.x, positionMonstre.y);	
	}
}

void attaquerJoueur(Monstre *monstre, int *pointage, int *argent)
{
	int perteArgent = monstre->attaque;
	*argent -= perteArgent;
	(*pointage)--;
}

unsigned int calculerVie(TypeMonstre type, unsigned char niveau)
{
	unsigned int vie = VIE_BASE*VIE_TYPE[type]*niveau+1;
	return vie;
}

int calculerVitesse(TypeMonstre type, unsigned char niveau)
{
	int vitesse = VITESSE_BASE*VITESSE_TYPE[type]*niveau+1;
	return vitesse;
}

int calculerAttaque(TypeMonstre type, unsigned char niveau)
{
	int attaque = ATTAQUE_TYPE[type]*ATTAQUE_BASE*niveau+1;
	return attaque;
}

void calculerResistances(unsigned int resistances[], TypeMonstre type)
{
	int i;
	for( i=0; i<NB_TYPES_TOUR; i++)
	{
		resistances[i] = RESISTANCE_BASE*RESISTANCES_TYPE[type][i];
	}
}

TypeMonstre calculerTypeMonstre(void)
{
	/* J'aimerai donner des arguments à la fonction,
	* histoire qu'on ait pas une probabilité uniforme
	/* on pourrait améliorer l'aléatoire (on a bien mieux que rand()) */
	int type = rand()%NB_TYPES_MONSTRE;
	return (TypeMonstre) type;
}

Noeud* calculerEntreeMonstre(Noeud **entrees, int nombreEntreesVague)
{
	Noeud *entree = entrees[rand()%nombreEntreesVague];
	return entree;
}