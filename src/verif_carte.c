#include "../include/verif_carte.h"

bool validerChemins(Carte *carte, PPM_Image *imageCarte, int *nombreModif)
{
	int indice;
	int indiceSuccesseur;

	int compteurModif = 0;

	bool valide = true;


	/*À changer plus tard, question des arguments de la fonction */
	int nombreNoeuds = carte->nombreNoeuds;
	Graphe *chemins = carte->chemins;
	/**/

	/*alias de variables définies par commodité. */
	Noeud *noeud, *noeudSuccesseur;

	/*lA vérification des entrées et sorties
	* EST FAITE LORS DE LA CRÉATION DE LA CARTE*/
	

	/*vérification pour tous les chemins qui partent d' indice*/
	for(indice=0; indice < nombreNoeuds; indice++)
	{
		noeud = chemins[indice];
		valide = valide & verifierCoord(imageCarte->largeur, imageCarte->hauteur, noeud);
		valide = valide & sommetType_modifierPoint(imageCarte, carte->couleurClef, noeud->coord, noeud->type, &compteurModif);
		for(indiceSuccesseur=0; indiceSuccesseur < noeud->nombreSuccesseurs; indiceSuccesseur++)
		{
			noeudSuccesseur = noeud->successeurs[indiceSuccesseur];
			valide = valide & bresenham_modifierSeg(imageCarte,carte->couleurClef, noeud, noeudSuccesseur, &compteurModif);
			
		}
	}
	*nombreModif = compteurModif;
	return valide;
}


bool sommetType_verifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *point, TypeNoeud type, int *nombreInvalide)
{
	sommetType(imageCarte, couleurClef, point, type, false, nombreInvalide);
	if( *nombreInvalide ) /*si il y a au moins un invalide*/
		return false;
	return true;
}

bool sommetType_modifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *point, TypeNoeud type, int *nombreModif)
{
	sommetType(imageCarte, couleurClef, point, type, true, nombreModif);
	if( *nombreModif )
		return false;
	return true;
}

void sommetType(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *point, TypeNoeud type, bool modifier, int *compteur)
{
	unsigned int ligne = point->y;
	unsigned int colonne = point->x;

	unsigned char couleur[NB_COULEURS];
	TypeNoeud typeCorrect = (type == intersection) ? coude : type;
	bool correspond; 
	if( ligne < imageCarte->hauteur
		&& ligne >= 0
		&& colonne < imageCarte->largeur
		&& colonne >= 0)
	{
		/*On enregistre la valeur du point*/
		PPM_accesCouleur(imageCarte, 
						ligne, 
						colonne, 
						couleur );
		/*Est-ce que la couleur correspond à un chemin ?*/
		correspond = comparerCouleurs(couleur, couleurClef[correspondanceType(typeCorrect)]);
		if( !correspond )
		{
			//afficherCouleur(couleur); afficherCouleur(couleurClef[correspondanceType(typeCorrect)]);
			/*On modifie la couleur selon le cas*/
				if( modifier )
						PPM_modifierCouleur(imageCarte,
											ligne, 
											colonne, 
											couleurClef[correspondanceType(typeCorrect)]);
				(*compteur)++;
		}
	}
}

void liste_sommetType(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *point, TypeNoeud listeTypes[], bool modifier, int *compteur)
{
	int i;
	unsigned int ligne = point->y;
	unsigned int colonne = point->x;

	unsigned char couleur[NB_COULEURS];
	TypeNoeud typeCorrect = (listeTypes[1] == intersection) ? coude : listeTypes[1];
	bool correspond; 
	if( ligne < imageCarte->hauteur
		&& ligne >= 0
		&& colonne < imageCarte->largeur
		&& colonne >= 0)
	{
		/*On enregistre la valeur du point*/
		PPM_accesCouleur( imageCarte, 
						ligne, 
						colonne, 
						couleur );
		/*Est-ce que la couleur correspond à un chemin ?*/
		/* ATTENTION : on peut en fait avoir la couleur de chemin
		* ou les couleurs des noeuds du segments
		* trois possibilités au total
		*/
		correspond = false;
		for( i=0; i<NB_COULEURS_SEGMENT; i++)
		{
			correspond  |= comparerCouleurs( couleur, couleurClef[correspondanceType(listeTypes[i])] );
		}
		if( !correspond )
		{
			//afficherCouleur(couleur); afficherCouleur(couleurClef[correspondanceType(typeCorrect)]);
			/*On modifie la couleur selon le cas*/
				if( modifier )
						PPM_modifierCouleur(imageCarte,
											ligne, 
											colonne, 
											couleurClef[correspondanceType(typeCorrect)]);
				(*compteur)++;
		}
	}
}


bool bresenham_verifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Noeud *n1, Noeud *n2, int *nombreInvalide)
{
	bresenham(imageCarte, couleurClef, n1, n2, false, nombreInvalide);
	if( *nombreInvalide ) /*si il y a au moins un point invalide*/
		return false;
	return true;
}


bool bresenham_modifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Noeud *n1, Noeud *n2, int *nombreModif)
{
	bresenham(imageCarte, couleurClef, n1, n2, true, nombreModif);
	if( *nombreModif )
		return false;
	return true;
}

void bresenham(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Noeud *n1, Noeud *n2, bool modifier, int *compteur)
{
	/* Adaptation de l'algorithme de bresenham (par valeurs entières) au problème 
	* On généralise le cas du premier octant*/
	
	Point *p1 = n1->coord;
	Point *p2 = n2->coord;

	/**** Définitions dans le repère (O, x, y) ****/
	int deltaX = (int)(p2->x - (int)p1->x);
	int deltaY = (int)(p2->y - (int)p1->y);
	int absDX = abs(deltaX);
	int absDY = abs(deltaY);

	unsigned int x1 = p1->x;
	unsigned int y1 = p1->y;
	unsigned int x2 = p2->x;
	unsigned int y2 = p2->y;

	/*Permet de se restreindre aux cas x et y positifs (octants 1 et 2)*/
	int sensX = (deltaX > 0) ? 1 : -1;
	int sensY = (deltaY > 0) ? 1 : -1;
	/****/

	/* Détermine si l'on se trouve sur les octants 2, 3, 6 ou 7.
	*C'est-à-dire si la valeur absolue du coéfficient directeur est > 1
	*/
	bool octantVertical = ( absDY > absDX ); /*le test ne fonctionne pas chez moi si j'utilise abs()*/

	/**** Passage au repère (0, g, p) 
	* avec g (resp. p) la coordonnée à la plus grande (resp. petite) distance de l'origine
	* Cela permet de généraliser le cas du coeff dir <= 1 en valeur absolue
	****/  
	int deltaG = max(absDY, absDX);
	int deltaP = min(absDX, absDY);
	
	/*itérateurs*/
	unsigned int g = octantVertical ? y1 : x1;
	unsigned int p = octantVertical ? x1 : y1;
	/*bornes du grand axe*/
	unsigned int g1 = octantVertical ? y1 : x1;
	unsigned int g2 = octantVertical ? y2 : x2;
	
	int sensG = octantVertical ? sensY : sensX;
	int sensP = octantVertical ? sensX : sensY;
	/*****/

	int ligne, colonne;
	bool condition;
	
	/* obsolète :*/
	/*TypeNoeud type = coude;*/
	TypeNoeud segmentType[NB_COULEURS_SEGMENT] = { n1->type, coude, n2->type};

	Point *point = creerPoint(0,0);

	int epsilon=0; /* parfois appelé epsilon prime*/

	/*On traite finalement le segment comme dans l'octant 2 pour (g,p) */
	/**** Traitement ****/
	
	while( sensG*g <= sensG*g2 )
	{
		/* On repasse dans le repère (0, x, y)*/
		colonne = octantVertical ? p : g;
		ligne = octantVertical ? g : p;
		point->x = colonne;
		point->y = ligne;
		/*On ne modifie pas la couleur des noeuds, qui doivent correspondre à leur type*/
		/*On vérifie ou modifie la couleur du point suivant le cas*/
		if( g != g1 && g != g2 )
		{
			liste_sommetType(imageCarte, couleurClef, point, segmentType, modifier, compteur );
		}
		
		/*Voir algorithme de Bresenham pour plus de détails*/
		epsilon += deltaP;
		condition = 2*epsilon >= deltaG;
		
		if( condition )
		{
			p += sensP;
			epsilon -= deltaG;

		}
		g += sensG;
	}
	/****/
	libererPoint(point);
}


bool verifierCoord(unsigned int largeur, unsigned int hauteur, Noeud *noeud)
{
	Point *coord = noeud->coord;
	if( coord->x > largeur || coord->y > hauteur )
	{
		printf("Noeud - %s d'indice %d, coordonnées (%u, %u) invalides. (largeur : %u, hauteur : %u)\n", TYPENOEUD[noeud->type], noeud->indice, coord->x, coord->y, largeur, hauteur);
		return false;
	}
	return true;
}
