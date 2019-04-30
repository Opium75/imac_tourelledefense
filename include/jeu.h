#ifndef __FONCTIONS_JEU

#define __FONCTIONS_JEU

#include <stdio.h>
#include <stdlib.h>

#include "carte.h"
#include "tour.h"
#include "vague.h"

/* Mega-structure qui comprend toutes les autres */
typedef struct {
	unsigned char niveau;
	unsigned int pointage;
	Carte *carte;
	Tour **tours;
	Vague *chaine;
	int nombreTours;
	/* redondant, mais permet ... quoi ? */
} Jeu;


#endif