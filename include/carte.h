#ifndef __FONCTIONS_STRUCTURE_CARTE

#define __FONCTIONS_STRUCTURE_CARTE

#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"
#include "commun.h"

typedef struct {
	int version;
	unsigned int energie;
	char nomImage[MAX_TAILLE_NOM_FICHIER];
	unsigned char couleurClef[NB_COULEURS_CLEFS][NB_COULEURS];
	int nombreNoeuds;
	Graphe *chemins;
} Carte;

/*Fonctions structure Carte*/
Carte* allouerCarte();
void libererCarte(Carte *carte);

void afficherCarte(Carte *carte); /*affichage sur console*/


#endif