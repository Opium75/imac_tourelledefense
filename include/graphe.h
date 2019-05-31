#ifndef __FONCTIONS_GRAPHE

#define __FONCTIONS_GRAPHE

#include <stdio.h>
#include <stdlib.h>

#include "point.h"

#define NB_MONSTRES_PAR_VAGUES


/*Indices de TypeNoeud et TYPENOEUD doivent se correspondre.*/
typedef enum {	entree = 1, 
				sortie = 2,
				coude = 3,
				intersection = 4} TypeNoeud;

static const char *TYPENOEUD[] = 
{
	"_PARDÉFAUT", /*élément présent pour décaler les indices.*/
	"entrée",
	"sortie",
	"coude",
	"intersection"
};

#define NB_TYPES_NOEUD 4 /*ne pas modifier*/
/**/


typedef struct _NOEUD {
	TypeNoeud type;
	int indice;
	Point *coord;
	int nombreSuccesseurs;
	struct _NOEUD **successeurs; /*liste de pointeurs sur noeuds*/
} Noeud, *Graphe;

typedef struct {
	int nombreNoeuds;
	TypeNoeud *listeType;
	Point **listeCoord;
	int **adjacence;
} Graphe_MatAdj;

/**** NOEUD ****/
Noeud* creerNoeud(TypeNoeud type, int indice, Point *coord, int nombreSuccesseurs);
void libererNoeud(Noeud *noeud);
void afficherNoeud(Noeud *noeud);

/**** GRAPHE ****/
Graphe* allouerGraphe(int nombreNoeuds);
void libererGraphe(int nombreNoeuds, Graphe *graphe);
void afficherGraphe(Graphe *graphe, int nombreNoeuds);

Graphe* matriceVersListeGraphe(int nombreNoeuds, Graphe_MatAdj *grapheMat);
/* à partir de la matrice d'adjacence associée*/

/* utilisées lors de la constitution de la Carte */
//void extraireEntreeSortie(int **indicesEntrees, int *nombreEntrees, int *indiceSortie, int nombreNoeuds, Graphe *graphe);

/** PARCOURS EN PROFONDEUR / ENTRÉES SORTIES **/
bool extraireEntreesSorties(int **indicesEntrees, int *nombreEntrees, int ***indicesSorties, int **nombreSorties, int nombreNoeuds, Graphe *graphe);
void extraireEntrees(int **indicesEntrees, int *nombreEntrees, int nombreNoeuds, Graphe *graphe);
bool extraireSorties(Noeud *noeud, int indiceEntree, int **indicesSorties, int *nombreSorties, int nombreNoeuds);

int compterSorties(Noeud *noeud, int nombreNoeuds);
void REC_compterSorties(Noeud *noeud, int *nombreSorties, int *listeComptes);

void enregistrerSorties(Noeud *noeud, int *indicesSorties, int nombreSorties, int nombreNoeuds);
void REC_enregistrerSorties(Noeud *noeud, int *indicesSorties, int *compteur, int nombreSorties, int *listeComptes);


void libererIndicesEntrees(int* indicesEntrees);
void libererIndicesSorties(int** indicesSorties, int nombreEntrees);
/** **/

/* utilisées dans la vérification de la Carte,
* et l'algorithme du plus court chemin.
* On utilise des valeurs 0,1,2 pour la vérification,
* et 0, 1 pour l'agorithme*/
int* creerVecteurEntier(int taille, int initial);
void afficherVecteurEntier(int *vecteur, int taille);
void libererVecteurEntier(int* vecteur);

Graphe_MatAdj* allouerGrapheMatrice(int nombreNoeuds);
/*Libère le graphe par matrice d'adjacence, avec la liste des noeuds, et avec ou sans les noeuds suivant avecContenuPoint*/
void libererGrapheMatrice(int nombreNoeuds, Graphe_MatAdj *grapheMat, bool avecContenuPoint);

int** allouerMatriceCarree(int rang);
void libererMatriceCarree(int rang, int **matrice);
void afficherMatriceCarree(int rang, int **matrice);




#endif