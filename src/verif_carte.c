#include "../include/verif_carte.h"

bool validerChemins(Carte *carte, PPM_Image *imageCarte)
{
	int indice;
	int indiceSuccesseur;

	int compteurModif = 0;

	bool valide;


	/*À changer plus tard, question des arguments de la fonction */
	int nombreNoeuds = carte->nombreNoeuds;
	Graphe *chemins = carte->chemins;
	/**/

	/*alias de variables définies par commodité. */
	Noeud *noeud, *noeudSuccesseur;

	/*vérification des entrées et sorties*/
	valide = verifierEntreeSortie(carte);

	/*vérification pour tous les chemins qui partent d' indice*/
	for(indice=0; indice < nombreNoeuds; indice++)
	{
		noeud = chemins[indice];
		valide = valide & verifierCoord(imageCarte->largeur, imageCarte->hauteur, noeud);
		sommetType_modifierPoint(imageCarte, carte->couleurClef, noeud->coord, noeud->type, &compteurModif);
		for(indiceSuccesseur=0; indiceSuccesseur < noeud->nombreSuccesseurs; indiceSuccesseur++)
		{
			noeudSuccesseur = noeud->successeurs[indiceSuccesseur];
			bresenham_modifierSeg(imageCarte,carte->couleurClef, noeud->coord, noeudSuccesseur->coord, &compteurModif);
			
		}
	}
	printf("Nombre de modifications : %d\n", compteurModif);
	return valide;
}


bool sommetType_verifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *point, TypeNoeud type, int *nombreInvalide)
{
	sommetType(imageCarte, couleurClef, point, type, true, nombreInvalide);
	if( nombreInvalide ) /*si il y a au moins un invalide*/
		return false;
	return true;
}

void sommetType_modifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *point, TypeNoeud type, int *nombreModif)
{
	sommetType(imageCarte, couleurClef, point, type, true, nombreModif);
}

void sommetType (PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *point, TypeNoeud type, bool modifier, int *compteur)
{
	unsigned int ligne = point->y;
	unsigned int colonne = point->x;

	unsigned char couleur[NB_COULEURS];
	TypeNoeud typeCorrect = (type == intersection) ? coude : type;
	bool correspond = comparerCouleurs(couleur,  couleurClef[correspondanceType(typeCorrect)]);
	
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
		if( !correspond )
		{
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


bool bresenham_verifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p1, Point *p2, int *nombreInvalide)
{
	bresenham(imageCarte, couleurClef, p1, p2, false, nombreInvalide);
	if( nombreInvalide ) /*si il y a au moins un point invalide*/
		return false;
	return true;
}


void bresenham_modifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p1, Point *p2, int *nombreModif)
{
	bresenham(imageCarte, couleurClef, p1, p2, true, nombreModif);
}

void bresenham(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p1, Point *p2, bool modifier, int *compteur)
{
	/* Adaptation de l'algorithme de bresenham (par valeurs entières) au problème 
	/* On généralise le cas du premier octant*/
	
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
	unsigned char couleur[NB_COULEURS];
	bool condition;
	
	TypeNoeud type = coude;

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
			sommetType(imageCarte, couleurClef, point, type, modifier, compteur );
		
		/*Voir algorithme de Bresenham pour polus de détails*/
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

bool verifierEntreeSortie(Carte *carte)
{	
	int i,j;
	Noeud *noeud, *voisin;
	Graphe *chemins = carte->chemins;
	bool aSortie, valide = true;
	for(i=0; i < carte->nombreNoeuds; i++)
	{
		noeud = chemins[i];
		if( noeud->type == entree )
		{
			aSortie = false;
			for(j=0; j<noeud->nombreSuccesseurs; j++)
			{
				voisin = (noeud->successeurs)[j];
				if(voisin->type == sortie)
					aSortie = true;
			}
			if( !aSortie )
			{
				printf("Chemin - Entrée d'indice %d n'a pas de sortie.\n", i);
				valide = false;
			}
		}
	}
	return valide;
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