#ifndef __FONCTIONS_STRUCTURE_CARTE

#define __FONCTIONS_STRUCTURE_CARTE

#include <stdio.h>
#include <stdlib.h>

#include "couleur.h"
#include "graphe.h"
#include "commun.h"

typedef struct {
	int version;
	unsigned int energie;
	char nomImage[MAX_TAILLE_NOM_FICHIER];
	unsigned char couleurClef[NB_COULEURS_CLEFS][NB_COULEURS];
	int nombreNoeuds, nombreEntrees;
	Graphe *chemins;
	int *indicesEntrees;
	int indiceSortie;
} Carte;

/*Fonctions structure Carte*/
Carte* allouerCarte(void);
void libererCarte(Carte *carte);

void terminalCarte(Carte *carte); /*affichage sur console*/

MotClef correspondanceType(TypeNoeud type);


#endif