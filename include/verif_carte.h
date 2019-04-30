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

#include "commun.h"

bool validerChemins(Carte *carte, PPM_Image *imageCarte, int *nombreModif);

/*Validation des chemins par algorithme de Bressenham*/
bool bresenham_verifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p1, Point *p2, int *nombreInvalide);
bool bresenham_modifierSeg(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p1, Point *p2, int *nombreModif);

void bresenham(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p1, Point *p2, bool modifier, int *nombrerModif);
/**/


/* Un truc */
bool sommetType_verifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *p, TypeNoeud type, int *nombreInvalide);
bool sommetType_modifierPoint(PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS], Point *p, TypeNoeud type, int *nombreModif);

void sommetType (PPM_Image *imageCarte, unsigned char couleurClef[][NB_COULEURS],  Point *p, TypeNoeud type, bool modifier, int *compteur);
/**/

bool verifierEntreeSortie(Carte *carte);
bool REC_verifierSortie(Noeud *noeud, int nombreNoeuds, int* listeVerifies);

bool verifierCoord(unsigned int largeur, unsigned int hauteur, Noeud *noeud);

#endif