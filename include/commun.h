#ifndef __CONSTANTES_GLOBALES

#define __CONSTANTES_GLOBALES

/**** AUTRES ****/
#define max(a,b) a>b ? a : b
#define min(a,b) a>b ? b : a
#define abs(a) max(a,-a)


/****** COULEURS ****/

#define NB_COULEURS 3 /*égal à 3 et ne pas modifier*/
typedef enum {rouge, vert, bleu} Couleur;

/***** CARTES *******/

/*Constantes pour fichiers .itd*/

#define MAX_TAILLE_MOTCLEF 10

#define CODE_COM '#'
#define CODE_SAUT_LIGNE '\n' /*ne pas modifier*/
#define MAX_VAL_COULEUR 255 /* garder <= 255 */

static const char REP_DONNEES_CARTE[] = "./data"; /*répertoire des fichiers .itd*/
static const char REP_DONNEES_IMAGE[] = "./images";

static const char CODE_PROJET[] = "@ITD";



/**** ATTENTION : les éléments de MOTCLEFS et l'énum doivent rester dans le même ordre !! *******/
/**** ET Garder chemin, noeud, construct, in, out, en premier. On s'en saert pour parcourir un tableau dans les vérifications*/

typedef enum {  MC_chemin,
        MC_noeud,
        MC_construct,
        MC_in,
        MC_out,
        MC_carte, 
        MC_energie
      } MotClef;

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

/*Constantes tailles maximales des noms et chemins des fichiers*/
#define MAX_TAILLE_NOM_FICHIER 20
#define MAX_TAILLE_CHEMIN_FICHIER 40
#define MAX_TAILLE_CODE 10 /*on prévoit large, dans le cads où le code est incorrect.*/
#define NB_COULEURS_CLEFS 5  /* égal à 5 et ne pas modifier*/


/*Constantes relatives aux versions des cartes*/
#define NB_VERSIONS 1
#define NB_PARAM_VERSION_1 7

static const int NB_PARAM_PAR_VERSION[NB_VERSIONS] = {NB_PARAM_VERSION_1};


/*Constantes pour structure de Noeuds*/

/*Au moins deux noeuds : un noeud d'entrée et un de sortie.*/
#define NB_MIN_NOEUDS 2

#define LARGEUR_SEGMENT 15 /*en pixels de l'image*/



/****************/

#endif