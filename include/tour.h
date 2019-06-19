#ifndef __FONCTIONS_TOUR
#define __FONCTIONS_TOUR

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vecteur.h"
#include "point.h"
#include "carte.h"
#include "monstre.h"

#define plancher(a) a>0 ? a : 0

/* présent dans "monstre.h"
* #define NB_TYPES_TOUR 4
*/

#define RAYON_TOUR 80 /* en pixel, rond */

/*attibuts unitaires*/
#define PUISSANCE_BASE 2 /*relatif aux points de vie et résistance des monstres.*/
#define TEMPS_TIR_BASE 0.05 /*en secondes*/
#define PORTEE_BASE 200 /*en pixels*/
#define COUT_BASE 5 /*PIÈCES*/
#define NB_CIBLES_MAX_BASE 1 /* nombre de cibles maximal possible */

static const unsigned int PUISSANCE_TYPE[NB_TYPES_TOUR] = {4, 2, 2, 2};
static const unsigned int PORTEE_TYPE[NB_TYPES_TOUR] = {2, 4, 8, 2};
static const double TEMPS_TIR_TYPE[NB_TYPES_TOUR] = {4, 2, 8, 2};
static const unsigned int COUT_TYPE[NB_TYPES_TOUR] = {3, 2, 3, 1};
static const unsigned int NOMBRE_CIBLES_MAX_TYPE[NB_TYPES_TOUR] = { 1, 1, 1, 4};

typedef enum {T_rouge, T_vert, T_bleu, T_jaune} TypeTour; 

typedef struct _TOUR {
	TypeTour type;
	unsigned int puissance, portee, coutAchat, nombreCiblesMax;
	/* MAINTENANT on a un temps de tir,
	* qui correspond à l'intervalle de temps entre deux tirs
	*/
	time_t tempsTir;
	/* ON REPORTE L'ACCUMULATEUR DANS LES CIBLES
	* AINSI LA TOUR TIRE SELON LA CIBLE */
	Point *coord;
	/* permettra d'enregistrer le monstre ciblé
	* pour le garder en mémoire tant qu'il ne change pas.
	*/
	/* on a maintenant créé une structure adapatée en liste chaînée */
	Cible *cible;
	/* J'ai changé la structure en liste chaînée
	* comme ça on peut en supprimer comme on veut
	*/
	struct _TOUR *suivante;
} Tour, *ListeTour;

typedef struct _CIBLE {
	Monstre *monstre;
	time_t tempsTir_acc;
	struct __CIBLE *suivante;
} Cible, *Mire;

Tour* creerTour(TypeTour type, unsigned int x, unsigned int y);
void libererTour(Tour *tour);

void terminalTour(Tour *tour);
void terminalListe(ListeTour liste);

void reinitialiserCibles(ListeTour liste);
bool estAPortee(Tour *tour, Monstre *monstre);
bool doitChangerCible(Tour *tour);


/*** LISTETOUR ***/
/* en pratique, on n'utilisera pas ces fonctions directement,
* on passera par le pendant de chacune pour la cité
*/
int longueurListe(ListeTour liste );
void ajouterTour(Tour *tour, ListeTour liste);
void enleverPremiereTour(ListeTour *liste);
void enleverTour(int indiceTour, ListeTour liste);
void libererListeTour( ListeTour liste );

/* En cas de succès, renvoie l'indice relative de la tour en (x, y).
* Renvoie -1 s'il n'y a aucune tour (x, y).
* L'indice change à chaque suppression, il faut donc le recalculer souvent
*/
int trouverIndiceTour(ListeTour liste, unsigned int x, unsigned int y);
int trouverIndiceTourRayon(ListeTour liste, unsigned int x, unsigned int y, int rayon);
int calculerIndiceClique(int *indicesTours, int *distancesTours, int nombreToursTrouvees);


void traitementListe(ListeTour *liste, clock_t deltaT,  Monstre **monstres, int nombreMonstres, int *gainPointsTotal, int *gainArgentTotal);


bool verifierEmplacementTour(ListeTour liste, Point *coordClique);

/* Une fonction de MONSTRE
* on préfère que Monstre contrôle les dégats qu'il reçoit
* pour qu'il sache qu'il doit disparaître
*/
bool recevoirDegats(Monstre *monstre, Tour *tour, int *gainPoints, int *gainArgent);

void attaquerCible(Tour *tour, clock_t deltaT, Monstre **monstres, int nombreMonstres, int *gainPoints, int *gainArgent);
void attaquerMonstres(ListeTour liste, clock_t deltaT,  Monstre **monstres, int nombreMonstres, int *gainPointsTotal, int *gainArgentTotal);

/* Fait cibler la tour sur un monstre à portée
* renvoie vrai en cas de succès
* faux si aucun monstre n'est à sa portée.
*/
bool ciblerMonstre(Tour *tour, Monstre **monstres, int nombreMonstres);
/* Le choix est de cibler le monstre le plus proche */
int calculerIndiceCible(int *indicesMonstres, int *distancesMonstres, int nombreMonstresCibles);

unsigned int calculerPuissance(TypeTour type);
unsigned int calculerPortee(TypeTour type);
unsigned int calculerCout(TypeTour type);
unsigned int calculerNombreCiblesMax(TypeTour type);

clock_t calculerTempsTir(TypeTour type);

#endif
