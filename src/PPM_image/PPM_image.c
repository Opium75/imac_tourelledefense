#include "../../include/PPM_image/PPM_image.h"

PPM_Image* PPM_creerImage(unsigned int largeur,unsigned int hauteur) 
{
	int i;

	unsigned long int taille_canal = largeur*hauteur*sizeof(unsigned char);
	if( !taille_canal )
	{
		printf("Image -- Échec de l'allocation dynamique pour la taille !\n");
		exit(EXIT_FAILURE);
	}

	PPM_Image *img = malloc(sizeof(PPM_Image));
	if( !img ) 
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

	img->dim = creerPoint(largeur, hauteur);
	return img;
}

void PPM_afficherImage(PPM_Image *img) 
{
	int i,j;
	printf("Format : %u x %u.\n",img->dim->x,img->dim->y);
	for(i=0;i<NB_COULEURS;i++) 
	{
		for(j=0;j<img->dim->x*img->dim->y;j++) 
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
	for(j=0;j<img->dim->x*img->dim->y;j++) 
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
	valide = PPM_lireEntete(src, &largeur, &hauteur);
	if( valide )
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

void PPM_ecrireCanaux(FILE *dest, PPM_Image *img) 
{
	int j, canal;
	for(j=0;j<img->dim->x*img->dim->y;j++) 
	{
		for(canal=0; canal<NB_COULEURS; canal++)
		{
			fwrite( (img->canaux[canal])+j, sizeof(unsigned char), 1, dest );
		}
	}
}

bool PPM_ecrireImage(FILE *dest, PPM_Image*img) {
	PPM_ecrireEntete(dest,img->dim->x,img->dim->y);
	PPM_ecrireCanaux(dest,img);
	/*PPM_libererImage(img);*/
	return true;
}


void PPM_accesCouleur(PPM_Image *img, unsigned int ligne, unsigned int colonne, unsigned char couleur[])
{
	int c;
	for(c=0; c<NB_COULEURS; c++)
	{
		couleur[c] = img->canaux[c][img->dim->x*ligne + colonne];
	}
}

void PPM_modifierCouleur(PPM_Image *img, unsigned int ligne, unsigned int colonne, unsigned char couleur[])
{
	int c;
	for(c=0; c<NB_COULEURS; c++)
	{
		img->canaux[c][img->dim->x*ligne + colonne] = couleur[c];
	}
}
