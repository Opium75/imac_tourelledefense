#include "../include/vague.h"

Vague* creerVague(unsigned char niveau, Carte *carte)
{
	int nombreEntrees, nombreMonstres;
	Noeud **entrees;
	Monstre **monstres;
	Vague *vague = malloc( sizeof(Vague) );
	if( !vague ) 
	{
		printf("Vague -- Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}

	vague->niveau = niveau;
	
	vague->tempsLimite = calculerTempsLimite(niveau);
	vague->tempsPause = calculerTempsPause(niveau);

	/*On récupère le noeud de sortie */
	vague->sortie = carte->chemins[carte->indiceSortie];

	/* on initialise la graine pour le tirage pseudo-aléatoire */
	srand(time(NULL));
	/* On calcule d'abord les entrées, puis les monstres grâce aux entrées */
	entrees = calculerEntreesVague(&nombreEntrees, carte);
	monstres = creerMonstresVague(niveau, &nombreMonstres, entrees, nombreEntrees);

	vague->entrees = entrees;
	vague->monstres = monstres;

	vague->nombreEntrees = nombreEntrees;
	vague->nombreMonstres = nombreMonstres;

	vague->suivante = NULL;
	
	return vague;
}

void lancerVague(Vague *vague, unsigned char niveau, Carte *carte, Tour **tours, int nombreTours)
{
	int k;
	Monstre *monstre;
	int nombreEtapes;
	int *parcours;
	for( k=0; k<vague->nombreMonstres; k++ )
	{
		monstre = (vague->monstres)[k];
		/* calcul du plus court chemi pour le monstre */
		parcours = plusCourtChemin(monstre, carte, tours, nombreTours, &nombreEtapes);
		monstre->parcours = parcours;
		monstre->nombreEtapes = nombreEtapes;
		monstre->indiceEtape = 0;
		/* */
		monstre->arrivee = carte->chemins[parcours[0]];
		/* afficherVecteur(parcours, nombreEtapes); */
	}
	afficherVague(vague);
}

void ajouterVague(Vague *vague, Chaine chaine)
{
	while( chaine )
		chaine = chaine->suivante;
	chaine->suivante = vague;
}

int longueurChaine(Chaine chaine)
{
	int longueur = 0;
	while( chaine)
	{
		longueur++;
		chaine = chaine->suivante;
	}
	return longueur;
}

void enleverPremiereVague(Chaine *chaine)
{
	Vague *suivante = (*chaine)->suivante;
	libererVague(*chaine);
	*chaine = suivante;
}

void enleverVague(int indiceVague, Chaine chaine)
{
	/* Pour tout indice >= 1 */
	/** ATTENTION s'assurer que l'indice n'est pas
	* supérieur à la taille de la chaine.
	*/
	int j;
	Vague *celleDapres;
	for( j=0; j<indiceVague-1; j++ )
		chaine = chaine->suivante;
	celleDapres = (chaine->suivante)->suivante;
	libererVague(chaine->suivante);
	chaine->suivante = celleDapres;
}




void afficherVague(Vague *vague)
{
	int indice;
	printf("--- AFFICHAGE VAGUE ---\n");
	/*** Niveau ***/
	printf("Niveau : %hhu\n", vague->niveau);
	/*** Temps ***/
	printf("Temps limite : %u s\n", vague->tempsLimite);
	printf("Temps pause : %u s\n", vague->tempsPause);
	/*** Entrées ***/
	printf("Entrées : \n");
	afficherGraphe(vague->entrees, vague->nombreEntrees);
	/*** Sortie ***/
	printf("Sortie : \n");
	afficherNoeud(vague->sortie);

	/*** Monstres ***/
	printf("Monstres :\n");
	for( indice=0; indice<vague->nombreMonstres; indice++ )
	{
		afficherMonstre(vague->monstres[indice]);
	}
	/*** Suivante ***/
	/*printf("Suivante : \n");*/
	printf("--- FIN AFFICHAGE ---\n");
}

int calculerNombreEntrees(int nombreEntreesTotal)
{
	int nombreEntrees = rand()%(nombreEntreesTotal)+1;
	return nombreEntrees;
}

int calculerNombreMonstres(unsigned char niveau)
{
	int nombreMonstres = (rand()%TAILLE_VAGUE_BASE)*(niveau+1) + 1;
	return nombreMonstres;
}

unsigned int calculerTempsLimite(unsigned char niveau)
{
	unsigned int tempsLimite = (unsigned int)TEMPS_LIMITE_BASE*niveau;
	return tempsLimite;
}

unsigned int calculerTempsPause(unsigned char niveau)
{
	unsigned int tempsPause = (unsigned int)(TEMPS_PAUSE_BASE*( 1./(1 + niveau) ) );
	return tempsPause;
}


Noeud** calculerEntreesVague(int *nombreEntrees, Carte *carte)
{
	/* tirages uniformes avec rand() */
	int k, indice;
	srand(time(NULL));
	int nombreEntreesVague = calculerNombreEntrees(carte->nombreEntrees);
	Noeud **entrees = malloc( nombreEntreesVague * sizeof(Noeud*) );
	if( !entree )
	{
		printf("Erreur, échec d'allocation dynamique, pour une vague !\n");
		exit(EXIT_FAILURE);
	}
	for( k=0; k<nombreEntreesVague; k++ )
	{
		indice = carte->indicesEntrees[rand()%carte->nombreEntrees];
		entrees[k] = carte->chemins[indice];
	}
	*nombreEntrees = nombreEntreesVague;
	return entrees;
}


Monstre** creerMonstresVague(unsigned char niveau, int *nombreMonstres, Noeud **entrees, int nombreEntrees)
{
	int i, nombre;
	TypeMonstre type;
	Noeud *entree;
	nombre = calculerNombreMonstres(niveau); 
	Monstre **monstres = malloc( nombre * sizeof(Monstre*) );
	if( !monstres ) 
	{
		printf("Erreur, échec d'allocation dynamique, pour une liste de monstres ! (nombre de monstres : %d)\n", nombre);
		exit(EXIT_FAILURE);
	}
	for( i=0; i<nombre; i++)
	{
		type = calculerTypeMonstre();
		entree = calculerEntreeMonstre(entrees, nombreEntrees);
		monstres[i] = creerMonstre(type, entree);
	}
	*nombreMonstres = nombre;
	return monstres;
}

void libererMonstresVague(int nombreMonstres, Monstre **monstres)
{
	int i;
	for( i=0; i<nombreMonstres; i++)
	{
		libererMonstre(monstres[i]);
	}
	free(monstres);
}

void libereEntreesVague(int nombreEntreesVague, Noeud **entrees)
{
	/*On ne libére pas les noeuds, seulement le pointeur sur eux*/
	free(entrees);
}

void libererVague(Vague *vague)
{
	libererMonstresVague(vague->nombreMonstres, vague->monstres);
	/* libérer entrées ?*/
	libereEntreesVague(vague->nombreEntrees, vague->entrees);
	free(vague);
}