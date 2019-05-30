#include "../include/couleur.h"

bool comparerCouleurs(unsigned char couleur1[], unsigned char couleur2[])
{
	int c;
	for(c=0; c<NB_COULEURS; c++)
	{
		if( couleur1[c] != couleur2[c] )
		{
			return false;
		}
	}
	return true;
}

void copierCouleur(unsigned char copie[], const unsigned char couleur[])
{
	int c;
	for( c=0; c<NB_COULEURS; c++ )
	{
		copie[c] = couleur[c];
	}
}

void homothetieCouleur(unsigned char couleur[], double scalaire)
{
	/* attention scalaire positif !*/
	int c;
	int valeur;
	for( c=0; c<NB_COULEURS; c++ )
	{
		valeur = couleur[c]*scalaire;
		couleur[c] = valeur <= MAX_VAL_COULEUR ? valeur : MAX_VAL_COULEUR;
	}
}

void afficherCouleur(unsigned char couleur[])
{
	int c;
	printf("(");
	for(c=0; c<NB_COULEURS; c++)
	{
		printf("%hhu, ",couleur[c]);
	}
	printf(")\n");
}