#ifndef _CONSTANTES_GLOBALES

#define _CONSTANTES_GLOBALES

/****** COULEURS ****/

#define NB_COULEURS 3
typedef enum {rouge, vert, bleu} Couleur;

/***** CARTES *******/

/*Constantes tailles maximales des noms et chemins*/
#define MAX_TAILLE_NOM_FICHIER 20
#define MAX_TAILLE_CHEMIN_FICHIER 30
#define TAILLE_CODE 4
#define NB_COULEURS_CLEFS 5  /* à ne pas modifier*/


/*Constantes relatives aux versions des cartes*/
#define NB_VERSIONS 1
#define NB_PARAM_VERSION_1 7



/*Constantes pour fichiers .itd*/

#define MAX_TAILLE_MOTCLEF 10

#define CODE_COM '#'
static const char CODE_PROJET[] = "@ITD";

/*static const char MOTCLEF_CARTE[] = "carte";
static const char MOTCLEF_ENERGIE[] = "energie";
static const char MOTCLEF_CHEMIN[]  = "chemin";
static const char MOTCLEF_NOEUD[] = "noeud";
static const char MOTCLEF_CONSTRUCT[] = "construct";
static const char MOTCLEF_IN[] = "in";
static const char MOTCLEF_OUT[] = "out";*/

/**** ATTENTION : les éléments de MOTCLEFS et l'énum doivent rester dans le même ordre !! *******/

typedef enum {  CC_chemin,
				CC_noeud,
				CC_construct,
				CC_in,
				CC_out} CouleurClef;

static const char *MOTCLEFS[] = 
{ "carte",
  "energie",
  "chemin",
  "noeud",
  "construct",
  "in",
  "out"
};

typedef enum {  MC_carte, 
				MC_energie,
				MC_chemin,
				MC_noeud,
				MC_construct,
				MC_in,
				MC_out} MotClef;

static const int NB_PARAM_PAR_VERSION[NB_VERSIONS] = {NB_PARAM_VERSION_1};



/****************/

#endif