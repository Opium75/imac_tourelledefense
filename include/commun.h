#ifndef _CONSTANTES_GLOBALES

#define _CONSTANTES_GLOBALES

/****** COULEURS ****/

#define NB_COULEURS 3 /*égal à 3 et ne pas modifier*/
typedef enum {rouge, vert, bleu} Couleur;

/***** CARTES *******/

/*Constantes tailles maximales des noms et chemins*/
#define MAX_TAILLE_NOM_FICHIER 20
#define MAX_TAILLE_CHEMIN_FICHIER 30
#define TAILLE_CODE 4
#define NB_COULEURS_CLEFS 5  /* égal à 5 et ne pas modifier*/


/*Constantes relatives aux versions des cartes*/
#define NB_VERSIONS 1
#define NB_PARAM_VERSION_1 7

static const int NB_PARAM_PAR_VERSION[NB_VERSIONS] = {NB_PARAM_VERSION_1};

/*Constantes pour fichiers .itd*/

#define MAX_TAILLE_MOTCLEF 10

#define CODE_COM '#'
#define CODE_SAUT_LIGNE '\n' /*ne pas modifier*/

static const char CODE_PROJET[] = "@ITD";

/**** ATTENTION : les éléments de MOTCLEFS et l'énum doivent rester dans le même ordre !! *******/
/**** ET Garder chemin, noeud, construct, in, out, en premier. On s'en saert pour parcourir un tableau dans les vérifications*/

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

typedef enum {  MC_chemin,
				MC_noeud,
				MC_construct,
				MC_in,
				MC_out,
				MC_carte, 
				MC_energie,} MotClef;

/*Constantes pour structure de Noeuds*/

/*Au moins deux noeuds : un noeud d'entrée et un de sortie.*/
#define NB_MIN_NOEUDS 2



/****************/

#endif