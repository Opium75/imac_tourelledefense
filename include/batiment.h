#ifndef BATIMENT_H
#define BATIMENT_H

#include <stdio.h>
#include <stdlib.h>

#define NB_TYPES_BATIMENT 2
#define NB_TYPES_INSTALLATION 3

typedef enum{Centrale_energie, Installation}TypeBatiment;
typedef enum{Radar, UsineArme, StockMunition}TypeInstallation;

typedef struct Batiment{
	unsigned int cout, portee;
	Point *coord;
	TypeBatiment type;
	TypeInstallation typeInst;
} Batiment;

Tour* creerBatiment(TypeBatiment, TypeInst, unsigned int x, unsigned int y);

#endif	