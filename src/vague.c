#include "../include/vague.h"

Vague* creerVague(nbMonstre, tpsLimite, entreeMonstre, unsigned int x, unsigned int y){

	Monstre *monstre;
	monstre = malloc(sizeof(Monstre));
	if(!monstre){
		printf("Erreur, échec d'allocation dynamique!\n");
		exit(EXIT_FAILURE);
	}

	nbMonstre = calculerNbMonstre(type);
	tpsLimite = calculerTpsLim(type);
	entreeMonstre = calculerEntreeMonstre(type);

	vague->coord =  creerPoint(x, y);

	return;
}

int intervalle(){
	
	return;
}