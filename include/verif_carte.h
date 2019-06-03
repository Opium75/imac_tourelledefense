#ifndef __FONCTIONS_VERIF_CARTE

#define __FONCTIONS_VERIF_CARTE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "point.h"
#include "graphe.h"
#include "carte.h"
#include "couleur.h"
#include "PPM_image/PPM_image.h"


#define NB_COULEURS_SEGMENT 3

bool validerChemins(Carte *carte, PPM_Image *imageCarte, int *nombreModif);

/*Validation des chemins par algorithme de Bressenham*/
bool bresenham_verifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Noeud *n1, Noeud *n2, int *nombreInvalide);
bool bresenham_modifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Noeud *n1, Noeud *n2, int *nombreModif);

void bresenham(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Noeud *n1, Noeud *n2, bool modifier, int *nombrerModif);
/**/


/* Un truc */
bool sommetType_verifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *p, TypeNoeud type, int *nombreInvalide);
bool sommetType_modifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *p, TypeNoeud type, int *nombreModif);

void sommetType (PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p, TypeNoeud type, bool modifier, int *compteur);

void liste_sommetType(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *point, TypeNoeud listeTypes[], bool modifer, int *compteur);

/**/

bool verifierEntreeSortie(Carte *carte);
bool REC_verifierSortie(Noeud *noeud, int nombreNoeuds, int* listeVerifies);

bool verifierCoord(unsigned int largeur, unsigned int hauteur, Noeud *noeud);

#endif