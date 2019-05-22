#include "../include/graphe.h"

Graphe_MatAdj* allouerGrapheMatrice(int nombreNoeuds)
{
	TypeNoeud *listeType = malloc( nombreNoeuds * sizeof(TypeNoeud) );
	Graphe_MatAdj *grapheMat = malloc( nombreNoeuds * sizeof(Graphe_MatAdj)) ;
	if( !listeType )
	{
		printf("Échec de l'allocation de la matrice d'adjacence. (nombre de noeuds : %d)\n", nombreNoeuds);
		exit(EXIT_FAILURE);
	}
	ListePoint *listeCoord = allouerListePoint(nombreNoeuds);
	int **adjacence = allouerMatriceCarree(nombreNoeuds);
	grapheMat->adjacence = adjacence;
	grapheMat->nombreNoeuds = nombreNoeuds;
	grapheMat->listeType = listeType;
	grapheMat->listeCoord = listeCoord;
	return grapheMat;
}

void libererGrapheMatrice(int nombreNoeuds, Graphe_MatAdj *grapheMat, bool avecContenuPoint)
{
	free(grapheMat->listeType);
	libererListePoint(nombreNoeuds, grapheMat->listeCoord, avecContenuPoint);
	libererMatriceCarree(nombreNoeuds, grapheMat->adjacence);
}

Graphe* matriceVersListeGraphe(int nombreNoeuds, Graphe_MatAdj *grapheMat)
{
	int ligne, colonne;
	int nombreSuccesseurs, compteurSuccesseurs;
	Graphe *graphe = allouerGraphe(nombreNoeuds);
	Noeud *noeud;
	for(ligne=0; ligne<nombreNoeuds; ligne++)
	{
		
		nombreSuccesseurs=0;
		for(colonne=0; colonne<nombreNoeuds; colonne++)
		{
			if(  ((grapheMat->adjacence)[ligne][colonne]) )
				nombreSuccesseurs++;
		}
		noeud = creerNoeud( (grapheMat->listeType)[ligne], ligne, (grapheMat->listeCoord)[ligne], nombreSuccesseurs );
		graphe[ligne] = noeud;
	}
	for(ligne=0; ligne<nombreNoeuds; ligne++)
	{
		compteurSuccesseurs=0;
		for(colonne=0; colonne<nombreNoeuds; colonne++)
		{
			if( ((grapheMat->adjacence)[ligne][colonne]) )
			{
				(graphe[ligne])->successeurs[compteurSuccesseurs] = graphe[colonne]; 
				compteurSuccesseurs+=1;
			}
		}
	}
	return graphe;
}

void extraireEntreesSortie(int **indicesEntrees, int *nombreEntrees, int *indiceSortie, int nombreNoeuds, Graphe *graphe)
{
	int i;
	int compteur = 0;

	*indicesEntrees = malloc( nombreNoeuds * sizeof(int) );
	if( !(*indicesEntrees) )
	{
		printf(" Entrées -- Échec d'allocation. (nombre de noeuds : %d)\n", nombreNoeuds);
		exit(EXIT_FAILURE);
	}	
	for( i=0; i<nombreNoeuds; i++ )
	{
		if( (graphe[i])->type == entree )
		{
			(*indicesEntrees)[compteur] = i;
			compteur++;
		}
		if( (graphe[i])->type == sortie )
			*indiceSortie = i;
	}
	*nombreEntrees = compteur;
	/* On se débarrasse de l'espace inutilisé */
	*indicesEntrees = realloc(*indicesEntrees, compteur * sizeof(int) );
	if( !(*indicesEntrees) )
	{
		printf(" Entrées -- Échec de réallocation. (nombre d'entrées : %d)\n", compteur);
		exit(EXIT_FAILURE);
	}
}

void libererIndicesEntrees(int* indicesEntrees)
{
	free(indicesEntrees);
}

Noeud* creerNoeud(TypeNoeud type, int indice, Point *coord, int nombreSuccesseurs)
{
	Noeud *noeud = malloc(sizeof(Noeud));
	if( !noeud )
	{
		printf("Échec de l'allocation du noeud. (noeud d'indice %d)\n", indice);
		exit(EXIT_FAILURE);
	}
	Noeud **successeurs = malloc( nombreSuccesseurs * sizeof(Noeud*) );
	if( !successeurs )
	{
		printf("Échec de l'allocation de la liste des successeurs. (noeud d'indice %d, nombre de successeurs %d)\n", indice, nombreSuccesseurs);
		exit(EXIT_FAILURE);
	}
	noeud->type = type;
	noeud->indice = indice;
	noeud->coord = coord;
	noeud->nombreSuccesseurs = nombreSuccesseurs;
	noeud->successeurs = successeurs;
	/*On ne touche pas ici à successeurs, on le calcule plus tard.*/
	return noeud;
}

void libererNoeud(Noeud *noeud)
{
	if( NULL != noeud->successeurs ) /*Si le noeud à des successeurs.*/
	{
		free(noeud->successeurs);
	}
	libererPoint(noeud->coord);
	free(noeud);
}

void afficherNoeud(Noeud *noeud)
{
	int j;
	printf("Noeud -> indice %d, type : %s, coord : (%u, %u)\n", noeud->indice, TYPENOEUD[noeud->type], noeud->coord->x, noeud->coord->y);
	printf("Noeud -> successeurs (nombre = %d) : ", noeud->nombreSuccesseurs);
	for(j=0; j < noeud->nombreSuccesseurs; j++)
	{
		printf("%d, ", (noeud->successeurs[j])->indice);
	}
	printf("\n");
}


Graphe* allouerGraphe(int nombreNoeuds)
{
	Graphe* graphe = malloc( nombreNoeuds * sizeof(Noeud*) );
	if( !graphe )
	{
		printf("Échec de l'allocation du graphe. (nombre de noeuds : %d)\n", nombreNoeuds);
		exit(EXIT_FAILURE);
	}
	return graphe;
}

void libererGraphe(int nombreNoeuds, Graphe *graphe)
{
	int i;
	/*On libère le graphe et tous ses noeuds.*/
	for(i=0; i<nombreNoeuds; i++)
	{
		libererNoeud(graphe[i]);
	}
	free(graphe);
}

void afficherGraphe(Graphe *graphe, int nombreNoeuds)
{
	int indice;
	printf("Graphe -> nombre de noeuds : %d\n", nombreNoeuds);
	for(indice=0; indice < nombreNoeuds; indice++)
	{
		afficherNoeud( graphe[indice] );
	}
	
}

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
	printf("Vecteur d'entiers -> (");
	for( i=0; i<taille; i++ )
	{
		printf("%d, ", vecteur[i]);
	}
	printf(")\n");
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