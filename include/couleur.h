#ifndef __FONCTIONS_COULEUR

#define __FONCTIONS_COULEUR

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/****** COULEURS ****/

#define NB_COULEURS 3 /*égal à 3 et ne pas modifier*/

#define MAX_VAL_COULEUR 255 /* garder <= 255 */

#define NB_COULEURS_CLEFS 5  /* égal à 5 et ne pas modifier*/


typedef enum {rouge, vert, bleu} Couleur;



bool comparerCouleurs(unsigned char couleur1[], unsigned char couleur2[]);

void copierCouleur(unsigned char copie[], const unsigned char couleur[]);

void homothetieCouleur(unsigned char couleur[], double scalaire);

void afficherCouleur(unsigned char couleur[]);

#endif