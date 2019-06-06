#ifndef __FONCTIONS_FICHIER_CARTE

#define __FONCTIONS_FICHIER_CARTE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#include "couleur.h"
#include "point.h"
#include "fichier.h"
#include "graphe.h"
#include "carte.h"



/*On n'autorise maintenant qu'une seule sortie */



/*Étapes de lecture*/
bool lireVersion(FILE *fichierCarte, int *version); /*Fonctionne pour l'instant uniquement pour des cartes de version 1*/
bool lireParametres(FILE *fichierCarte, Carte *carte);
bool lireChemins(FILE *fichierCarte, int *nombreNoeuds, Graphe **chemins);
/**/

/*Lecture entière*/
bool lireCarte(FILE *fichierCarte, Carte *carte); /*avec structure Carte*/
/**/

int correspondanceMotClef(char motClef[], int version);
int correspondanceOption(char option[]);

/*Fonction de formatage*/
void sautLigne(FILE *fichierCarte);
/*Lecture de fichier avec vérif de formatage*/
int lireEntier(FILE *fichierCarte, char position[],int ligne, int *e);
int lireChaine(FILE *fichierCarte, char position[],int ligne, char chaine[]);

#endif