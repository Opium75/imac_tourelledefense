#ifndef __FONCTIONS_PPM_ENTETE

#define __FONCTIONS_PPM_ENTETE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lecture_fichier_carte.h"
#include "../fichier.h"

#define CODE_COM_PPM '#'
#define CODE_SAUT_LIGNE_PPM '\n'
static const char CODE_PPM[] = "P6";


/* Retourne une erreur et arrête le programme si l'en-tête du fichier 
n'est pas conforme au format du projet.*/

bool PPM_lireEntete(FILE *src, unsigned int *largeur, unsigned int *hauteur);
void PPM_ecrireEntete(FILE *dest,unsigned int largeur, unsigned int hauteur);

void lireCommentaire(FILE *src);
void PPM_sautLigne(FILE *src);

#endif