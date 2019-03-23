#ifndef _FONCTIONS_FICHIER_CARTE

#define _FONCTIONS_FICHIER_CARTE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commun.h"
#include "noeud.h"

int lireCarte(FILE *carte, int *energie, char nomImage[],  unsigned char couleurClef[][NB_COULEURS], int *nombreNoeuds, Arbre *chemins);

MotClef correspondanceMotClef(char motClef[], int version);

#endif