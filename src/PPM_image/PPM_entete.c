#include "../../include/PPM_image/PPM_entete.h"


bool PPM_lireEntete(FILE *src, unsigned int *largeur, unsigned int *hauteur)
{
	char chaineFormat[MAX_TAILLE_CODE];
	int valCouleur;
	if( !lireChaine(src, "Image", 0, chaineFormat) )
		return false;
	lireCommentaire(src);
	fscanf(src, "%u %u\n",largeur,hauteur);
	/*if( !lireEntier(src, "Image", 1, largeur) || !lireEntier(src, "Image", 1, hauteur))
		return false;*/
	lireCommentaire(src);
	fscanf(src,"%d\n", &valCouleur);
	/*if( !lireEntier(src, "Image", 1, &valCouleur) )
		return false;*/
	if( (valCouleur != MAX_VAL_COULEUR) || strcmp(chaineFormat, CODE_PPM) ) 
	{
		printf("Image -- non conforme !\n");
		printf("%u x %u, %d, format %s.\n",*largeur,*hauteur, valCouleur, chaineFormat);
		return false;
	}
	return true;
}

void PPM_ecrireEntete(FILE*dest,unsigned int largeur, unsigned int hauteur) 
{
	fputs(CODE_PPM,dest);
	fprintf(dest,"\n%u %u\n%hhu\n",largeur,hauteur,MAX_VAL_COULEUR);
}



void lireCommentaire(FILE *src) 
{
	char commentaireSigne;
	char commentaireTampon;
	commentaireSigne = fgetc(src);
	
	if(commentaireSigne == CODE_COM_PPM) {
		commentaireTampon = fgetc(src);
		
		while(commentaireTampon != CODE_SAUT_LIGNE_PPM) {
			commentaireTampon = fgetc(src);
		}
	}
	else fseek(src,-sizeof(char),SEEK_CUR);
}

void PPM_sautLigne(FILE *src)
{
	int i=0;
	while( fgetc(src) != CODE_SAUT_LIGNE_PPM || feof(src) )
	{
		i++;
	}
}