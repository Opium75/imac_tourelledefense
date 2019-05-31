#include "../include/parcours.h"

int* plusCourtChemin(Monstre *monstre, Carte *carte, Cite *cite, int *nombreEtapes)
{
	int indice, compteurEtapes, j, k;
	int *indicesPrecedents, *parcours;
	Noeud *noeud0 = monstre->depart;
	int distanceMin, indiceSortieMin, nombreEtapesMin, d;
	/* On récupère les indices des précédents par l'algorithme de dijkstra */
	/* L'algorithme déterminera la sortie la plus proche ? */
	int *distances = creerVecteurEntier(carte->nombreNoeuds, -1);
	indicesPrecedents = dijkstra(noeud0, carte->chemins, carte->nombreNoeuds, cite->listeTour, distances);
	/* On reconstruit le parcours à effectuer
	* à noter que (i <= nombreNoeuds) ne doit pas se produire
	* cela signifirait que le noeud n'admet pas de chemins vers la sortie
	*/
	distanceMin = -1;
	indiceSortieMin = -1;
	nombreEtapesMin = -1;
	/* recherche de l'indice effectif de sortie */
	int indiceEntree = correspondanceIndicesEntrees(noeud0->indice, carte->indicesEntrees, carte->nombreEntrees);
	/* */
	for( k=0; k<carte->nombreSorties[indiceEntree]; k++ )
	{
		/* est-ce que cette sortie est la bonne ? */
		indice = carte->indicesSorties[indiceEntree][k];
		///////////////
		compteurEtapes = 0;
		d = 0;
		/* on commence par compter le nombre d'étapes */
		while( indice != noeud0->indice && (compteurEtapes < carte->nombreNoeuds) )
		{
			indice = indicesPrecedents[indice];
			d += distances[indice];
			compteurEtapes++;
		}
		if( compteurEtapes >=  carte->nombreNoeuds )
		{
			printf("Plus court chemin -- Nombre d'étapes supérieur au nombre de noeuds !\n");
			exit(EXIT_FAILURE);
		}
		if(distanceMin == -1 || (d <= distanceMin ))
		{
			/* on a trouvé une sortie plus proche */
			indiceSortieMin = indice;
			nombreEtapesMin = compteurEtapes;
		}
	}
	//
	libererVecteurEntier(distances);
	//
	*nombreEtapes = nombreEtapesMin;
	parcours = malloc( nombreEtapesMin * sizeof(int) );
	if( !parcours )
	{
		printf("Plus court chemin -- Échec d'allocation dynamique du parcours. (nombre d'étapes : %d)\n", nombreEtapesMin);
		exit(EXIT_FAILURE);
	}
	indice = indiceSortieMin;
	for( j=0; j<nombreEtapesMin; j++ )
	{
		/* Le parcours est inversé par rapport aux précédents */
		parcours[nombreEtapesMin - 1 - j] = indice;
		indice = indicesPrecedents[indice];
	}
	libererVecteurEntier(indicesPrecedents);
	return parcours;
}


bool avancerMonstre(Monstre *monstre, clock_t deltaT, Carte *carte, Cite *cite)
{
	/* Un monstre avance de n unité de distances par secondes,
	* donc on calcule par rapport à la longueur du segment
	* sur lequel il se trouve, et sa progression sur celui-ci
	*/
	/* Renvoie vrai s'il a atteint le noeud de sortie. */
	bool estSorti = false;
	int d;
	/* on utilise des floattants ici pour éviter le repliement*/
	double parcourue, aParcourir, reste;
	d = calculerDistance(monstre->depart->coord, monstre->arrivee->coord);
	parcourue = (double)d*monstre->avancement;
	/* sur quel modèle le monstre va avancer ? */
	aParcourir = (deltaT*(monstre->vitesse))/(double)CLOCKS_PER_SEC;
	//printf("Et voilà : parcourue %lf distance %d vitesse %ld à parcourir %lf\n", parcourue, d, monstre->vitesse, aParcourir);
	if( parcourue + aParcourir >= d )
	{
		/* Si l'on dépasse le segment */
		reste = d - ( parcourue + aParcourir );
		estSorti = changerSegmentMonstre(monstre, reste, carte, cite);
		monstre->modif_INDIC_mem = cite->modif_INDIC;
	}
	else
	{
		/* le monstre avance */
		monstre->avancement = (parcourue + aParcourir)/(double)d;
		
	}
	return estSorti;
}

bool changerSegmentMonstre(Monstre *monstre, double reste, Carte *carte, Cite *cite)
{
	int nouveauNombreEtapes;
	if( monstre->modif_INDIC_mem == cite->modif_INDIC )
	{
		/* si l'indicateur d'état est le même
		* que celui enregistré la dernière fois,
		* pas besoin de recalculer :
		* on avance sur le parcours déjà enregistré.
		*/
		monstre->indiceEtape++;

	}
	else
	{
		/* on est contraint de recalculer le parcours */
		libererVecteurEntier(monstre->parcours);
		monstre->parcours = plusCourtChemin(monstre, carte, cite, &nouveauNombreEtapes);
		
		monstre->indiceEtape = 0;
		monstre->nombreEtapes = nouveauNombreEtapes;
		
	}
	if( monstre->indiceEtape == monstre->nombreEtapes )
	{
		/* le monstre est arrivé à la sortie, prudence !! */
		/* que faire dans ce cas-là ?
		* bon pour l'instant, on se contente de renvoyer un booléen.
		*/
		monstre->etat = estSorti;
		return true;
	}
	/* on passe à l'étape suivante */
	monstre->depart = monstre->arrivee;
	monstre->arrivee = (carte->chemins)[  monstre->parcours[monstre->indiceEtape] ];
	
	/* on calcule le reste ? */
	/* on va pas trop s'embêter pour l'instance,
	* on considère qu'il est négligeable.
	*/
	monstre->avancement = 0;
	/* le monstre n'est pas encore sorti */
	return false;
}


int* dijkstra(Noeud *noeud0, Graphe *chemins, int nombreNoeuds, Tour *listeTour, int *distances)
{
	int j, indiceMin, d;
	Noeud *noeud, *voisin;
	/// IL FAUDRA RENVOYER LES DISTANCES ET CALCULER DANS calculerParcour
	/** Créations des tableaux utilisés */
	
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
			d = (distances[noeud->indice] == -1) ? -1 : distances[noeud->indice] + calculerDistancePonderee(noeud, voisin, listeTour);
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
	/* si on ne fait que calculer les distances,
	* on renvoie pointeur nul
	*/
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

int calculerDistancePonderee(Noeud *noeud, Noeud *voisin, Tour *listeTour )
{
	int compteurToursPortee=0;
	int distanceTour;
	int distanceNoeud;
	int distancePonderee;
	/* calcul du nombre de tours dont le segment est à la portée */
	Tour *tour = listeTour;
	while( tour )
	{
		//printf("ON RECALCULE LE CHEMIN LE PLUS COURT !!!!\n");
		distanceTour = calculerDistanceSegment( tour->coord, noeud->coord, voisin->coord );
		if( distanceTour <= (int)tour->portee )
			compteurToursPortee++;
		tour = tour->suivante;
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
			if( !trouve )
			{
				indiceMin = i;
				minimum = distances[i];
				trouve = true;
			}
			else if(  ( distances[i] != -1 ) && distances[i] < minimum )
			{
				indiceMin = i;
				minimum = distances[i];
			}
		}
	}
	
	/* Si tout a déjà été vérifié, on renvoie -1 */
	if( !trouve )
		indiceMin = -1;
	return indiceMin;
}