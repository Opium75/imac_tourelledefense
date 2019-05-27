#include "../include/parcours.h"

int* plusCourtChemin(Monstre *monstre, Carte *carte, Cite *cite, int *nombreEtapes)
{
	int indice, compteurEtapes, j;
	int *indicesPrecedents, *parcours;
	Noeud *noeud0 = monstre->depart;
	/* On récupère les indices des précédents par l'algorithme de dijkstra */
	indicesPrecedents = dijkstra(noeud0, carte->chemins, carte->nombreNoeuds, cite->listeTour);
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


int* dijkstra(Noeud *noeud0, Graphe *chemins, int nombreNoeuds, Tour *listeTour)
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
			d = distances[noeud->indice] == -1 ? -1 : distances[noeud->indice] + calculerDistancePonderee(noeud, voisin, listeTour);
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