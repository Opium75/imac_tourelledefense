#ifndef __FONCTIONS_STRUCTURE_CARTE

#define __FONCTIONS_STRUCTURE_CARTE

#include <stdio.h>
#include <stdlib.h>

#include "fichier.h"

#include "couleur.h"
#include "graphe.h"

/***** CARTES *******/

/*Constantes pour fichiers .itd*/

#define MAX_TAILLE_MOTCLEF 15

#define CODE_COM '#'
#define CODE_SAUT_LIGNE '\n' /*ne pas modifier*/

static const char REP_DONNEES_CARTE[] = "./data"; /*répertoire des fichiers .itd*/
static const char REP_DONNEES_IMAGE[] = "./images/carte";

static const char CODE_PROJET[] = "@ITD";

/*Constantes relatives aux versions des cartes*/
#define NB_VERSIONS 1
#define NB_PARAM_VERSION_1 7

#define NB_PARAM_OPTION 1

static const int NB_PARAM_PAR_VERSION[NB_VERSIONS] = {NB_PARAM_VERSION_1};


/*Constantes pour structure de Noeuds*/

/*Au moins deux noeuds : un noeud d'entrée et un de sortie.*/
#define NB_MIN_NOEUDS 2

#define LARGEUR_SEGMENT 15 /*en pixels de l'image*/




/**** ATTENTION : les éléments de MOTCLEFS et l'énum doivent rester dans le même ordre !! *******/
/**** ET Garder chemin, noeud, construct, in, out, en premier. On s'en saert pour parcourir un tableau dans les vérifications*/

typedef enum {  MC_chemin,
        MC_noeud,
        MC_construct,
        MC_in,
        MC_out,
        MC_carte, 
        MC_energie,
      } MotClef;

typedef enum {
	OPT_arrierePlan
} OPT_MotClef;

static const char *MOTCLEFS[] = 
{ 
  "chemin",
  "noeud",
  "construct",
  "in",
  "out",
  "carte",
  "energie",
};

static const char *OPT_MOTCLEFS[] =
{
	"arriereplan"
};

typedef struct {
	int version;
	unsigned int energie;
	char nomImage[MAX_TAILLE_NOM_FICHIER];
	/* POUR L'AFFICHAGE UN PEU PLUS JOLI */
	bool possedeArrierePlan;
	char nomArrierePlan[MAX_TAILLE_NOM_FICHIER];
	unsigned char couleurClef[NB_COULEURS_CLEFS][NB_COULEURS];
	int nombreNoeuds;
	Graphe *chemins;
  int nombreEntrees;
	int *indicesEntrees;
  /* les indices des noeuds de soties possibles
  * pour chaque entrée
  */
  int *nombreSorties;
	int **indicesSorties;
} Carte;

/*Fonctions structure Carte*/
Carte* allouerCarte(void);
void libererCarte(Carte *carte);

int correspondanceIndicesEntrees(int indice, int *indicesEntrees, int nombreEntrees);

void terminalCarte(Carte *carte); /*affichage sur console*/

MotClef correspondanceType(TypeNoeud type);


#endif