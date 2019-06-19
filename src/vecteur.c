#include "../include/vecteur.h"



int* creerVecteurEntier(int taille, int initial)
{
	int i;
	int* vecteur;
	if( initial == 0)
		vecteur = calloc( taille, sizeof(int) );
	else
		vecteur = malloc( taille * sizeof(int) );
	if( !vecteur )
	{
		printf("Vecteur -- Échec d'allocation dynamique. (taille : %d)\n", taille);
		exit(EXIT_FAILURE);
	}
	if( initial != 0 )
	{
		for( i=0; i<taille; i++ )
		{
			vecteur[i] = initial;
		}
	}
	return vecteur;
}

void afficherVecteurEntier(int *vecteur, int taille)
{
	int i;
	printf("Vecteur d'entiers (%d)-> (", taille);
	for( i=0; i<taille; i++ )
	{
		printf("%d, ", vecteur[i]);
	}
	printf(")\n");
}

int estDansVecteur(int element, int *vecteur, int taille)
{
	int i;
	for(i=0; i<taille; i++ )
	{
		if( vecteur[i] == element )
			return i;
	}
	/* si non trouvé */
	return -1;
}

void libererVecteurEntier(int* vecteur)
{
	free(vecteur);
}

int** allouerMatriceCarree(int rang)
{
	int ligne=0, colonne=0;
	int **matrice= malloc(rang * sizeof(int*));
	if( !matrice )
	{
		printf("Échec de l'allocation de la matrice carrée. (rang : %d)\n", rang);
		exit(EXIT_FAILURE);
	}
	for(ligne=0; ligne<rang; ligne++)
	{
		matrice[ligne] = malloc(rang * sizeof(int));
		if( !matrice[ligne] )
		{
			printf("Échec de l'allocation de la matrice carrée. (rang : %d)\n", rang);
			exit(EXIT_FAILURE);
		}
		for(colonne=0; colonne<rang; colonne++)
		{
			matrice[ligne][colonne] = 0;
		}
	}
	return matrice;
}


void libererMatriceCarree(int rang, int **matrice)
{
	int ligne;
	for(ligne=0; ligne<rang; ligne++)
	{
		free(matrice[ligne]);
	}
	free(matrice);
}

void afficherMatriceCarree(int rang, int **matrice)
{
	int ligne, colonne;
	for(colonne=0; colonne<rang; colonne++)
	{
		printf("\n( ");
		for(ligne=0; ligne<rang; ligne++)
		{
			printf("%6d ", matrice[ligne][colonne]);
		}
		printf(" )\n");
	}
}