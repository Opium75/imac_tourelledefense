#include "../include/gestion_affichage.h"



SDL_Surface* lancerAffichage(void)
{
    /* Initialisation SDL*/
    if( -1 == SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Eh non en fait casse-toi");
        exit(EXIT_FAILURE);
    }

    /*ouverture fenêtre et création contexte OpenGL*/
    SDL_Surface *scene;

    redimensionner(&scene, LARGEUR_FENETRE, HAUTEUR_FENETRE); /* valeurs par défaut*/
    SDL_WM_SetCaption(TITRE_FENETRE, NULL); /* titre fenêtre*/

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);

    return scene;
}

void fermerAffichage(SDL_Surface *scene)
{
    SDL_FreeSurface(scene);
    SDL_Quit();
}



void chargerRessourcesAffichage(SDL_Surface *lutins[], GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage)
{
    /* création des listes des types de lutins, et dimensions */
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
    int k;
    TypeLutin type;
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


void calculerCoordonneesVirtuelles(Point *coord, double *posX, double *posY, Dimensions *dimImage)
{
    *posX = ( ((double)coord->x - (double)dimImage->x/2)/(double)dimImage->x )*GL_VUE_LARGEUR;
    *posY = -( ((double)coord->y - (double)dimImage->y/2)/(double)dimImage->y )*GL_VUE_HAUTEUR;
}

void calculerCoordonneesEchelle(Point *coord, int x, int y, Dimensions *dimImage)
{
    /* le coordonnées reçues sont relative à la fenêtre -> LARGEUR_FENETRE */
    coord->x = (unsigned int)( (x/(double)LARGEUR_FENETRE)*dimImage->x );
    coord->y = (unsigned int)( (y/(double)HAUTEUR_FENETRE)*dimImage->y );
}


void  dessinerLutinEchelle(GLuint idTexture, TypeLutin *type, Dimensions *dimLutin)
{
    /* on prend ici en compte la taille de l'image, de l'écran etc..*/
    double propX, propY;
    propX = dimLutin->x/(double)LARGEUR_FENETRE;
    propY = dimLutin->y/(double)HAUTEUR_FENETRE;
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
    double largeur = GL_VUE_LARGEUR, hauteur = GL_VUE_HAUTEUR;
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



TypeLutin correspondanceTypeLutin(int indice)
{
    TypeLutin type;
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

void redimensionner(SDL_Surface** surface, unsigned int largeur, unsigned int hauteur)
{ 
    SDL_Surface* surface_temp = SDL_SetVideoMode(   
        largeur, hauteur, BIT_PER_PIXEL,
        SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    if(NULL == surface_temp) 
    {
        fprintf(
            stderr, 
            "Erreur lors du redimensionnement de la fenetre.\n");
        exit(EXIT_FAILURE);
    }
    *surface = surface_temp;

    glViewport(0, 0, (*surface)->w, (*surface)->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        -GL_VUE_LARGEUR / 2. , GL_VUE_LARGEUR / 2. , 
        -GL_VUE_HAUTEUR / 2. , GL_VUE_HAUTEUR / 2.);
}