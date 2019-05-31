#include "../include/vague.h"

Vague* creerVague(unsigned char niveau, Carte *carte)
{
	int nombreEntrees, nombreMonstres;
	Noeud **entrees, ***sorties;
	int *nombreSorties;
	Monstre **monstres;
	Vague *vague = malloc( sizeof(Vague) );
	if( !vague ) 
	{
		printf("Vague -- Échec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}

	vague->niveau = niveau;

	vague->tempsPause = calculerTempsPause(niveau);
	vague->tempsPause_acc = 0;

	vague->tempsEntracte_acc = 0;

	/* on a pas encore déployé la vague */
	vague->etat = nonLancee;

	/* On calcule d'abord les entrées, puis les monstres grâce aux entrées */
	entrees = calculerEntreesVague(&nombreEntrees, carte);
	vague->entrees = entrees;
	vague->nombreEntrees = nombreEntrees;
	/*On récupère les noeuds de sortie correspondants*/
	sorties = calculerSortiesVague(entrees, nombreEntrees, &nombreSorties, carte);
	vague->sorties = sorties;
	vague->nombreSorties = nombreSorties;

	/**/
	monstres = creerMonstresVague(niveau, &nombreMonstres, entrees, nombreEntrees);
	vague->monstres = monstres;
	vague->nombreMonstres = nombreMonstres;

	vague->suivante = NULL;
	
	return vague;
}

Noeud*** calculerSortiesVague(Noeud **entrees, int nombreEntrees, int **nombreSortiesVague, Carte *carte)
{
	int k, j, indiceEntree;
	Noeud ***sorties = malloc( nombreEntrees * sizeof(Noeud**) );
	if( !sorties )
	{
		printf("Vague -- Échec d'allocation des sorties (nombre d'entrées : %d).\n", nombreEntrees);
		exit(EXIT_FAILURE);
	}
	int indicesSortie;
	*nombreSortiesVague = creerVecteurEntier(nombreEntrees,-1);
	for( k=0; k<nombreEntrees; k++ )
	{
		indiceEntree = correspondanceIndicesEntrees(entrees[k]->indice, carte->indicesEntrees, carte->nombreEntrees);
		(*nombreSortiesVague)[k] = carte->nombreSorties[indiceEntree];
		sorties[k] = malloc( (*nombreSortiesVague)[k] * sizeof(Noeud*) );
		if( !sorties )
		{
			printf("Vague -- Échec d'allocation des sorties (nombre de sorties : %d).\n", (*nombreSortiesVague)[k]);
			exit(EXIT_FAILURE);
		}
		//
		for( j=0; j<(*nombreSortiesVague)[k]; j++ )
		{
			sorties[k][j] = carte->chemins[ carte->indicesSorties[indiceEntree][j] ];
		}
	}
	return sorties;
}

void calculerParcours(Monstre *monstre, Carte *carte, Cite *cite)
{
	int nombreEtapes;
	int *parcours = plusCourtChemin(monstre, carte, cite, &nombreEtapes);
	monstre->parcours = parcours;
	monstre->nombreEtapes = nombreEtapes;
	monstre->indiceEtape = 0;
	monstre->arrivee = carte->chemins[parcours[0]];
}

void deployerMonstre(Monstre *monstre, Carte *carte, Cite *cite)
{
	/* calcul du plus court chemi pour le monstre */
	calculerParcours(monstre, carte, cite);
	monstre->etat = enMouvement;
}

void lancerVague(Vague *vague, Carte *carte, Cite *cite)
{
	int k;
	Monstre *monstre;
	/* on déploie le premier, on calcule pour le reste */
	monstre = (vague->monstres)[0];
	deployerMonstre(monstre, carte, cite);
	vague->etat = lancee;
}

int accesIndiceDeploiement(Vague *vague)
{
	int k=0;
	Monstre *monstre = vague->monstres[k];
	while( (k < vague->nombreMonstres) && (monstre->etat != enAttente) )
	{
		k++;
		monstre = vague->monstres[k];
	}
	if( k == vague->nombreMonstres )
	{
		/* la vague n'a plus de monstres à déployer
		* on renvoie une échec avec la valeur -1
		*/
		return -1;
	}
	else
		return k;
}


bool traitementChaine(Chaine *chaine, clock_t deltaT, Carte *carte, Cite *cite, unsigned char *niveau, int *pointage, int *argent)
{
	int k, j;
	bool  estSorti, conditionEntracte;
	Vague *vague;
	Monstre *monstre;
	/* les vagues de monstres */
	
	vague = *chaine;
	if( vague->etat == lancee )
	{
		if( deltaT + vague->tempsPause_acc > vague->tempsPause )
		{
			j = accesIndiceDeploiement(vague);
			if( j != -1 )
			{
				/* si tous les monstres n'ont pas été déployés */
				monstre = vague->monstres[j];
				deployerMonstre(monstre, carte, cite);
				
			}
			vague->tempsPause_acc = 0;
		}
		else
		{
			vague->tempsPause_acc += deltaT;
		}
		for( k=0; k<vague->nombreMonstres; k++ )
		{
			
			monstre = vague->monstres[k];
			if( monstre->etat == enMouvement )
			{
				estSorti = avancerMonstre(monstre, deltaT, carte, cite);

				if( estSorti )
				{
					/* le monstre attaque le joueur */
					attaquerJoueur(monstre, pointage, argent);
				}
			}
		}
		/* est-ce que la vague est terminée ?  */
		if( estTerminee(vague) )
		{
			//printf("Ouaou !\n");
			(*chaine) = (*chaine)->suivante;
			libererVague(vague, cite->listeTour);
			/*on monte de niveau */
			(*niveau)++;
			/* est-ce qu'on aura déjà créé la vague ? */
			if( !(*chaine) )
				*chaine = creerVague(*niveau, carte);
			/*return false;*/
		}
	}
	else
	{
		vague = *chaine;
		/* la vague était en attente jusqu'à maintenant
		* on vérifie que c'est toujours le cas
		*/
		conditionEntracte = ( calculerTempsSecondes(deltaT + vague->tempsEntracte_acc) > TEMPS_ENTRACTE );
		//printf("Ceci n'est pas un message : %lf ?\n", calculerTempsSecondes(deltaT +vague->tempsEntracte_acc));
		if( conditionEntracte )
		{
				lancerVague(vague, carte, cite);
				vague->tempsEntracte_acc = 0;
		}
		else
			vague->tempsEntracte_acc += deltaT;
	}
	/* ON OUBLIE PAS D'AUGMENTER LES ACCUMULATEURS AVANT*/
	
	return false;
}


bool estTerminee(Vague *vague)
{
	int k;
	Monstre *monstre;
	/* On parcours la liste des monstres en sens inverse
	* car les derniers monstres à être vaincus
	* seront probablement les derniers à être entrés  
	*/
	for( k=vague->nombreMonstres-1; k>=0; k-- )
	{
		monstre = (vague->monstres)[k];
		if( monstre->etat == enMouvement
			|| monstre->etat == enAttente )
			return false;
	}
	return true;
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
	while( chaine )
	{
		longueur++;
		chaine = chaine->suivante;
	}
	return longueur;
}

void supprimerVagueSuivante(Chaine chaine, ListeTour liste)
{
	Vague *celleDapres = (chaine->suivante)->suivante;
	libererVague(chaine->suivante, liste);
	chaine->suivante = celleDapres;
}

void supprimerPremiereVague(Chaine *chaine, ListeTour liste)
{
	Vague *suivante = (*chaine)->suivante;
	libererVague(*chaine, liste);
	*chaine = suivante;
}

void enleverVague(int indiceVague, Chaine chaine, ListeTour liste)
{
	/* Pour tout indice >= 1 */
	/** ATTENTION s'assurer que l'indice n'est pas
	* supérieur à la taille de la chaine.
	*/
	int j;
	Vague *celleDapres;
	for( j=0; j<indiceVague-1; j++ )
		chaine = chaine->suivante;
	/* s'il reste au moins un élément après
	* c-à-d qu'on enlève le dernier élément
	*/
	if( chaine->suivante )
		celleDapres = (chaine->suivante)->suivante;
	else
		celleDapres = NULL;
	libererVague(chaine->suivante, liste);
	chaine->suivante = celleDapres;
}

void libererChaine(Chaine chaine, ListeTour liste)
{
	Vague *vague;
	while( chaine )
	{
		vague = chaine;
		chaine = chaine->suivante;
		libererVague(vague, liste);
	}
}

void terminalVague(Vague *vague)
{
	int indice;
	printf("--- AFFICHAGE VAGUE ---\n");
	/** état **/
	printf("État : %s\n", ETAT_VAGUE[vague->etat] );
	/*** Niveau ***/
	printf("Niveau : %hhu\n", vague->niveau);
	/*** Temps ***/
	printf("Temps pause : %ld/%ld tours processeur\n", vague->tempsPause_acc, vague->tempsPause);
	/*** Entrées ***/
	printf("Entrées : \n");
	afficherGraphe(vague->entrees, vague->nombreEntrees);
	/*** Sortie ***/
	printf("Sortiew : \n");
	//afficherGraphe(vague->sortie, ...);

	/*** Monstres ***/
	printf("Monstres : (nombre = %d)\n", vague->nombreMonstres);
	for( indice=0; indice<vague->nombreMonstres; indice++ )
	{
		terminalMonstre(vague->monstres[indice]);
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

clock_t calculerTempsPause(unsigned char niveau)
{
	clock_t tempsPause = CLOCKS_PER_SEC/(TEMPS_PAUSE_BASE*( 1./(1 + niveau) ));
	return tempsPause;
}

double calculerTempsSecondes(clock_t temps)
{
	double tempsSecondes = temps/(double)CLOCKS_PER_SEC;
	return tempsSecondes;
}


Noeud** calculerEntreesVague(int *nombreEntrees,  Carte *carte)
{
	/* tirages uniformes avec rand() */
	int k, indice;
	int nombreEntreesVague = calculerNombreEntrees(carte->nombreEntrees);
	Noeud **entrees = malloc( nombreEntreesVague * sizeof(Noeud*) );
	if( !entree )
	{
		printf("Erreur, échec d'allocation dynamique pour une vague !\n");
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
		monstres[i] = creerMonstre(type, niveau, entree);
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

void libererEntreesVague(int nombreEntreesVague, Noeud **entrees)
{
	/*On ne libére pas les noeuds, seulement le pointeur sur eux*/
	free(entrees);
}

void libererSortiesVague(int nombreEntreesVague, Noeud ***sorties, int *nombreSorties)
{
	/* idem */
	int i;
	for( i=0; i<nombreEntreesVague; i++ )
	{
		free(sorties[i]);
	}
	free(nombreSorties);
}
void libererVague(Vague *vague, ListeTour liste)
{
	/* les tours abandonnent leur cible */
	reinitialiserCibles(liste);
	/* on libère les monstres */
	libererMonstresVague(vague->nombreMonstres, vague->monstres);
	/* libérer entrées ?*/
	libererEntreesVague(vague->nombreEntrees, vague->entrees);
	libererSortiesVague(vague->nombreEntrees, vague->sorties, vague->nombreSorties);
	free(vague);
}