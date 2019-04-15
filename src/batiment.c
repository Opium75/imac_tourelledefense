#include "../include/batiment.h"

Tour* creerBatiment(TypeBatiment, TypeInst, unsigned int x, unsigned int y){
	Batiment *batiment;
	batiment = malloc(sizeof(Batiment));
	if(!batiment){
		printf("Erreur, échec d'allocation dynamique!\n");
		exit(EXIT_FAILURE);
	}

	batiment->coord =  creerPoint(x, y);

			puissance = calculerCout(type, typeInst);
			portee = calculerPortee(type, typeInst);
	return;

}