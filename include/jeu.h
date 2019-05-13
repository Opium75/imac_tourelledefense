#ifndef __FONCTIONS_JEU

#define __FONCTIONS_JEU

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "carte.h"
#include "tour.h"
#include "vague.h"



/* Mega-structure qui comprend toutes les autres */
typedef struct {
	unsigned char niveau;
	int pointage, argent;
	Carte *carte;
	Cite *cite;
	Vague *chaine;
} Jeu;

Jeu* creerJeu(unsigned char niveau, int pointage, int argent, Carte *carte, Cite *cite, Vague *chaine);

Jeu* allouerJeu(void);
void relancerJeu(Jeu *jeu);

void lancerJeu(Jeu *jeu);

void traitementJeu(Jeu* jeu, time_t deltaT);

void libererJeu(Jeu *jeu);

#endif