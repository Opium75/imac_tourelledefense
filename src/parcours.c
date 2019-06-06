#include "../include/parcours.h"

int* plusCourtChemin(Monstre *monstre, Carte *carte, Cite *cite, int *nombreEtapes)
{
	int indice, compteurEtapes, j, k;
	int *indicesPrecedents, *parcours;
	Noeud *noeud0;

	int *indicesSortiesNoeud;
	int nombreSortiesNoeud;

	int entierProba;

	Noeud *noeud;
	int distanceMin, indiceSortieMin;
	/* On récupère les indices des précédents par l'algorithme de dijkstra */
	/* L'algorithme déterminera la sortie la plus proche ? */
	int *distances = creerVecteurEntier(carte->nombreNoeuds, -1);
	/*** ***/
	/* SI LE MONSTRE N'EST PAS ENCORE DÉPLOYÉ, IL N'A PAS D'ARRIVÉE
	* ET ON DOIT CACULER À PARTIR DE SON DÉPART
	* MAIS SI AU CONTRAIRE IL EST EN MOUVEMENT,
	* IL EST ENCORE SUR LE SEGMENT QU'IL VIENT DE PARCOURIR
	* ON CALCULE DONC À PARTIR DE SON ARRIVÉE
	*/
	if( !monstre->arrivee )
		noeud0 = monstre->depart;
	else
		noeud0 = monstre->arrivee;
	/*** ***/
	indicesPrecedents = dijkstra(noeud0, carte->chemins, carte->nombreNoeuds, cite->listeTour, distances);
	//afficherVecteurEntier(indicesPrecedents, carte->nombreNoeuds);
	//afficherVecteurEntier(distances, carte->nombreNoeuds);
	/* On reconstruit le parcours à effectuer
	* à noter que (i <= nombreNoeuds) ne doit pas se produire
	* cela signifirait que le noeud n'admet pas de chemins vers la sortie
	*/
	/** Il faut retrouver un noeud d'entrée (par nécessairement celui dont provient le monstre)
	** pour connaître les sorties accessibles depuis le noeud actuel du montre.
	** En fait, le monstre ne peut pas accéder à toutes ces sorties
	** mais le calcul des précédents piochera la sortie parmi ces sorties-là.
	**/

	/** ON RÉCUPÈRE LES SORTIES ACCESSIBLES PAR LE MONSTRE **/
	extraireSorties(noeud0, &indicesSortiesNoeud, &nombreSortiesNoeud, carte->nombreNoeuds);
	/** **/

	distanceMin = -1;
	indiceSortieMin = -1;

	/* déterminer l'indice de la sortie la plus proche (minimum des distances) */
	/** AVEC PROBABILITÉ DE NE PAS LA PRENDRE, ET UNE AUTRE AU HASARD **/
	entierProba = rand()%INVERSE_PROBA_ECART;
	if( !entierProba )
	{
		indiceSortieMin = indicesSortiesNoeud[rand()%nombreSortiesNoeud];
	}
	else
	{
		for( k=0; k<nombreSortiesNoeud; k++ )
		{
			indice = indicesSortiesNoeud[k];
			if(distanceMin == - 1 || (
									distances[indice] != -1 && distances[indice] < distanceMin 
									)
				)
			{
				indiceSortieMin = indice;
				distanceMin = distances[indice];
			}
		}
	}
	//
	libererVecteurEntier(indicesSortiesNoeud);
	libererVecteurEntier(distances);
	/* calcul du nombre d'étapes */
	indice = indiceSortieMin;
	compteurEtapes = 0;
	/* on ne va pas juqu'au noeud d'entrée, donc noeud0 et pas noeud*/
	while( indice != noeud0->indice && (compteurEtapes < carte->nombreNoeuds) )
	{
		indice = indicesPrecedents[indice];
		compteurEtapes++;
	}
	//
	*nombreEtapes = compteurEtapes;
	parcours = malloc( (*nombreEtapes) * sizeof(int) );
	if( !parcours )
	{
		printf("Plus court chemin -- Échec d'allocation dynamique du parcours. (nombre d'étapes : %d)\n", *nombreEtapes);
		exit(EXIT_FAILURE);
	}
	indice = indiceSortieMin;
	for( j=0; j<*nombreEtapes; j++ )
	{
		/* Le parcours est inversé par rapport aux précédents */
		parcours[*nombreEtapes - 1 - j] = indice;
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
	aParcourir = (double)monstre->vitesse*(deltaT/(double)CLOCKS_PER_SEC);
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
	if( monstre->modif_INDIC_mem == cite->modif_INDIC
		|| monstre->indiceEtape == monstre->nombreEtapes-1 )
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
		listeVerifies[noeud->indice] = 1;
		if( distances[noeud->indice] != -1 )
		{
			for( j=0; j<noeud->nombreSuccesseurs; j++ )
			{
				voisin = noeud->successeurs[j];
				d = distances[noeud->indice] + calculerDistancePonderee(noeud, voisin, listeTour);
				if(  (d < distances[voisin->indice]) || (distances[voisin->indice] == -1) )
				{
					distances[voisin->indice] = d;
					indicesPrecedents[voisin->indice] = noeud->indice;
				}
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
	/* si on ne fait que calculer les distances,
	* on renvoie pointeur nul
	*/
	return indicesPrecedents;
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
	for( i=0; i<nombreNoeuds; i++ )
	{
		if( !listeVerifies[i] )
		{
			if( distances[i] != -1 )
			{
				if( !trouve )
				{
					indiceMin = i;
					minimum = distances[i];
					trouve = true;
				}
				else if( distances[i] < minimum )
				{
					indiceMin = i;
					minimum = distances[i];
				}
			}
			
		}
	}
	
	/* Si tout a déjà été vérifié, on renvoie -1 */
	if( !trouve )
		indiceMin = -1;
	return indiceMin;
}