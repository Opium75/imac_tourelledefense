
#include "../include/afficheElements.h"

void drawOrigin()
{
    glBegin(GL_LINES);
        /* Abscisses*/
        glColor3f(255,0,0);
        glVertex2f(0,0);
        glVertex2f(10,0);
        /* Ordonnées*/
        glColor3f(0,255,0);
        glVertex2f(0,0);
        glVertex2f(0,10);
    glEnd();
}

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
void afficherListeTour(ListeTour liste, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    while( liste )
    {
        afficherTour(liste, banqueAffichage, banqueTextures, dimImage);
        liste = liste->suivante;
    }
}

void chargerRessourcesAffichage(SDL_Surface *lutins[], GLuint banqueAffichage[], GLuint banqueTextures[])
{
    /* création des listes des types de lutins, et dimensions */
    TypeLutin listeType[NB_LUTINS];
    Dimensions listeDim[NB_LUTINS];
    remplirListeType(listeType);
    remplirListeDimensions(listeDim, listeType);
    /* */

    /* chargement des textures */
    chargerToutesTexturesLutins(lutins, banqueTextures);
    remplirBanqueAffichage(banqueAffichage, banqueTextures, listeType, listeDim);
}

void libererRessourcesAffichage(SDL_Surface *lutins[],  GLuint banqueAffichage[], GLuint banqueTextures[])
{
    int k;
    /* ON LIBÈRE LES TEXTURES PUIS LES IMAGES */
    glDeleteTextures(NB_LUTINS, banqueTextures);
    glDeleteLists(banqueAffichage, NB_LUTINS);
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
    int k, indice;
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



void afficherCite(Cite *cite, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /** on affichera pour l'instant que la liste des tours */
    afficherListeTour(cite->listeTour, banqueAffichage, banqueTextures, dimImage);
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

    afficherElement(type, &coordMonstre, banqueAffichage, banqueTextures, dimImage);
}

void afficherTour(Tour *tour, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
     int indice;
    /* on crée le type de lutin de toute pièce */
    TypeLutin type;
    type.nature = LUT_tour;
    type.typeTour = tour->type;
    /* on  affiche le monstre  */
    afficherElement(type, tour->coord, banqueAffichage, banqueTextures, dimImage);
}

void afficherElement(TypeLutin type, Point *coord, GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
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
        glTranslatef(posX - GL_VIEW_WIDTH/2, -(posY - GL_VIEW_HEIGHT/2), 0.);
        /* on appelle la liste d'affichage qui contient le lutin */
        glPushMatrix();
             glCallList(banqueAffichage[indice]);
        glPopMatrix();
    glPopMatrix();
}

void calculerCoordonneesVirtuelles(Point *coord, double *posX, double *posY, Dimensions *dimImage)
{
    *posX = (coord->x/(double)dimImage->x)*GL_VIEW_WIDTH;
    *posY = (coord->y/(double)dimImage->y)*GL_VIEW_HEIGHT;
}

void remplirBanqueAffichage(GLuint banqueAffichage[], GLuint banqueTextures[], TypeLutin listeType[], Dimensions listeDim[])
{
    int k, indice;
    GLuint id;
    /***    POURQUOI ÀÇA MARCHE PAS ARHJAGR ***/
    for( k=0; k<NB_LUTINS; k++)
    {
        /* on crée la liste d'affichage pour le lutin */
        banqueAffichage[k] = glGenLists(1);
        /* dessiner les lutins */
        glNewList(banqueAffichage[k], GL_COMPILE);
            dessinerLutinEchelle(banqueTextures[k], listeType[k], listeDim[k]);
        glEndList();
    }
}


void  dessinerLutinEchelle(GLuint idTexture, TypeLutin type, Dimensions dim)
{
    /* on prend ici en compte la taille de l'image, de l'écran etc..*/
    double propX, propY;
    int centrerX, centrerY;
    centrerX = -(int)dim.x/2;
    centrerY = -(int)dim.y/2;
    printf("(%d, %d)\n", centrerX, centrerY);
    propX = dim.x/(double)WINDOW_WIDTH;
    propY = dim.y/(double)WINDOW_WIDTH;
    /** DESSIN **/
    
    glPushMatrix();
        glScalef(propX, propY, 1.);
        glPushMatrix();
            glTranslatef(centrerX, centrerY, 0.);
            /* on le dessine en prenant en compte les transformations d'échelle */
            dessinerLutin(idTexture, type);
        glPopMatrix();
    glPopMatrix();
}


void dessinerLutin(GLuint idTexture, TypeLutin type)
{
    /* On va le dessiner en carré, aux dimensions de la fenêtre virtuelle.  */
    double debut = 0., taille = 1.;
    double largeur = GL_VIEW_WIDTH, hauteur = GL_VIEW_HEIGHT;
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
}

void chargerToutesTexturesLutins(SDL_Surface *lutins[], GLuint banqueTextures[])
{
    int k;
    TypeLutin type;
    for( k=0; k<NB_LUTINS; k++ )
    {
        type = correspondanceTypeLutin(k);
        lutins[k]  = chargerTextureLutin( &(banqueTextures[k]), type );
    }
}

SDL_Surface* chargerTextureLutin(GLuint *idTexture, TypeLutin type)
{
    SDL_Surface* lutin; 
    /** CHEMIN **/
    char cheminLutin[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminLutin(cheminLutin, type);
    /** VÉRIF **/
    printf("%s\n", cheminLutin);
    /** CHARGEMENT MÉMOIRE VIVE **/
    lutin = IMG_Load(cheminLutin);
    if( !lutin )
    {
        printf("Lutin -- Échec d'ouverture au chemin : %s.\n", cheminLutin);
        exit(EXIT_FAILURE);
    }
    /** CHARGEMENT MÉMOIRE GRAPHIQUE (?) **/
    glGenTextures( (GLsizei) 1, idTexture);
    glBindTexture(GL_TEXTURE_2D, *idTexture);
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
        type.nature = LUT_monstre;
    else if( indice < NB_LUTINS_MONSTRE + NB_LUTINS_TOUR )
    {
        type.nature = LUT_tour;
        type.typeTour = indice - NB_LUTINS_MONSTRE;
    }
    return type;
}

int correspondanceIndiceLutin(TypeLutin type)
{
    int indice;
    if( type.nature == LUT_monstre )
    {
        indice = 0;
    }
    else if( type.nature == LUT_tour )
    {
        indice = type.typeTour;
    }
    return indice;
}


void correspondanceCheminLutin(char *cheminLutin, TypeLutin type)
{
    int indice;
    indice = correspondanceIndiceLutin(type);
    /* on construit le chemin du lutin */
    strcpy(cheminLutin, REP_LUTIN);
   
    /* non suivant le type du lutin */
    switch(type.nature)
    {
        case LUT_tour :
            strncat(cheminLutin, CHEMIN_IMAGE_TOUR, MAX_TAILLE_CHEMIN_FICHIER);
            strncat(cheminLutin, NOM_IMAGE_TOUR[type.typeTour], MAX_TAILLE_NOM_FICHIER);
            break;
        case LUT_monstre :
            strncat(cheminLutin, CHEMIN_IMAGE_MONSTRE, MAX_TAILLE_CHEMIN_FICHIER);
            strncat(cheminLutin, NOM_IMAGE_MONSTRE[0], MAX_TAILLE_NOM_FICHIER);
            break;
        default :
            printf("NOoooon\n");
            exit(EXIT_FAILURE);
    }
    strncat(cheminLutin, EXTENTION, MAX_TAILLE_NOM_FICHIER);
}