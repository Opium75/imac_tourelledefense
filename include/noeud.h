#ifndef _FONCTIONS_NOEUD

#define _FONCTIONS_NOEUD

#include <stdio.h>
#include <stdlib.h>


/*Indices de TypeNoeud et TYPENOEUD doivent se correspondre.*/
typedef enum { entree, sortie, coude, intersection} TypeNoeud;

static const char *TYPENOEUD[] = 
{
	"entrée",
	"sortie",
	"coude",
	"intersection"
};
/**/


typedef struct _NOEUD {
	TypeNoeud type;
	int indice;
	unsigned int x,y;
	int *successeurs;
	/*struct _NOEUD *successeurs; Pas vraiement non, on passe par les indices.*/
} Noeud, *Arbre;

/**** NOEUD ****/
Noeud* allouerNoeud(int nombreSuccesseurs);
/*ATTENTION creerNoeud suppose que successeurs est DÉJÀ ALLOUÉ ET REMPLI*/
Noeud* creerNoeud(TypeNoeud type, int indice, unsigned int x, unsigned int y, int nombreSuccesseurs, int *successeurs);
/**/
void libererNoeud(Noeud *noeud);

/**** ARBRE ****/
Arbre* allouerArbre(int nombreNoeuds);
void libererArbre(int nombreNoeuds, Arbre *arbre);
Arbre* indexerArbre(int nombreNoeuds, Arbre *listeNoeuds);
#endif