#include "../../include/PPM_image/PPM_image.h"

PPM_Image* PPM_creerImage(unsigned int largeur,unsigned int hauteur) 
{
	int i;
	unsigned long int taille_canal = largeur*hauteur*sizeof(unsigned char);
	PPM_Image *img = malloc(sizeof(PPM_Image));
	if(!img) 
	{
		printf("Image -- Échec de l'allocation dynamique pour l'Image !\n");
		exit(EXIT_FAILURE);
	}
	for(i=0;i<NB_COULEURS;i++) 
	{
		(img->canaux)[i] = malloc(taille_canal);
		if(!(img->canaux)[i]) 
		{
			printf("Image -- Échec de l'allocation dynamique pour les canaux !\n");
			exit(EXIT_FAILURE);
		}
	}
	img->largeur = largeur;
	img->hauteur = hauteur;
	return img;
}

void PPM_afficherImage(PPM_Image *img) 
{
	int i,j;
	printf("Format : %u x %u.\n",img->largeur,img->hauteur);
	for(i=0;i<NB_COULEURS;i++) 
	{
		for(j=0;j<img->largeur*img->hauteur;j++) 
		{
			printf("%hhu ",*((img->canaux[i])+j));
		}
	}
	printf("\n");
}

void PPM_libererImage(PPM_Image *img) 
{
	int i;
	for(i=0;i<NB_COULEURS;i++) {
		free(img->canaux[i]);
	}
	free(img);
}

void PPM_lireCanaux(FILE *src, PPM_Image *img) 
{
	int j, canal;
	for(j=0;j<img->largeur*img->hauteur;j++) 
	{
		for(canal=0; canal<NB_COULEURS; canal++)
		{
			fread( (img->canaux[canal])+j, sizeof(unsigned char), 1, src );
		}
	}
}

bool PPM_lireImage(FILE *src, PPM_Image **img) 
{
	int valide;
	unsigned int largeur,hauteur;
	if( valide = PPM_lireEntete(src, &largeur, &hauteur) )
	{
		*img = PPM_creerImage(largeur, hauteur);
		PPM_lireCanaux(src, *img);
	}
	else
	{
		printf("Formatage de l'image associée erroné, échec de lecture de l'image.\n");
	}
	return valide;
}