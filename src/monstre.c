#include "../include/monstre.h"

Monstre* creerMonstre(TypeMonstre type, Noeud *depart, Noeud *arrivee)
{
	Monstre *monstre = malloc( sizeof(Monstre) );
	if( !monstre )
	{
		printf("Monstre - Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	monstre->ptVie = calculerVie(type);
	monstre->vitesse = calculerVitesse(type);
	calculerResistances(monstre->resistances, type);

	monstre->type = type;

	monstre->depart = depart;
	monstre->arrivee = arrivee;
	monstre->avancement = 0.;
	return monstre;
}

void libererMonstre( Monstre *monstre)
{
	free(monstre);
}

unsigned int calculerVie(TypeMonstre type)
{
	unsigned int ptVie = VIE_BASE*VIE_TYPE[type];
	return ptVie;
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