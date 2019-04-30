#include "../include/parcours.h"

int* plusCourtChemin(Monstre *monstre, Carte *carte, Tour **tours, int nombreTours, int *nombreEtapes)
{
	int indice, compteurEtapes, j;
	int *indicesPrecedents, *parcours;
	Noeud *noeud0 = monstre->depart;
	/* On récupère les indices des précédents par l'algorithme de dijkstra */
	indicesPrecedents = dijkstra(noeud0, carte->chemins, carte->nombreNoeuds, tours, nombreTours);
	/* On reconstruit le parcours à effectuer
	* à noter que (i <= nombreNoeuds) ne doit pas se produire
	* cela signifirait que le noeud n'admet pas de chemins vers la sortie
	*/
	indice = carte->indiceSortie;
	compteurEtapes = 0;
	/* on commence par compter le nombre d'étapes */
	while( indice != noeud0->indice && (compteurEtapes < carte->nombreNoeuds) )
	{
		indice = indicesPrecedents[indice];
		compteurEtapes++;
	}
	if( compteurEtapes >=  carte->nombreNoeuds )
	{
		printf("Plus court chemin -- Nombre d'étapes supérieur au nombre de noeuds !\n");
		exit(EXIT_FAILURE);
	}
	parcours = malloc( compteurEtapes * sizeof(int) );
	if( !parcours )
	{
		printf("Plus court chemin -- Échec d'allocation dynamique du parcours. (nombre d'étapes : %d)\n", compteurEtapes);
		exit(EXIT_FAILURE);
	}
	indice = carte->indiceSortie;
	for( j=0; j<compteurEtapes; j++ )
	{
		/* Le parcours est inversé par rapport aux précédents */
		parcours[compteurEtapes-1 - j] = indice;
		indice = indicesPrecedents[indice];
		
	}
	libererVecteurEntier(indicesPrecedents);
	*nombreEtapes = compteurEtapes;
	return parcours;
}

int* dijkstra(Noeud *noeud0, Graphe *chemins, int nombreNoeuds, Tour **tours, int nombreTours)
{
	int j, indiceMin, d;
	Noeud *noeud, *voisin;
	
	/** Créations des tableaux utilisés */
	int *distances = creerVecteurEntier(nombreNoeuds, -1);
	int *listeVerifies = creerVecteurEntier(nombreNoeuds, 0);
	/* Liste chaînée qui décrira le résultat */
	int *indicesPrecedents = creerVecteurEntier(nombreNoeuds, -1);

	/** Traitement **/
	distances[noeud0->indice] = 0;
	noeud = noeud0;
	while( noeud )
	{
		listeVerifies[noeud->indice] = true;
		for( j=0; j<noeud->nombreSuccesseurs; j++ )
		{
			voisin = noeud->successeurs[j];
			d = distances[noeud->indice] == -1 ? -1 : distances[noeud->indice] + calculerDistancePonderee(noeud, voisin, tours, nombreTours);
			if(  d != -1 && ( d < distances[voisin->indice] || distances[voisin->indice] == -1) )
			{
				distances[voisin->indice] = d;
				indicesPrecedents[voisin->indice] = noeud->indice;
			}
		}
		indiceMin = indiceMinDistance(listeVerifies, distances, nombreNoeuds);
		if( indiceMin == -1)
			noeud = NULL;
		else
			noeud = chemins[indiceMin];
	}
	/** Libération des tableaux **/
	libererVecteurEntier(listeVerifies);
	libererVecteurEntier(distances);

	return indicesPrecedents;
}

void afficherVecteur(int *vecteur, int taille)
{
	int k;
	printf("(");
	for(k=0; k<taille; k++)
	{
		printf("%d ",vecteur[k]);
	}
	printf(")\n");
}

int calculerDistancePonderee(Noeud *noeud, Noeud *voisin, Tour **tours, int nombreTours)
{
	int k;
	int compteurToursPortee=0;
	int distanceTour;
	int distanceNoeud;
	int distancePonderee;
	/* calcul du nombre de tours dont le segment est à la portée */
	for( k=0; k<nombreTours; k++ )
	{
		distanceTour = calculerDistanceSegment( (tours[k])->coord, noeud->coord, voisin->coord );
		if( distanceTour <= (int)(tours[k])->portee )
			compteurToursPortee++;
	}
	distanceNoeud = calculerDistance(noeud->coord, voisin->coord);
	/* calcul de la longueur du segment 
	* pondérée par le nombre de tours à portée */
	distancePonderee = distanceNoeud*(compteurToursPortee + 1);
	return distancePonderee;
}

int indiceMinDistance(int *listeVerifies, int *distances, int nombreNoeuds)
{
	int i;
	int indiceMin, minimum;
	bool trouve = false;
	for( i=1; i<nombreNoeuds; i++ )
	{
		if( !listeVerifies[i] )
		{
			if( !trouve || ( ( distances[i] != -1 ) && distances[i] < minimum ) )
			{
				indiceMin = i;
				minimum = distances[i];
				trouve = true;
			}
		}
	}
	
	/* Si tout a déjà été vérifié, on renvoie -1 */
	if( !trouve )
		indiceMin = -1;
	return indiceMin;
}

int* creerDistances(int nombreNoeuds)
{
	int i;
	int* distances = malloc( nombreNoeuds * sizeof(int) );
	if( !distances )
	{
		printf("Distances -- Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	for( i=0; i<nombreNoeuds; i++ )
	{
		distances[i] = -1;
	}
	return distances;
}



void libererDistances(int *distances)
{
	free(distances);
}