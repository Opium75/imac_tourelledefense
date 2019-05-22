
#include "../include/afficheElements.h"

void afficherVague(Vague *vague, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    int k;
    Monstre *monstre;
    for( k=0; k<vague->nombreMonstres; k++ )
    {
        monstre = vague->monstres[k];
        if( monstre->etat == enMouvement )
            afficherMonstre(monstre, banqueAffichage, banqueTextures, dimImage);
    }
}

void afficherChaine(Chaine chaine, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /** on se contentera d'affciher la première vague lancée */
    while( chaine && chaine->etat != lancee )
        chaine = chaine->suivante;
    if( chaine )
        afficherVague(chaine, banqueAffichage, banqueTextures, dimImage);
}

void afficherCite(Cite *cite, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /** on affichera pour l'instant que la liste des tours */
    
    afficherListeTour(cite->listeTour, banqueAffichage, banqueTextures, dimImage);

}

void afficherListeTour(ListeTour liste, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{

    while( liste )
    {
        afficherTour(liste, banqueAffichage, banqueTextures, dimImage);   
        liste = liste->suivante;
    }
}

void chargerRessourcesAffichage(SDL_Surface *lutins[], GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /* création des listes des types de lutins, et dimensions */
    TypeLutin type;
    TypeLutin listeType[NB_LUTINS];
    Dimensions listeDim[NB_LUTINS];
    remplirListeType(listeType);
    remplirListeDimensions(listeDim, listeType);
    /* */
    /* chargement des textures */
    chargerToutesTexturesLutins(lutins, banqueTextures);
    remplirBanqueAffichage(banqueAffichage, banqueTextures, listeType, listeDim, dimImage);
}

void remplirBanqueAffichage(GLuint banqueAffichage[], GLuint banqueTextures[], TypeLutin listeType[], Dimensions listeDim[], Dimensions *dimImage)
{
    int k;
    /***    POURQUOI ÀÇA MARCHE PAS ARHJAGR ***/
    /* on crée la liste d'affichage pour leS lutinS */
   
    for( k=0; k<NB_LUTINS; k++)
    {
        /* création d'une liste d'affichage */
         banqueAffichage[k] = glGenLists(1);
        /* dessiner les lutins */
        glNewList(banqueAffichage[k], GL_COMPILE);
            dessinerLutinEchelle(banqueTextures[k], &listeType[k], &listeDim[k]);
        glEndList();
    }
}

void chargerToutesTexturesLutins(SDL_Surface *lutins[], GLuint banqueTextures[])
{
    /** SUITE À UNE ERREUR NON RÉSOLUE, QUI CORROMPT banqueTextures (chargée en tas, si c'est utile à savoir),
    * on utilise une banqueInter (chargée donc en pile) pour charger les textures,
    * puis on la copie dans banqueTextures.
    * Ça fonctionne donc bon..
    **/
    int k, j;
    TypeLutin type;
    GLuint idTexture;
    GLuint banque[NB_LUTINS];
    glGenTextures( (GLsizei) NB_LUTINS, banque);
    for( k=0; k<NB_LUTINS; k++ )
    {
        type = correspondanceTypeLutin(k);
        lutins[k]  = chargerTextureLutin( banque[k], &type );
        /* la corruption des deux premiers indices 
        * se fait pendant ce chargement ? AAAAAAAAAAAAAAAAAAAAAAAAAA
        * (alors que banqueTextures n'est pas en argument)
        * aherharhez
        */
    }
    for( k=0; k<NB_LUTINS; k++ )
    {
        banqueTextures[k] = banque[k];
    }
    /* un grand mystère. */
}

SDL_Surface* chargerTextureLutin(GLuint idTexture, TypeLutin *type)
{
    SDL_Surface* lutin;
    /** CHEMIN **/
    char cheminLutin[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminLutin(cheminLutin, type);
    /** VÉRIF **/
   printf("Chemin lutin : %s\n", cheminLutin);
    /** CHARGEMENT MÉMOIRE VIVE **/
    lutin = IMG_Load(cheminLutin);
    if( !lutin )
    {
        printf("Lutin -- Échec d'ouverture au chemin : %s.\n", cheminLutin);
        exit(EXIT_FAILURE);
    }
    /** CHARGEMENT MÉMOIRE GRAPHIQUE (?) **/
    glBindTexture(GL_TEXTURE_2D, idTexture);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D( GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    lutin->w,
                    lutin->h,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    lutin->pixels
                    );
    glBindTexture(GL_TEXTURE_2D, 0);
    return lutin;
}

void libererRessourcesAffichage(SDL_Surface *lutins[],  GLuint banqueAffichage[], GLuint banqueTextures[])
{
    int k;
    /* ON LIBÈRE LES TEXTURES PUIS LES IMAGES */
    glDeleteTextures(NB_LUTINS, banqueTextures);
    glDeleteLists(banqueAffichage[0], NB_LUTINS);
    for( k=0; k<NB_LUTINS; k++ )
    {
        SDL_FreeSurface(lutins[k]);
    }
    
}

void remplirListeType(TypeLutin listeType[])
{
    int k;
    for( k=0; k<NB_LUTINS; k++ )
    {
        listeType[k] = correspondanceTypeLutin(k);
    }
}

void remplirListeDimensions(Dimensions listeDim[], TypeLutin listeType[])
{
    int k;
    for( k=0; k<NB_LUTINS; k++ )
    {
        switch( (listeType[k]).nature )
        {
            case LUT_tour :
                listeDim[k].x = BASE_TAILLE_TOUR;
                listeDim[k].y =  BASE_TAILLE_TOUR;
                break;
            case LUT_monstre :
                listeDim[k].x = BASE_TAILLE_MONSTRE;
                listeDim[k].y = BASE_TAILLE_MONSTRE;
                break;
            default :
                printf("Échec de reconnaissance d'énumération (TypeLutin:nature).\n");
                exit(EXIT_FAILURE);
        }
    }
}


void afficherMonstre(Monstre *monstre, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /* on crée le type de lutin de toute pièce */
    TypeLutin type;
    type.nature = LUT_monstre;
    type.typeMonstre = monstre->type;

    /* on récupère les coordonnées du monstre */
    Point coordMonstre;
    calculerPositionMonstre(monstre, &coordMonstre);
    /* on  affiche le monstre  */
    afficherElement(&type, &coordMonstre, banqueAffichage, banqueTextures, dimImage );
}

void afficherTour(Tour *tour, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /* on crée le type de lutin de toute pièce */
    TypeLutin type;
    type.nature = LUT_tour;
    type.typeTour = tour->type;
    /* on  affiche le monstre  */
    afficherElement(&type, tour->coord, banqueAffichage, banqueTextures, dimImage);
    
}

void afficherElement(TypeLutin *type, Point *coord, GLuint banqueAffichage[], GLuint banqueTextures[],  Dimensions *dimImage)
{
    /* l'indice du dessin dans la banque d'affichage */
  
    int indice = correspondanceIndiceLutin(type);
    /* la correspondance entre coordonnées (x,y) et position sur l'écran */
    double posX, posY;

    calculerCoordonneesVirtuelles(coord, &posX, &posY, dimImage);
    /* affichage prenant en compte les coordonn)ées de l'élémenent */
     
    glPushMatrix();
        /* On part du point en haut à gauche
        * de la fenêtre virtuelle,
        * d'où les deux soustractions
        */
        glTranslatef(posX, posY, 0.);
        /* on appelle la liste d'affichage qui contient le lutin */
        glPushMatrix();
             glCallList(banqueAffichage[indice]);
        glPopMatrix();
    glPopMatrix();
}

void calculerCoordonneesVirtuelles(Point *coord, double *posX, double *posY, Dimensions *dimImage)
{
    *posX = ( ((double)coord->x/(double)dimImage->x)*GL_VIEW_WIDTH - GL_VIEW_WIDTH/2 );
    *posY = -( ((double)coord->y/(double)dimImage->y)*GL_VIEW_HEIGHT - GL_VIEW_HEIGHT/2 );
}

void calculerCoordonneesEchelle(Point *coord, int x, int y, Dimensions *dimImage)
{
    /* le coordonnées reçues sont relative à la fenêtre -> WINDOW_WIDTH */
    coord->x = (unsigned int)( (x/(double)WINDOW_WIDTH)*dimImage->x );
    coord->y = (unsigned int)( (y/(double)WINDOW_HEIGHT)*dimImage->y );
}


void  dessinerLutinEchelle(GLuint idTexture, TypeLutin *type, Dimensions *dimLutin)
{
    /* on prend ici en compte la taille de l'image, de l'écran etc..*/
    double propX, propY;
    propX = dimLutin->x/(double)WINDOW_WIDTH;
    propY = dimLutin->y/(double)WINDOW_HEIGHT;
    /** DESSIN **/

    glPushMatrix();
       glScalef(propX, propY, 1.);
            /* on le dessine en prenant en compte les transformations d'échelle */
            dessinerLutin(idTexture, type);
    glPopMatrix();
}


void dessinerLutin(GLuint idTexture, TypeLutin *type)
{
    /* On va le dessiner en carré, aux dimensions de la fenêtre virtuelle.  */
    double debut = 0., taille = 1.;
    double largeur = GL_VIEW_WIDTH, hauteur = GL_VIEW_HEIGHT;
    if( type->nature == LUT_monstre )
    {
        /* on a le même lutin pour tous les monstres, 
        * mais on change les couleurs selon le type.
        */
         glColor3f(COULEUR_MONSTRE[type->typeMonstre][0], COULEUR_MONSTRE[type->typeMonstre][1], COULEUR_MONSTRE[type->typeMonstre][2]);
    }
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glBindTexture(GL_TEXTURE_2D, idTexture);
            glBegin(GL_QUADS);
                glTexCoord2f(debut, debut);
                    glVertex2f(-largeur/2, hauteur/2);
               glTexCoord2f(debut + taille, debut);
                    glVertex2f(largeur/2, hauteur/2);
               glTexCoord2f(debut + taille, debut + taille);
                    glVertex2f(largeur/2, -hauteur/2);
               glTexCoord2f(debut, debut + taille);
                    glVertex2f(-largeur/2, -hauteur/2);
            glEnd();    
        glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    if( type->nature == LUT_monstre )
    {
        /* on réinitialise la couleur */
        glColor3f(COULEUR_PARDEFAUT[0], COULEUR_PARDEFAUT[1], COULEUR_PARDEFAUT[2]);
    }
}


void libererToutesTexturesLutins(GLuint banqueTextures[])
{
    glDeleteTextures(NB_LUTINS, banqueTextures);
}

void libererToutesImagesLutins(SDL_Surface *lutins[])
{
    int k;
    for( k=0; k<NB_LUTINS; k++ )
    {
        SDL_FreeSurface(lutins[k]);
    }
}

TypeLutin correspondanceTypeLutin(int indice)
{
    TypeLutin type;
    int k;
    if( indice < NB_LUTINS_MONSTRE )
    {
        type.nature = LUT_monstre;
        type.typeMonstre = indice;
    }
    else if( indice < NB_LUTINS )
    {
        type.nature = LUT_tour;
        type.typeTour = indice - NB_LUTINS_MONSTRE;
    }
    return type;
}

int correspondanceIndiceLutin(TypeLutin *type)
{
    int indice;
    if( type->nature == LUT_monstre )
    {
        indice = type->typeMonstre;

    }
    else if( type->nature == LUT_tour )
    {
        indice = type->typeTour + NB_LUTINS_MONSTRE;
    }
    return indice;
}


void correspondanceCheminLutin(char *cheminLutin, TypeLutin *type)
{
    int indice;
    indice = correspondanceIndiceLutin(type);

    /* on construit le chemin du lutin */
    strcpy(cheminLutin, REP_LUTIN);
   
    /* non suivant le type du lutin */
    switch(type->nature)
    {
        case LUT_tour :
            strncat(cheminLutin, CHEMIN_IMAGE_TOUR, MAX_TAILLE_CHEMIN_FICHIER);
            strncat(cheminLutin, NOM_IMAGE_TOUR[type->typeTour], MAX_TAILLE_NOM_FICHIER);
            break;
        case LUT_monstre :
            /* on a le même lutin pour tous les types de monstre
            * mais on changera sa couleur avant de le dessiner
            */
            strncat(cheminLutin, CHEMIN_IMAGE_MONSTRE, MAX_TAILLE_CHEMIN_FICHIER);
            strncat(cheminLutin, NOM_IMAGE_MONSTRE[0], MAX_TAILLE_NOM_FICHIER);
            break;
        default :
            printf("NOoooon\n");
            exit(EXIT_FAILURE);
    }
    strncat(cheminLutin, EXTENTION, MAX_TAILLE_NOM_FICHIER);
}