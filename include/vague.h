#ifndef VAGUE_H
#define VAGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "carte.h"
#include "graphe.h"
#include "monstre.h"
#include "tour.h"
#include "parcours.h"

/* J'ai modifié quelques trucs :
* T'as bien fait de définir un nombre max de monstres,
* Mais utiliser un tableau statique (taille fixe),
* prend de l'espace qui ne sera pas forcément utilisé.
* Donc j'ai changé le tableau en dynamique,
* quitte à allouer dynamiquement l'espace.
*/
/* J'ai aussi changé les types pour les faire correspondre
* à ce dont on va se servir (Noeud, Monstre, etc)
*/
/* J'ai pas compris à quoi correspondait les coord ?
*/
/*Rajouté un niveau, qui permettra de modifier les tempsLimite, les monstres,
* et un temps entre l'arrivé de deux monstres
*/
/* Rajouté une structure de liste chaînée,
* permettra de les manipuler plus facilement.
*/

#define TAILLE_VAGUE_BASE 10
#define TEMPS_LIMITE_BASE 20 /*en secondes*/
#define TEMPS_PAUSE_BASE 20 /*en secondes*/
#define NB_MAX_MONSTRES 50

typedef struct _VAGUE {
	unsigned char niveau;
	unsigned int tempsLimite, tempsPause;
	/* Les monstres peuvent venir de plusieurs entrées*/
	Noeud **entrees;
	/* le noeud de sortie de la carte.  */
	Noeud *sortie;
	int nombreEntrees;
	/* Voir ici un tableau dynamique de pointeurs sur Monstre*/
	Monstre **monstres;
	int nombreMonstres;
	struct _VAGUE *suivante;
} Vague, *Chaine;

/*Oublie pas les types dans les arguments des fonctions*/
Vague* creerVague(unsigned char niveau, Carte *carte);
void libererVague(Vague *vague);
/*initialise le parcours des monstres de la vague */
void lancerVague(Vague *vague, unsigned char niveau, Carte *carte, Tour **tours, int nombreTours);


/** VAGUE comme CHAÎNE **/
int longueurChaine(Vague *vague);

/* libère la première vague d'une chaîne
* et fait pointer la chaine vers la seconde
* (comme un << pop >>)
*/
void enleverPremiereVague(Chaine *chaine);
/* idem mais pour une indice NON NUL */
void enleverVague(int indiceVague, Chaine chaine);
/** **/

void afficherVague(Vague *vague);

unsigned int calculerTempsLimite(unsigned char niveau);
unsigned int calculerTempsPause(unsigned char niveau);
int calculerNombreEntrees(int nombreEntreesTotal);
int calculerNombreMonstres(unsigned char niveau);

Noeud** calculerEntreesVague(int *nombreEntrees, Carte *carte);
Monstre** creerMonstresVague(unsigned char niveau, int *nombreMonstres, Noeud **entrees, int nombreEntrees);

void libererMonstresVague(int nombreMonstres, Monstre **monstres);
void libereEntreesVague(int nombreEntreesVague, Noeud **entrees);

#endif		