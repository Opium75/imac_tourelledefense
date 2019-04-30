#include "../include/jeu.h"

Jeu* creerJeu(unsigned char niveau, unsigned int pointage, Carte *carte, Tour **tours, int nombreTours, Vague *chaine)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->carte = carte;
	jeu->tours = tours;
	jeu->nombreTours = nombreTours;
	jeu->chaine = chaine;
	return jeu;
}

Jeu* allouerJeu(Carte *carte)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = 0;
	jeu->pointage = 0;
	jeu->carte = carte;
	jeu->tours = NULL;
	jeu->nombreTours = 0;
	jeu->chaine = NULL;
}

void relancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	lancerVague(jeu->chaine, jeu->niveau, jeu->carte, jeu->tours, jeu->nombreTours);

}

void lancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	/* première itération */
	Vague *vague = creerVague(jeu->niveau, jeu->carte);
	jeu->chaine = vague;
	lancerVague(jeu->chaine, jeu->niveau, jeu->carte, jeu->tours, jeu->nombreTours);
	afficherVague(jeu->chaine);
}

void libererJeu(Jeu *jeu)
{
	/* on libère tout */
	libererCarte(jeu->carte);
	//////laflemme
}