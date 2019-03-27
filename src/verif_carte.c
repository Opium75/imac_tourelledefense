#include "./include/verif_carte.h"

bool validerChemins(Carte *carte, PPM_Image *imageCarte)
{
	int indice;
	int indiceSuccesseur;
	double coeffDir;


	/*À changer plus tard, question des arguments de la fonction */
	int nombreNoeuds = carte->nombreNoeuds;
	Graphe *chemins = carte->chemins;
	unsigned char *couleufClef = carte->couleufClef;
	/**/

	/*alias de variables définies par commodité. */
	Noeud *noeud;

	/*vérification pour tous les chemins qui partent d' indice*/
	for(indice=0; indice < nombreNoeuds; indice++)
	{
		noeud = chemins[i];
		for(indiceSuccesseur=0; indiceSuccesseur < noeud->nombreSuccesseurs)
		{
			/* ?????*/
		}
	}
	return true;
}
double coeffDir(unsigned int x1, unsigned int y1, unsigned int )