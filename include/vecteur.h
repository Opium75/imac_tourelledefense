#ifndef __FONCTIONS_VECTEUR

#define __FONCTIONS_VECTEUR

/* utilisées dans la vérification de la Carte,
* et l'algorithme du plus court chemin.
* On utilise des valeurs 0,1,2 pour la vérification,
* et 0, 1 pour l'agorithme*/
int* creerVecteurEntier(int taille, int initial);
void libererVecteurEntier(int* vecteur);

void afficherVecteurEntier(int *vecteur, int taille);

int estDansVecteur(int element, int *vecteur, int taille);



int** allouerMatriceCarree(int rang);
void libererMatriceCarree(int rang, int **matrice);

void afficherMatriceCarree(int rang, int **matrice);

#endif