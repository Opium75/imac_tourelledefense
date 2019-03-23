#include "../include/noeud.h"

Noeud* allouerNoeud(int nombreSuccesseurs)
{
	/** ON SUPPOSE ICI QU'UN NOEUD PEUT NE PAS AVOIR DE SUCCESSEURS*/
	int *successeurs;
	Noeud *noeud = malloc(sizeof(Noeud));
	if( !noeud )
	{
		printf("Échec de l'allocation du noeud.\n");
		exit(EXIT_FAILURE);
	}
	if(nombreSuccesseurs > 0)
	{
		successeurs = malloc(nombreSuccesseurs * sizeof(int));
		if( !successeurs )
		{
			printf("Échec de l'allocation des successeurs du noeud. (nombre de successeurs : %d)\n", nombreSuccesseurs);
			exit(EXIT_FAILURE);
		}
	}
	noeud->successeurs = successeurs;
	return noeud;
}

Noeud* creerNoeud(TypeNoeud type, int indice, unsigned int x, unsigned int y, int nombreSuccesseurs, int *successeurs)
{
	Noeud *noeud = malloc(sizeof(Noeud));
	if( !noeud )
	{
		printf("Échec de l'allocation du noeud. (noeud d'indice %d)\n", indice);
		exit(EXIT_FAILURE);
	}
	noeud->type = type;
	noeud->indice = indice;
	noeud->x = x;
	noeud->y = y;
	noeud->successeurs = successeurs; /*Indexation des successeurs (vecteur déjà alloué). */
	return noeud;
}

void libererNoeud(Noeud *noeud)
{
	if(noeud->successeurs > 0) /*Si le noeud à des successeurs.*/
		free(noeud->successeurs);
	free(noeud);
}

Arbre* allouerArbre(int nombreNoeuds)
{
	Arbre *arbre = malloc(nombreNoeuds * sizeof(Noeud*));
	if( !arbre )
	{
		printf("Échec de l'allocation de l'arbre. (nombre de noeuds : %d)\n", nombreNoeuds);
		exit(EXIT_FAILURE);
	}
	return arbre;
}

void libererArbre(int nombreNoeuds, Arbre *arbre)
{
	int i;
	/*On libère l'arbre et tous ses noeuds.*/
	for(i=0; i<nombreNoeuds; i++)
	{
		free(arbre[i]);
	}
	free(arbre);
}

Arbre* indexerArbre(int nombreNoeuds, Arbre *listeNoeuds)
{
	/*Permettre d'accéder au ième noeud de l'arbre par arbre[i].*/
}