#include "../include/gestion_fichier_carte.h"

/*On utilise les constantes relatives aux cartes définies dans "commun.h" */

/*Fonctionne pour l'instant uniquement pour des cartes de version 1*/
int lireCarte(FILE *carte, int *energie, char nomImage[],  unsigned char couleurClef[][NB_COULEURS], int *nombreNoeuds, Arbre *chemins)
{
	int i, j, k, couleur;
	char codeCarte[TAILLE_CODE];
	int versionCarte;
	char motClef[MAX_TAILLE_MOTCLEF];
	MotClef MC_lu;

	Noeud *noeud;
	int indice;
	TypeNoeud type;
	unsigned int x,y;
	int *successeurs;
	int nombreSuccesseurs;

	int tamponMemoire;
	long adresseDebutSuccesseurs;

	/**** vérif code ****/
	fscanf(carte,"%s %d\n",codeCarte, &versionCarte);
	if( strcmp(CODE_PROJET, codeCarte) != 0) 
	{
		printf("Code carte incorrect : %s au lieu de %s.\n", codeCarte, CODE_PROJET);
		return EXIT_FAILURE;
		
	}
	else
	{
		printf("Version carte : %d.\n", versionCarte);
	}

	/*vérif version (égale à 1)*/
	if( versionCarte != 1 )
	{
		printf("Version de la carte non supportée : %d.\n", versionCarte);
		return EXIT_FAILURE;
	}
	/***********/


	/***** Lecture des paramètres d'en-tête ******/
	for(i=0; i < NB_PARAM_PAR_VERSION[versionCarte -1]; i++) /*condition correspond aux nombre de paramètres attendus pour la version*/
	{
		if( fgetc(carte) == CODE_COM )
		{
			/*gestion des commetaire ?*/

			/*On ignore le commentaire, on passe à la ligne suivante*/
			while( fgetc(carte) != CODE_SAUT_LIGNE)
				{
					/*rien à faire ici */
				}
			i--; /*on  décrémente le compteur pour qu'il revienne à sa valeur initiale au début de la prochaine itération*/

		}
		else 
		{	
			/*on revient sur nos pas*/
			fseek(carte,-sizeof(char),SEEK_CUR); 
			/*et on recommence*/
			fscanf(carte,"%s", motClef);
			switch( MC_lu = correspondanceMotClef(motClef, versionCarte) )
			{
				case MC_carte :
					fscanf(carte, "%s", nomImage);
					printf("Image trouvée : %s.\n",nomImage);
					break;
				case MC_energie :
					/*fread(energie, sizeof(int), 1, carte); ne fonctionnera pas*/
					fscanf(carte, "%d",energie);
					printf("Énergie déterminée : %d.\n",*energie);
					break;
				case MC_chemin : case MC_noeud : case MC_construct : case MC_in : case MC_out :
					/*Dans ce cas-là, MC_lu est compris entre 0 et le nombre de params de couleurs - 1, et les indices correspondent à couleurClef et MOTCLEFS.*/
					for(couleur=0; couleur < NB_COULEURS; couleur++)
					{
						fscanf(carte, "%hhu", &couleurClef[MC_lu][couleur]);
					}
					printf("Couleur %s déterminée : (%hhu, %hhu, %hhu).\n", MOTCLEFS[MC_lu],couleurClef[MC_lu][rouge], couleurClef[MC_lu][vert], couleurClef[MC_lu][bleu]);
					break;
				case -1 : /*Valeur de renvoi pour mot-clef non reconnu*/
					return EXIT_FAILURE;
				default :
					printf("Vous ne devriez pas être ici !\n");
					return EXIT_FAILURE;
			}
			fscanf(carte,"\n"); /*On passe à la ligne suivante*/
		}

	}
	/**********/

	/***** Lecture de l'arbre *****/
	fscanf(carte, "%d", nombreNoeuds); /*du nombre de noeuds*/
	if(*nombreNoeuds < NB_MIN_NOEUDS) /*Il faut au moins une entrée et une sortie, donc deux*/
	{
		printf("Chemins non valide, trouvé %d noeuds : au moins %d noeuds nécessaires,.\n", *nombreNoeuds, NB_MIN_NOEUDS);
		return EXIT_FAILURE;
	}
	printf("Nombre de noeuds : %d\n", *nombreNoeuds);
	chemins = allouerArbre(*nombreNoeuds);
	for(k=0; k<*nombreNoeuds; k++)
	{
		fscanf(carte,"\n");
		fscanf(carte, "%d %d %u %u", &indice, &type, &x, &y);

		/*IMPORTANT : on vérifie que l'indice du noeud est compris entre 0 et nombreNoeuds-1*/
		if( indice < 0 || indice >= *nombreNoeuds )
		{
			printf("Ligne de noeud n°%d, indice %d invalide : doit être au plus %d. (\n", k, indice, *nombreNoeuds);
		}
		
		/*Décompte du nombre de successeurs.
		Avant de continuer, on enregistre notre position courante sur le fichier pour pouvoir revenir sur nos pas.*/
		nombreSuccesseurs = 0;
		adresseDebutSuccesseurs = ftell(carte);
		/*Si on est au dernier noeud, il faut s'assurer que l'on n'est pas arrivé à la fin du fichier.*/
		while( fgetc(carte) != CODE_SAUT_LIGNE || (k == *nombreNoeuds-1 && feof(carte)) )
		{
			fseek(carte,-sizeof(char),SEEK_CUR);
			/*On  avance dans le fichier, sans enregistrer les valeurs pour l'instant.*/
			fscanf(carte, "%d", &tamponMemoire);
			nombreSuccesseurs++; /*On a compté un successeur en plus.*/
		}
		fseek(carte, adresseDebutSuccesseurs, SEEK_SET); /*On revient à notre position d'avnt la boucle.*/
		/*À la fin de la boucle, on a le bon nombre de successeur (moyennant une vérification que l'on fera plus tard.)*/

		successeurs = malloc(nombreSuccesseurs * sizeof(int));
		if( !successeurs )
		{
			printf("Échec de l'allocation dynamique des successeurs. (noeud d'indice %d)\n", indice);
		}
		/*On recommence, en enregistrant cette fois.*/
		for(j=0; j<nombreSuccesseurs; j++)
		{
				fscanf(carte, "%d", &successeurs[j]);
		}

		/*On crée le noeud à partir du résultat.*/
		noeud = creerNoeud(type, indice, x, y, nombreSuccesseurs, successeurs);
		/*Ajoute le noeud à l'arbre, avec correspondance ) l'indice.*/
		chemins[indice] = noeud;
		printf("Noeud d'indice %d : type : %s, coord : (%u,%u)\n", indice, TYPENOEUD[chemins[indice]->type], chemins[indice]->x, chemins[indice]->y);
	}



	/********** Bientôt dans vos foyer : vérifs ***********/


	return EXIT_SUCCESS;
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
	printf("Mot-clef non reconnu : %s (version %d).\n", motClef, version);
	return -1; /*valeur -1 taitée comme un échec*/
}