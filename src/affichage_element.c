#include "../include/affichage_element.h"

void afficherCarte(GLuint idAffichage, Dimensions *dimImage)
{
    /* La carte prend tout l'espace de la fenêtre */
    afficherElement(idAffichage);
}


void afficherAide(GLuint idAffichage, Dimensions *dimImage)
{
    /* l'aide remplace la carte */
    afficherElement(idAffichage);
}

void afficherImageMenu(GLuint idAffichage, Point *coordMenu, Dimensions *dimMenu, Dimensions *dimImage)
{
    double posX, posY;
    double propX, propY;
    Point coord;
    Dimensions dimEchelle;
    /* calcul coordonnées virtuelles */
    calculerCoordonneesPourcentage(&coord, coordMenu, dimImage);
    calculerCoordonneesVirtuelles(&coord, &posX, &posY, dimImage);
    /* calcul de l'échelle */
    calculerDimensionsPourcentage(&dimEchelle, dimMenu, dimImage);
    calculerDimensionsEchelleImage(&propX, &propY, &dimEchelle, dimImage);
    /* on  affiche le menu  */
        glPushMatrix();
            /* affiche le monstre a<zu bon endroit */
            glTranslatef(posX, posY, 0.);
            glScalef(propX, propY, 1.);
            afficherElement(idAffichage);
        glPopMatrix();
}


void afficherVague(Vague *vague, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage)
{
    int k;
    int indiceBanque;
    Monstre *monstre;
    /* on crée le type de lutin de toute pièce */
    TypeLutin type;
    type.nature = LUT_monstre;
    for( k=0; k<vague->nombreMonstres; k++ )
    {

        monstre = vague->monstres[k];
        if( monstre->etat == enMouvement )
        {
             /* change le type */
            type.typeMonstre = monstre->type;
            /* calcule l'indice associé au type */
            indiceBanque = correspondanceIndiceLutin(&type);
            afficherMonstre(monstre, banqueAffichage[indiceBanque], &listeDim[indiceBanque], dimImage);
        }
    }
}

void afficherChaine(Chaine chaine, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage)
{
    /** on se contentera d'affciher la première vague lancée */
    while( chaine && chaine->etat != lancee )
        chaine = chaine->suivante;
    if( chaine )
        afficherVague(chaine, banqueAffichage, listeDim, dimImage);
}

void afficherCite(Cite *cite, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage)
{
    /** on affichera pour l'instant que la liste des tours */

    afficherListeTour(cite->listeTour, banqueAffichage,listeDim, dimImage);
}

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[], Dimensions listeDim[], Dimensions *dimImage)
{
    int indiceBanque;
    /* idem Monstre */
    TypeLutin type;
    type.nature = LUT_tour;
    while( liste )
    {
        /* impérativement changer le type */
        type.typeTour = liste->type;
        indiceBanque = correspondanceIndiceLutin(&type);
        /* */
        afficherTour(liste, banqueAffichage[indiceBanque], &listeDim[indiceBanque], dimImage);   
        liste = liste->suivante;
    }
}

void afficherMonstre(Monstre *monstre, GLuint idAffichage, Dimensions *dimLutin, Dimensions *dimImage)
{
    double posX, posY;
    double propX, propY;
    /* on récupère les coordonnées du monstre */
    Point coordMonstre;
    calculerPositionMonstre(monstre, &coordMonstre);
    /* calcul coordonnées virtuelles */
    calculerCoordonneesVirtuelles(&coordMonstre, &posX, &posY, dimImage);
    /* calcul de l'échelle */
    calculerDimensionsEchelle(&propX, &propY, dimLutin);
    /* on  affiche le monstre  */
    /* on a le même lutin pour tous les monstres, 
    * mais on change les couleurs selon le type.
    */
    GL_changerCouleurTrait(COULEUR_MONSTRE[monstre->type]);
        glPushMatrix();
            /* affiche le monstre au bon endroit */
            glTranslatef(posX, posY, 0.);
            glScalef(propX, propY, 1.);
            afficherElement(idAffichage);
        glPopMatrix();
        /* on réinitialise la couleur */
    GL_changerCouleurTrait(COULEUR_PARDEFAUT);
}

void afficherTour(Tour *tour, GLuint idAffichage, Dimensions *dimLutin, Dimensions *dimImage)
{
    /* la correspondance entre coordonnées (x,y) et position sur l'écran */
    double posX, posY;
    /* affichage prenant en compte les coordonnées de l'élément */
    double angle;
    double mPosX, mPosY;
    Point coordCible;
    unsigned char couleurTir[NB_COULEURS];
    /*  */
    double propX, propY;
    calculerDimensionsEchelle(&propX, &propY, dimLutin);
    /* */
    calculerCoordonneesVirtuelles(tour->coord, &posX, &posY, dimImage);
    /* on calcule l'angle par rapport à la cible */
    if( tour->cible )
    {
        /* on calcule la position de la cible, l'angle */
        calculerPositionMonstre(tour->cible, &coordCible);
        angle = calculerAngle(tour->coord, &coordCible);
        calculerCoordonneesVirtuelles(&coordCible, &mPosX, &mPosY, dimImage);

        /** Dessin **/
        /* ligne de mire*/
        calculerCouleurTir(couleurTir, tour);
        dessinerSegment(posX, posY, mPosX, mPosY, couleurTir);
        /** **/
    }
    glPushMatrix();
        glTranslatef(posX, posY, 0.);
        //ICI ÉCHELLE
        glScalef(propX, propY, 1.);
        if( tour->cible )
        {
            /* on fait pointer la tour vers sa cible */
             glRotatef(angle, 0., 0., 1.);
             /* on dessine la ligne de mire */
        }
        /* on appelle la liste d'affichage qui contient le lutin
        * la fonction mettra aussi le lutin à l'échelle
         */
        afficherElement(idAffichage);
    glPopMatrix();
}

void afficherImageRang(int rang, GLuint idAffichage, Point *coordRang, Dimensions *dimRang, Dimensions *dimImage)
{
    double posX, posY;
    double mPosX, mPosY;
    /*  */
    double propX, propY;
    Point coordEchelle;
    Point dimEchelle;
    calculerCoordonneesPourcentage(&coordEchelle, coordRang, dimImage);
    calculerCoordonneesVirtuelles(&coordEchelle, &posX, &posY, dimImage);
    /* */
    calculerDimensionsPourcentage(&dimEchelle, dimRang, dimImage);
    calculerDimensionsEchelleImage(&propX, &propY, &dimEchelle, dimImage);
    /* */
    glPushMatrix();
        glTranslatef(posX, posY, 0.);
        //ICI ÉCHELLE
        glScalef(propX, propY, 1.);
        afficherElement(idAffichage);
    glPopMatrix();
}


void afficherElement(GLuint idAffichage)
{
    /* */
    /* on appelle la liste d'affichage qui contient l'élément */
    glCallList(idAffichage);
}


/////TEXTES////////////
char *allouerTexte(int taille)
{
    char *texte = calloc(taille, sizeof(char));
    if( !texte )
    {
        printf("Texte -- Échec d'allocation dynamique (taille = %d).\n", taille);
        exit(EXIT_FAILURE);
    }
    return texte;
}

void libererTexte(char *texte)
{
    free(texte);
}

void afficherNiveau(unsigned char niveau, Dimensions *dimImage)
{
     char *texteNiveau = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        sprintf(texteNiveau, "%s%hhu", TEXTE_NIVEAU, niveau);
        afficherTexte( texteNiveau, &POSITION_TEXTE_NIVEAU, COULEUR_TEXTE_NIVEAU,  dimImage);
    libererTexte(texteNiveau);
}

void afficherPointage(int pointage, Dimensions *dimImage)
{
    char *textePointage = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        sprintf(textePointage, "%s%d", TEXTE_POINTAGE, pointage);
        afficherTexte( textePointage, &POSITION_TEXTE_POINTAGE, COULEUR_TEXTE_POINTAGE, dimImage);
    libererTexte(textePointage);
}

void afficherArgent(int argent, Dimensions *dimImage)
{
    char *texteArgent = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        sprintf(texteArgent, "%s%d", TEXTE_ARGENT, argent);
        afficherTexte( texteArgent, &POSITION_TEXTE_ARGENT, COULEUR_TEXTE_ARGENT, dimImage);
    libererTexte(texteArgent);
}

void afficherTexteRang(int rang, Dimensions *dimImage)
{
    char *texteRang = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        strcpy(texteRang, TEXTE_RANG);
        strcat(texteRang, NOM_RANGS[rang]);
        afficherTexte( texteRang, &POSITION_TEXTE_RANG, COULEUR_TEXTE_RANG, dimImage);
    libererTexte(texteRang);
}

void afficherEtatJeu(int etatJeu, Dimensions *dimImage)
{
    char *texteEtat = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        strcpy(texteEtat, TEXTES_ETAT_JEU[etatJeu]);
        afficherTexte( texteEtat, &POSITION_TEXTE_ETAT_JEU, COULEUR_TEXTE_ETAT_JEU, dimImage);
    libererTexte(texteEtat);
}

void afficherTitreMenu(Dimensions *dimImage)
{
    char *texteTitre = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        strcpy(texteTitre, TEXTE_TITRE_MENU);
        afficherTexte( texteTitre, &POSITION_TEXTE_TITRE_MENU, COULEUR_TEXTE_TITRE_MENU, dimImage);
    libererTexte(texteTitre);
}

void afficherTexteBoutonMenu(Dimensions *dimImage)
{
    char *texteBouton = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        strcpy(texteBouton, TEXTE_BOUTON_MENU);
        afficherTexte( texteBouton, &POSITION_TEXTE_BOUTON_MENU, COULEUR_TEXTE_BOUTON_MENU, dimImage);
    libererTexte(texteBouton);
}

void afficherCredits(Dimensions *dimImage)
{
    int i;
    char *texteCredits = allouerTexte(MAX_TAILLE_TEXTE);
    Point *coordCredits =  copiePoint(&POSITION_CREDITS);
        for( i=0; i<NB_CREDITS; i++ )
        {
            coordCredits->y += DECALAGE_TEXTES_CREDITS;
            strcpy(texteCredits, TEXTES_CREDITS[i]);;
            afficherTexte( texteCredits, coordCredits, COULEUR_TEXTE_CREDITS, dimImage);
        }
    libererPoint(coordCredits);
    libererTexte(texteCredits);
}
void afficherTouche(char toucheCode, Dimensions *dimImage)
{
    char *texteTouche = allouerTexte(MAX_TAILLE_TEXTE);
        /**** PROVOQUE PARFOIS UNE ERREUR D'ALLOCATION, 
        **** ou corrupted size vs. prev_size
        **** à la sortie du programme
        **** À CORRIGER
        ****/
        strcpy(texteTouche, TEXTE_TOUCHE);
        strncat(texteTouche, &toucheCode, 1);
        //sprintf(texteTouche, "%s : %c", TEXTE_TOUCHE, toucheCode);
        afficherTexte( texteTouche, &POSITION_TEXTE_TOUCHE, COULEUR_TEXTE_TOUCHE, dimImage);
    libererTexte(texteTouche);
}


void afficherTexte(char *texte, Point *origine, unsigned char couleurTexte[], Dimensions *dimImage)
{
    GL_changerCouleurTrait(couleurTexte);
        GLUT_afficherTexte(texte, origine, GLUT_BITMAP_HELVETICA_18, dimImage);
    GL_changerCouleurTrait(COULEUR_PARDEFAUT);
}

void GLUT_afficherTexte(char *texte, Point *origine, void *police, Dimensions *dimImage)
{
    double posX, posY;
    Point origineEchelle;
    /* pourcentage puis virtuelles */
    calculerCoordonneesPourcentage(&origineEchelle, origine, dimImage);
    calculerCoordonneesVirtuelles(&origineEchelle, &posX, &posY, dimImage);
    glRasterPos2f((GLfloat)posX, (GLfloat)posY); // Positionne le premier caractère de la chaîne
    int c;
    c=0;
    while( texte[c] != '\0' )
    {
        glutBitmapCharacter(police, texte[c]); // Affiche chaque caractère de la chaîne
        c++;
    }
}