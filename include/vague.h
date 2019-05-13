#ifndef VAGUE_H
#define VAGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "carte.h"
#include "graphe.h"
#include "monstre.h"
#include "tour.h"
#include "cite.h"
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
/*Rajouté un niveau, qui permettra de modifier les tempsLimite, les monstres,
* et un temps entre l'arrivé de deux monstres
*/
/* Rajouté une structure de liste chaînée,
* permettra de les manipuler plus facilement.
*/

#define TAILLE_VAGUE_BASE 10
#define TEMPS_PAUSE_BASE 4 /*en secondes*/
#define NB_MAX_MONSTRES 50

/* une constante de temps entre cle moment où la dernière vague s'achêve
* et la prochaine vague se lance */
#define TEMPS_ENTRACTE 5 /* secondes */

/* c'est pourri comme énum, mais bon. */
typedef enum {nonLancee, lancee} EtatVague;

#define NB_ETATS_VAGUE 2

static const char *ETAT_VAGUE[] = {"Non lancée ","Lancée"};

typedef struct _VAGUE {
	unsigned char niveau;
	/* temps de pause entre le déploiement d echaque monstre */
	clock_t tempsPause;
	clock_t tempsPause_acc;
	/* quand la vague n'est pas encore lancée,
	* on augmente l'acc jusqu''à c equ'il atteingne TEMPS_ENTRACTE
	*/
	clock_t tempsEntracte_acc;
	/* pour savoir si la vague a été déployée */
	EtatVague etat;
	/* Les monstres peuvent venir de plusieurs entrées*/
	Noeud **entrees;
	/* le noeud de sortie de la carte.  */
	Noeud *sortie;
	int nombreEntrees;
	/* Voir ici un tableau dynamique de pointeurs sur Monstre*/
	Monstre **monstres;
	int nombreMonstres;
	//int indiceMonstreDeployer; non, on calculera à chaque fois.
	struct _VAGUE *suivante;
} Vague, *Chaine;

/*Oublie pas les types dans les arguments des fonctions*/
Vague* creerVague(unsigned char niveau, Carte *carte);
void libererVague(Vague *vague);
/*initialise le parcours des monstres de la vague */
void lancerVague(Vague *vague, Carte *carte, Cite *cite);
bool estTerminee(Vague *vague);

void deployerMonstre(Monstre *monstre, Carte *carte, Cite *cite);
int accesIndiceDeploiement(Vague *vague);

/** VAGUE comme CHAÎNE **/

bool traitementChaine(Chaine *chaine, clock_t deltaT, Carte *carte, Cite *cite, unsigned char *niveau, int *pointage, int *argent);

int longueurChaine(Vague *vague);

/* ajoute en dernière place à la chaîne  */
void ajouterVague(Vague *vague, Chaine chaine);
/* libère la première vague d'une chaîne
* et fait pointer la chaine vers la seconde
* (comme un << pop >>)
*/
void supprimerPremiereVague(Chaine *chaine);
void supprimerVagueSuivante(Chaine chaine);


/* obsolète */
void enleverVague(int indiceVague, Chaine chaine);
void libererChaine(Chaine chaine);
/** **/

void afficherVague(Vague *vague);

/* passage de temps processeur par unité de longueur à unité de longueur par secondes */
double calculerTempsSecondes(clock_t temps);

clock_t calculerTempsPause(unsigned char niveau);
int calculerNombreEntrees(int nombreEntreesTotal);
int calculerNombreMonstres(unsigned char niveau);

Noeud** calculerEntreesVague(int *nombreEntrees, Carte *carte);
Monstre** creerMonstresVague(unsigned char niveau, int *nombreMonstres, Noeud **entrees, int nombreEntrees);

void libererMonstresVague(int nombreMonstres, Monstre **monstres);
void libereEntreesVague(int nombreEntreesVague, Noeud **entrees);

#endif