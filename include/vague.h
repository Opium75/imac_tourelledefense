#ifndef VAGUE_H
#define VAGUE_H

#include <stdio.h>
#include <stdlib.h>

#define NB_MAX_MONSTRE 10

typedef struct Vague{
	unsigned int nbMonstre, tpsLimite, entreeMonstre;
	int tableauMonstre[NB_MAX_MONSTRE];
	Point *coord;
} Vague;

Vague* creerVague(nbMonstre, tpsLimite, entreeMonstre, unsigned int x, unsigned int y);

#endif		