#ifndef __FONCTIONS_PPM_IMAGE

#define __FONCTIONS_PPM_IMAGE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../couleur.h"
#include "../point.h"
#include "../fichier.h"

#include "PPM_entete.h"

/*Si l'image est en NDG, seul le canal rouge (=gris) est utilisé.*/
/*J'ai choisi d'intégrer les moyennes de luminosité à la structure par commodité, ça mange pas de pain.*/


typedef struct {
	unsigned int largeur, hauteur;
	Dimensions *dim;
	unsigned char *canaux[NB_COULEURS];
} PPM_Image;

PPM_Image* PPM_creerImage(unsigned int largeur,unsigned int hauteur);
void PPM_afficherImage(PPM_Image *img);
void PPM_libererImage(PPM_Image *img);

void PPM_lireCanaux(FILE *src, PPM_Image *img);
bool PPM_lireImage(FILE *src, PPM_Image **img);

bool PPM_lireImage(FILE *src, PPM_Image **img);
void PPM_ecrireCanaux(FILE *dest, PPM_Image *img);

bool PPM_ecrireImage(FILE*dest, PPM_Image*img);

void PPM_accesCouleur(PPM_Image *img, unsigned int ligne, unsigned int colonne, unsigned char couleur[]);
void PPM_modifierCouleur(PPM_Image *img, unsigned int ligne, unsigned int colonne, unsigned char couleur[]);

#endif