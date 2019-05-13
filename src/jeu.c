#include "../include/jeu.h"

Jeu* creerJeu(unsigned char niveau, int pointage, int argent, Carte *carte, Cite *cite, Vague *chaine)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = niveau;
	jeu->pointage = pointage;
	jeu->argent = argent;
	jeu->carte = carte;
	jeu->cite = cite;
	jeu->chaine = chaine;
	return jeu;
}

Jeu* allouerJeu(void)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = 0;
	jeu->pointage = 0;
	jeu->argent = 0;
	jeu->carte = allouerCarte();
	jeu->cite = allouerCite();
	jeu->chaine = NULL;
	return jeu;
}

void relancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);
}

void lancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	/* première itération */
	Vague *vague = creerVague(jeu->niveau, jeu->carte);
	jeu->chaine = vague;
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);
}

void traitementJeu(Jeu* jeu, time_t deltaT)
{
	/* ici, on fait tout les calculs et changements,
	* la fonction est appelée à chaque itération du jeu
	*/
	int k;
	
	/* les vagues de monstres */
	traitementChaine(&(jeu->chaine), deltaT, jeu->carte, jeu->cite, &(jeu->niveau), &(jeu->pointage), &(jeu->argent) );
	
}

void libererJeu(Jeu *jeu)
{
	/* on libère tout */
	libererCarte(jeu->carte);
	libererCite(jeu->cite);
	libererChaine(jeu->chaine);
	free(jeu);
}