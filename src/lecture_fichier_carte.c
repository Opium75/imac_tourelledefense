#include "../include/lecture_fichier_carte.h"

/*On utilise les constantes relatives aux cartes définies dans "commun.h" */

bool lireCarte(FILE *fichierCarte, Carte *carte)
{
	bool valide; /*booléen*/
	int version;
	unsigned int energie; 
	char nomImage[MAX_TAILLE_NOM_FICHIER];
	unsigned char couleurClef[NB_COULEURS_CLEFS][NB_COULEURS];
	int nombreNoeuds;
	Graphe *chemins;

	/**** vérif code et version****/ 
	valide = lireVersion(fichierCarte, &version);

	if( !valide )
	{
		printf("Version de carte erroné, échec de lecture.\n");
		return false;
	}
	/***********/

	/***** Lecture des paramètres d'en-tête ******/
	valide = lireParametres(fichierCarte, version, &energie, carte->nomImage, carte->couleurClef);

	if( !valide )
	{
		printf("Paramètres carte erronés, échec de lecture.\n");
		return false;
	}
	/**********/

	/***** Lecture de du graphe des chemins *****/
	valide = lireChemins(fichierCarte, &nombreNoeuds, &chemins);

	if( !valide )
	{
		printf("Formattage graphe carte erroné, échec de lecture.\n");
		return false;
	}
	/***********/

	/*Remplissment des champs de la carte.*/
	carte->version = version;
	carte->energie = energie;
	carte->nombreNoeuds = nombreNoeuds;
	carte->chemins = chemins;

	/********** Bientôt dans vos foyer : vérifs ***********/

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

bool lireParametres(FILE *fichierCarte, int version, unsigned int *energie, char nomImage[],  unsigned char couleurClef[][NB_COULEURS])
{
	int i, couleur;
	int c, e;

	bool valide;

	char chaineMotClef[MAX_TAILLE_MOTCLEF];
	MotClef MC_lu;

	int aEteLu[ NB_PARAM_PAR_VERSION[version-1] ]; /*tableau de booléen qui permettra de savoir si on a lu les mot-clefs*/

	for(i=0; i < NB_PARAM_PAR_VERSION[version-1]; i++) /*amorcé à 0*/
		aEteLu[i] = 0;
	/***** Lecture des paramètres d'en-tête ******/
	for(i=0; i < NB_PARAM_PAR_VERSION[version-1]; i++) /*condition correspond aux nombre de paramètres attendus pour la version*/
	{
		if( fgetc(fichierCarte) == CODE_COM )
		{
			/*gestion des commetaire ?*/

			/*On ignore le commentaire, on passe à la ligne suivante*/
			sautLigne(fichierCarte);
			i--; /*on  décrémente le compteur pour qu'il revienne à sa valeur initiale au début de la prochaine itération*/

		}
		else 
		{
			/*on revient sur nos pas*/
			fseek(fichierCarte,-sizeof(char),SEEK_CUR);
			/*et on recommence*/
			if ( !lireChaine(fichierCarte, "Paramètre", i, chaineMotClef) )
				return false;
			MC_lu = correspondanceMotClef(chaineMotClef, version);
			if( MC_lu != -1 && aEteLu[MC_lu] ) /* si on avait déjà eu lu ce mot-clef, on  renvoie une erreur.*/
			{
				printf("Paramètre -- Ligne n°%d, redéfinition du mot-clef %s.\n", i, MOTCLEFS[MC_lu]);
				return false;
			}
			switch( MC_lu )
			{
				case MC_carte :
					if ( !lireChaine(fichierCarte, "Paramètre", i, nomImage) )
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
					 *energie = e;
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
						couleurClef[MC_lu][couleur] = c;
					}
					break;
				case -1 : /*Valeur de renvoi pour mot-clef non reconnu*/
					printf("Paramètre -- Ligne n°%d, mot-clef non reconnu : %s (version %d).\n", i, chaineMotClef, version);
					return false;
				default :
					printf("Vous ne devriez pas être ici !\n");
					exit(EXIT_FAILURE);
			}
			aEteLu[MC_lu]  = 1; /*On lu le mot-clef sans erreurs*/
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

	/*On passe à la liste des noeud en sautant de ligne.*/
	sautLigne(fichierCarte);
	
	for(k=0; k<*nombreNoeuds; k++)
	{
		/*Il se peut que le nombre de noeuds indiqué soit plus grand que le nombre de noeuds réellement présent.*/
		/*On vérifie donc que l'on n'est pas arrivé à la fin du fichier avant de continuer.*/
		if( feof(fichierCarte) )
		{
			printf("Nombre de noeuds erroné, indiqué %d noeuds : %d trouvés.\n", *nombreNoeuds, k);
			return false;
		}
		if( 4 != fscanf(fichierCarte, "%d %d %d %d", &indice, &type, &x, &y) )
		{
			printf("Graphe -- Ligne n°%d, erreur de formatage paramètres : doivent être entiers.\n", k);
			return false;
		}

		/**** Vérifs paramètres ***.
		/*indice du noeud est compris entre 0 et nombreNoeuds-1*/
		if( indice != k ) /* ( indice < 0 || indice >= *nombreNoeuds ) marchera aussi, mais on nous impose un ordre.*/
		{
			printf("Graphe -- Ligne n°%d, indice %d invalide : doit être égal à la ligne.\n", k, indice);
			return false;
		}
		/*Le type doit être reconnu ( entre 1 et )*/
		if( type < 1 || type > NB_TYPE_NOEUD )
		{
			printf("Graphe -- Ligne n°%d, type n°%d non reconnu : doit être entre %d et %d.\n", k, type, 1, NB_TYPE_NOEUD);
		}
		/*VÉRIF ENTRÉE-SORTIE*/
		if( type == entree )
			entreeExiste = true;
		if( type == sortie )
			sortieExiste = true;
		/**/
		if( x < 0 || y < 0)
		{
			printf("Graphe -- Ligne n°%d, coordonnées (%d,%d) incorrectes : doivent être positives.\n", k, x, y);
		}

		coord = creerPoint( (unsigned int) x, (unsigned int) y );
		(grapheMat->listeType)[indice] = type;
		(grapheMat->listeCoord)[indice] = coord;
			
		/*** Décompte du nombre de successeurs. ***/
		/*Si on est au dernier noeud, il faut s'assurer que l'on n'est pas arrivé à la fin du fichier.*/
		/*le SAUT DE LIGNE est automatisé par la conditon de boucle.*/
		while( fgetc(fichierCarte) != CODE_SAUT_LIGNE || (k == *nombreNoeuds-1 && feof(fichierCarte)) )
		{
			fseek(fichierCarte,-sizeof(char),SEEK_CUR);
			/*On  avance dans le fichier, sans enregistrer les valeurs pour l'instant.*/
			if ( !lireEntier(fichierCarte, "Graphe", k, &indiceSuccesseur) )
				return false;
			if( indiceSuccesseur > *nombreNoeuds )
			{
				printf("Graphe -- Ligne n°%d, indice successeur %d incorrect : indice hors limite. (Nombre noeuds = %d)\n", k, indiceSuccesseur, *nombreNoeuds);
				return false;
			}
			if( indiceSuccesseur == indice )
			{
				printf("Graphe -- Ligne n°%d, indice successeur %d incorrect : un noeud ne peut pas être so npropre successeur. (Nombre noeuds = %d)\n", k, indiceSuccesseur, *nombreNoeuds);
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
	/*On n'aura plus besoin de notre graphe sous forme de matrice d'adjacence, on le libère.*/
	libererGrapheMatrice(*nombreNoeuds, grapheMat);
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
	return -1; /*valeur -1 taitée comme un échec*/
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
	}
	return format;
}

int lireChaine(FILE *fichierCarte, char position[],int ligne, char chaine[])
{
	int format = fscanf(fichierCarte, "%s", chaine);
	if( !format )
	{
				printf("%s -- Ligne n°%d, erreur de formatage : chaîne de caractères attendue.\n", position, ligne);
	}
	return format;
}