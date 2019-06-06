#include "../include/lecture_fichier_carte.h"

/*On utilise les constantes relatives aux cartes définies dans "commun.h" */



bool lireCarte(FILE *fichierCarte, Carte *carte)
{
	bool valide; /*booléen*/

	/**** vérif code et version****/ 
	valide = lireVersion(fichierCarte, &(carte->version));

	if( !valide )
	{
		printf("Version de carte erroné, échec de lecture.\n");
		return false;
	}
	/***********/

	/***** Lecture des paramètres d'en-tête ******/
	valide = lireParametres(fichierCarte, carte);
	if( !valide )
	{
		printf("Paramètres carte erronés, échec de lecture.\n");
		return false;
	}
	/**********/

	/***** Lecture de du graphe des chemins *****/
	valide = lireChemins(fichierCarte, &(carte->nombreNoeuds), &(carte->chemins));

	if( !valide )
	{
		printf("Formattage graphe carte erroné, échec de lecture.\n");
		return false;
	}
	/***********/

	/** étape supplémentaire : lister les entrées (pour vagues de monstres) et les sorties **/
	//extraireEntreeSortie(&indicesEntrees, &nombreEntrees, &indiceSortie, nombreNoeuds, chemins);

	
	valide = extraireEntreesSorties(&(carte->indicesEntrees), &(carte->nombreEntrees), &(carte->indicesSorties), &(carte->nombreSorties), carte->nombreNoeuds, carte->chemins);
	if( !valide )
	{
		printf("Entrées / Sorties invalides, échec de lecture.\n");
		return false;
	}
	/********** Prochaine étape : vérifs -> verif_carte.c ***********/

	return valide;
}


bool lireVersion(FILE *fichierCarte, int *version)
{
	char codeCarte[MAX_TAILLE_CODE];
	if ( !lireChaine(fichierCarte, "Version", 0, codeCarte) )
		return false;
	if ( !lireEntier(fichierCarte, "Version", 0, version) )
		return false;
	if( strcmp(CODE_PROJET, codeCarte) != 0 ) /*Si le code ne correspond pas.*/
	{
		printf("Version -- code carte incorrect %s : on attend %s.\n", codeCarte, CODE_PROJET);
		return false;
	}
	else
		if( *version <= 0 || *version > NB_VERSIONS ) /*Cas de la version non supportée.*/
		{
			printf("Version --  version %d non supportée : support jusqu'à la version %d.\n", *version, NB_VERSIONS);
			return false;
		}
	sautLigne(fichierCarte);
	return true;
}

bool lireParametres(FILE *fichierCarte, Carte *carte)
{
	int i, couleur;
	int c, e;
	bool valide, alireOption=false;

	/* alias */
	int version = carte->version;

	long int pos;

	char chaineMotClef[MAX_TAILLE_MOTCLEF];
	MotClef MC_lu;
	OPT_MotClef OPT_lue;
	int aEteLu[ NB_PARAM_PAR_VERSION[version-1] ]; /*tableau de booléen qui permettra de savoir si on a lu les mot-clefs*/

	for(i=0; i < NB_PARAM_PAR_VERSION[version-1]; i++) /*amorcé à 0*/
		aEteLu[i] = 0;
	/***** Lecture des paramètres d'en-tête ******/
	for(i=0; i < NB_PARAM_PAR_VERSION[version-1] + NB_PARAM_OPTION; i++) /*condition correspond aux nombre de paramètres attendus pour la version*/
	{
		pos = ftell(fichierCarte);
		if( fgetc(fichierCarte) == CODE_COM )
		{
			/*gestion des commetaires ?*/
			/*On ignore le commentaire, on passe à la ligne suivante*/
			sautLigne(fichierCarte);
			i--; /*on  décrémente le compteur pour qu'il revienne à sa valeur initiale au début de la prochaine itération*/

		}
		else 
		{
			/*on revient sur nos pas*/
			fseek(fichierCarte, pos, SEEK_SET);
			/*et on recommence*/
			pos = ftell(fichierCarte);
			if ( !lireChaine(fichierCarte, "Paramètre", i, chaineMotClef) )
				return false;
			if( atoi(chaineMotClef) )
			{
				/* On a atteint les noeuds
				* on revient sur nos pas et on passe à la suite.
				*/
				fseek(fichierCarte, pos, SEEK_SET);
				break;
			}
			MC_lu = correspondanceMotClef(chaineMotClef, version);

			if( MC_lu != -1 && aEteLu[MC_lu] )
			{
				/* si on avait déjà eu lu ce mot-clef, on  renvoie une erreur.*/
				printf("Paramètre -- Ligne n°%d, redéfinition du mot-clef %s.\n", i, MOTCLEFS[MC_lu]);
				return false;
				
			}
			switch( MC_lu )
			{
				case MC_carte :
					if ( !lireChaine(fichierCarte, "Paramètre", i, carte->nomImage) )
						return false;
					break;
				case MC_energie :
					/*fread(&energie, sizeof(int), 1, fichierCarte); ne fonctionnera pas*/
					if ( !lireEntier(fichierCarte, "Paramètre", i, &e) )
						return false;
					if( e < 0 )
					 {
					 	printf("Paramètre -- Ligne n°%d, valeur d'énergie incorrecte %d : doit être positive.\n", i, e);
						return false;
					 }
					 carte->energie = e;
					break;
				case MC_chemin : case MC_noeud : case MC_construct : case MC_in : case MC_out :
					/*Dans ce cas-là, MC_lu est compris entre 0 et le nombre de params de couleurs - 1, et les indices correspondent à couleurClef et MOTCLEFS.*/
					for(couleur=0; couleur < NB_COULEURS; couleur++)
					{
						if ( !lireEntier(fichierCarte, "Paramètre", i, &c) )
							return false;
						if( c < 0 || c > MAX_VAL_COULEUR)
						{
							printf("Paramètre -- Ligne n°%d, valeur couleur incorrecte %d : maximum à %d.\n", i, c, MAX_VAL_COULEUR);
							return false;
						}
						carte->couleurClef[MC_lu][couleur] = c;
					}
					break;
				default :
					/* on doit vérifier s'il s'agit d'une option */
					alireOption = true;
					break;
			}
			if( alireOption )
			{
				OPT_lue = correspondanceOption(chaineMotClef);
				switch( OPT_lue )
				{
					case OPT_arrierePlan :
						if( !lireChaine(fichierCarte, "Paramètre", i, carte->nomArrierePlan) )
							return false;
						/* la carte possède un arrière-plan */
						carte->possedeArrierePlan = true;
						break;
					case -1 : /*Valeur de renvoi pour mot-clef non reconnu*/
						printf("Paramètre -- Ligne n°%d, mot-clef non reconnu : %s (version %d).\n", i, chaineMotClef, version);
						return false;
					default :
						printf("Vous ne devriez pas être ici !\n");
						exit(EXIT_FAILURE);

				}
				/* ON OUBLIE PAS DE RÉINITIALISER */
				alireOption = false;
				/* ET ON N'A PAS LU DE MOT-CLEF OBLIGATOIRE
				* DONC ON N'INCRÉMENTE PAS
				*/
				i--;
			}
			else
				aEteLu[MC_lu] = 1; /*On a lu le mot-clef sans erreurs*/
			/*On passe à la ligne suivante*/
			sautLigne(fichierCarte);
		}

	}
	valide = true;
	for(i=0; i < NB_PARAM_PAR_VERSION[version-1]; i++) /*On vérifie qu'on a lu tous les mot-clefs nécessaires à la version.*/
	{
		if( !aEteLu[i] )
		{
			if( valide )
				printf("Paramètre -- Mot-clef(s) manquant(s) :");
			printf("%s, ", MOTCLEFS[i]);
			valide = false;
		}
	}
	if( !valide )
		printf("\n");
	return valide;
}

bool lireChemins(FILE *fichierCarte, int *nombreNoeuds, Graphe **chemins)
{
	int k;
	int indice;
	int type;
	int x, y;
	Point *coord;

	Graphe_MatAdj *grapheMat; /*graphe par matrice d'adjacence*/

	int indiceSuccesseur;

	bool entreeExiste=false, sortieExiste=false; /*On peut vérifier ici qu'il existe une zone d'entrée et de sortie*/
	
	if ( !lireEntier(fichierCarte, "Graphe", 0, nombreNoeuds) )
		return false; /*du nombre de noeuds*/
	if(*nombreNoeuds < NB_MIN_NOEUDS) /*Il faut au moins une entrée et une sortie, donc deux*/
	{
		printf("Chemins erronés, trouvé %d noeuds : au moins %d noeuds nécessaires.\n", *nombreNoeuds, NB_MIN_NOEUDS);
		return false;
	}
	*chemins = allouerGraphe(*nombreNoeuds);
	grapheMat = allouerGrapheMatrice(*nombreNoeuds);

	/*On passe à la liste des noeuds en sautant de ligne.*/
	sautLigne(fichierCarte);
	
	for(k=0; k<*nombreNoeuds; k++)
	{
		/*Il se peut que le nombre de noeuds indiqué soit plus grand que le nombre de noeuds réellement présent.*/
		/*On vérifie donc que l'on n'est pas arrivé à la fin du fichier avant de continuer.*/
		if( feof(fichierCarte) )
		{
			printf("Graphe -- Nombre de noeuds erroné, indiqué %d noeuds : %d trouvés.\n", *nombreNoeuds, k);
			return false;
		}
		if( 4 != fscanf(fichierCarte, "%d %d %d %d", &indice, &type, &x, &y) )
		{
			printf("Graphe -- Ligne n°%d, erreur de formatage paramètres : doivent être entiers.\n", k);
			return false;
		}

		/**** Vérifs paramètres ***.
		/*indice du noeud est compris entre 0 et nombreNoeuds-1*/
		/* PAS NÉCESSAIREMENT, IL FAUDRA FAIRE UNE VÉRIF EN PLUS
		*/
		/* On impose pas d'ordre.*/
		if( indice < 0 || indice >= *nombreNoeuds )
		{
			printf("Graphe -- Ligne n°%d, indice %d invalide : doit être égal à la ligne.\n", k, indice);
			return false;
		}
		/*Le type doit être reconnu ( entre 1 et )*/
		if( type < 1 || type > NB_TYPES_NOEUD )
		{
			printf("Graphe -- Ligne n°%d, type n°%d non reconnu : doit être entre %d et %d.\n", k, type, 1, NB_TYPES_NOEUD);
			return false;
		}
		/*VÉRIF ENTRÉE-SORTIE*/
		if( type == entree )
			entreeExiste = true;
		if( type == sortie )
		{
			/* nous acceptons maintenant plusieurs sorties */
			sortieExiste = true;
		}
		/**/
		if( x < 0 || y < 0)
		{
			printf("Graphe -- Ligne n°%d, coordonnées (%d,%d) incorrectes : doivent être positives.\n", k, x, y);
			return false;
		}
		coord = creerPoint( (unsigned int) x, (unsigned int) y );
		(grapheMat->listeType)[indice] = type;
		(grapheMat->listeCoord)[indice] = coord;
			
		/*** Décompte du nombre de successeurs. ***/
		/*Si on est au dernier noeud, il faut s'assurer que l'on n'est pas arrivé à la fin du fichier.*/
		/*le SAUT DE LIGNE est automatisé par la condition de boucle.*/
		while( fgetc(fichierCarte) != CODE_SAUT_LIGNE || (k == *nombreNoeuds-1 && feof(fichierCarte)) )
		{
			fseek(fichierCarte,-sizeof(char),SEEK_CUR);
			/*On  avance dans le fichier, sans enregistrer les valeurs pour l'instant.*/
			if ( !lireEntier(fichierCarte, "Graphe", k, &indiceSuccesseur) )
				return false;
			if( indiceSuccesseur > *nombreNoeuds )
			{
				printf("Graphe -- Ligne n°%d, indice n°%d successeur %d incorrect : indice hors limite. (Nombre noeuds = %d)\n", k, indice, indiceSuccesseur, *nombreNoeuds);
				return false;
			}
			if( indiceSuccesseur == indice )
			{
				printf("Graphe -- Ligne n°%d, indice n°%d successeur %d incorrect : un noeud ne peut pas être son propre successeur.\n", k, indice, indiceSuccesseur);
				return false;
			}
			(grapheMat->adjacence)[indice][ indiceSuccesseur ] = 1; /*On a compté un successeur en plus.*/
		}
		/*À la fin de la boucle, on a le bon nombre de successeur (moyennant une vérification que l'on fera plus tard.)*/
		/*On remplit la matrice d'adjacence à partir du résultat.*/
	}
	/*afficherMatriceCarree( *nombreNoeuds, grapheMat->adjacence );*/
	if( !entreeExiste | !sortieExiste )
	{
		printf("Graphe -- Chemins invalides : non présence d'un couple entrée-sortie.\n");
		return false;
	}
	*chemins = matriceVersListeGraphe( *nombreNoeuds, grapheMat );
	/*On n'aura plus besoin de notre graphe sous forme de matrice d'adjacence, on le libère.
	*On libère entre autres la liste de points, mais pas les points eux-mêmes,
	*Qu'on passe ensuite aux noeuds*/
	libererGrapheMatrice(*nombreNoeuds, grapheMat, false);
	return true;
}


MotClef correspondanceMotClef(char motClef[], int version)
{
	int i;
	for(i=0; i<NB_PARAM_PAR_VERSION[version-1]; i++)
	{
		if( !strcmp(motClef, MOTCLEFS[i]) )
		{
			return (MotClef) i;
		}
	}
	return -1; /*valeur -1 traitée comme un échec*/
}

OPT_MotClef correspondanceOption(char option[])
{
	int i;
	for( i=0; i<NB_PARAM_OPTION; i++ )
	{
		if( !strcmp(option, OPT_MOTCLEFS[i]) )
		{
			return (OPT_MotClef) i;
		}
	}
	return -1; /* idem */
}

void sautLigne(FILE *fichierCarte)
{
	while( fgetc(fichierCarte) != CODE_SAUT_LIGNE || feof(fichierCarte) )
	{
					/*rien à faire ici */
	}
}

int lireEntier(FILE *fichierCarte, char position[],int ligne, int *e)
{
	int format = fscanf(fichierCarte, "%d", e);
	if( !format )
	{
		printf("%s -- Ligne n°%d, erreur de formatage : entier attendu.\n", position, ligne);
		return 0;	
	}
	return format;
}

int lireChaine(FILE *fichierCarte, char position[],int ligne, char chaine[])
{
	int format = fscanf(fichierCarte, "%s", chaine);
	if( !format )
	{
		printf("%s -- Ligne n°%d, erreur de formatage : chaîne de caractères attendue.\n", position, ligne);
		return 0;
	}
	return format;
}