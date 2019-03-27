#ifndef __FONCTIONS_GRAPHE

#define __FONCTIONS_GRAPHE

#include <stdio.h>
#include <stdlib.h>

#include "point.h"


/*Indices de TypeNoeud et TYPENOEUD doivent se correspondre.*/
typedef enum { 	entree = 1, 
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


#define NB_TYPE_NOEUD 4 /*ne pas modifier*/
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

/**** GRAPHE ****/
Graphe* allouerGraphe(int nombreNoeuds);
void libererGraphe(int nombreNoeuds, Graphe *graphe);

Graphe* matriceVersListeGraphe(int nombreNoeuds, Graphe_MatAdj *grapheMat);
/* à partir de la mmatrice d'adjacence associée*/

Graphe_MatAdj* allouerGrapheMatrice(int nombreNoeuds);
void libererGrapheMatrice(int nombreNoeuds, Graphe_MatAdj *grapheMat);

int** allouerMatriceCarree(int rang);
void libererMatriceCarree(int rang, int **matrice);
void afficherMatriceCarree(int rang, int **matrice);




#endif