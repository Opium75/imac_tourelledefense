#include "../include/gestion_affichage.h"

Ressources* allouerRessources()
{
    Ressources *ressources = malloc( sizeof(Ressources) );
    if( !ressources )
    {
        printf("Ressources -- échec d'allocation dynamique.\n");
        exit(EXIT_FAILURE);
    }
    return ressources;
}

void libererRessources(Ressources *ressources)
{
    /* qqc */
    libererRessourcesAffichage(ressources);
    free(ressources);
}


void lancerAffichage(SDL_Surface **scene)
{
    /* Initialisation SDL*/
    if( -1 == SDL_Init(SDL_INIT_VIDEO) )
    {
        printf("Eh non en fait casse-toi\n");
        exit(EXIT_FAILURE);
    }
    int temp = 1;
    char *tempC[] = {"Coucou c'est un message, peut-être ?"};
    glutInit(&temp, tempC);

   
    /*ouverture fenêtre et création contexte OpenGL*/
    redimensionner(scene, LARGEUR_FENETRE, HAUTEUR_FENETRE); /* valeurs par défaut*/
    SDL_WM_SetCaption(TITRE_FENETRE, NULL); /* titre fenêtre*/

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
}

void fermerAffichage(SDL_Surface *scene)
{
    SDL_FreeSurface(scene);
    SDL_Quit();
}

void chargerRessourcesAffichage(Ressources *ressources, Dimensions *dimImage, bool possedeArrierePlan, char nomArrierePlan[])
{
    
    /* création des listes des types de lutins, et dimensions */
    TypeLutin listeType[NB_LUTINS];
    remplirListeType(listeType);
    remplirListeDimensions(ressources->listeDim, listeType);
    /* */
    /* chargement des textures */
    chargerTextureLutins(ressources->lutins, ressources->banqueTextures);
    chargerAffichageLutins(ressources->banqueAffichage, ressources->banqueTextures);
    if( possedeArrierePlan )
    {
        chargerTextureArrierePlan(&(ressources->arrierePlan), &(ressources->textureArrierePlan), nomArrierePlan);
        chargerAffichageArrierePlan(&(ressources->affichageArrierePlan), ressources->textureArrierePlan, dimImage);
    }
    /* */

    /* chargement des rangs */
    chargerTextureRangs(ressources->rangs, ressources->rangTextures);
    chargerAffichageRangs(ressources->rangAffichage, ressources->rangTextures);
    /* */
}

void  chargerAffichageArrierePlan(GLuint *idAffichage, GLuint idTexture, Dimensions *dimImage)
{
    *idAffichage = glGenLists(1);
    /* dessiner les lutins */
    glNewList(*idAffichage, GL_COMPILE);
        dessinerTexture(idTexture);
    glEndList();
}


void chargerAffichageLutins(GLuint banqueAffichage[], GLuint banqueTextures[])
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
            dessinerTexture(banqueTextures[k]);
        glEndList();
    }
}

void chargerAffichageRangs(GLuint rangAffichage[], GLuint rangTextures[])
{
     int k;
   
    for( k=0; k<NB_RANGS; k++)
    {
        /* création d'une liste d'affichage */
        rangAffichage[k] = glGenLists(1);
        /* dessiner les lutins */
        glNewList(rangAffichage[k], GL_COMPILE);
            dessinerTexture(rangTextures[k]);
        glEndList();
    }
}


void chargerTextureLutins(SDL_Surface *lutins[], GLuint banqueTextures[])
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
 
void chargerTextureRangs(SDL_Surface *rangs[], GLuint rangTextures[])
{
    int k;
    GLuint banqueRang[NB_RANGS];
    glGenTextures( (GLsizei) NB_RANGS, banqueRang);
    for( k=0; k<NB_RANGS; k++ )
    {
        rangs[k]  = chargerTextureRang( banqueRang[k], k);
    }
    for( k=0; k<NB_RANGS; k++ )
    {
        rangTextures[k] = banqueRang[k];
    }
}

SDL_Surface* chargerTextureRang(GLuint idTexture, int indice)
{
    SDL_Surface *rang;
    /** CHEMIN **/
    char cheminRang[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminRang(cheminRang, indice);
    /** VÉRIF **/
    printf("Chemin rang : %s\n", cheminRang);
    /** CHARGEMENT MÉMOIRE VIVE **/
    rang = IMG_Load(cheminRang);
    if( !rang )
    {
        printf("rang -- Échec d'ouverture au chemin : %s.\n", cheminRang);
        exit(EXIT_FAILURE);
    }
    /** CHARGEMENT MÉMOIRE GRAPHIQUE (?) **/
    glBindTexture(GL_TEXTURE_2D, idTexture);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D( GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    rang->w,
                    rang->h,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    rang->pixels
                    );
    glBindTexture(GL_TEXTURE_2D, 0);
    return rang;
}

void chargerTextureArrierePlan(SDL_Surface **arrierePlan, GLuint *idTexture, char nomArrierePlan[])
{
    /* CHEMIN */
    char cheminArrierePlan[MAX_TAILLE_CHEMIN_FICHIER];
    sprintf(cheminArrierePlan,"%s/%s",REP_ARRIEREPLAN_CARTE, nomArrierePlan);
    printf("Chemin arrière-plan : %s\n", cheminArrierePlan);
    *arrierePlan = IMG_Load(cheminArrierePlan);
    if( !arrierePlan )
    {
        printf("Arrière-plan -- Échec d'ouverture au chemin : %s.\n", cheminArrierePlan);
        exit(EXIT_FAILURE);
    }
    /* */
    glGenTextures( (GLsizei) 1, idTexture);
    /** CHARGEMENT MÉMOIRE GRAPHIQUE (?) **/
    glBindTexture(GL_TEXTURE_2D, *idTexture);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D( GL_TEXTURE_2D,
                    0,
                    GL_RGBA,
                    (*arrierePlan)->w,
                    (*arrierePlan)->h,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    (*arrierePlan)->pixels
                    );
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

SDL_Surface* chargerTextureLutin(GLuint idTexture, TypeLutin *type)
{
    SDL_Surface *lutin;
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

void libererRessourcesAffichage(Ressources *ressources)
{
    int k;
    /* ON LIBÈRE LES TEXTURES PUIS LES IMAGES */
    glDeleteTextures(1, &(ressources->textureArrierePlan));
    glDeleteLists(ressources->affichageArrierePlan, 1);

    SDL_FreeSurface(ressources->arrierePlan);
    /* */
    glDeleteTextures(NB_LUTINS, ressources->banqueTextures);
    glDeleteLists(ressources->banqueAffichage[0], NB_LUTINS);
    /* */
    for( k=0; k<NB_LUTINS; k++ )
    {
        SDL_FreeSurface(ressources->lutins[k]);
    }

    glDeleteTextures(NB_RANGS, ressources->rangTextures);
    glDeleteLists(ressources->rangAffichage[0], NB_RANGS);
    for( k=0; k<NB_RANGS; k++ )
    {
        SDL_FreeSurface(ressources->rangs[k]);
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

void GL_changerCouleurTrait(const unsigned char couleur[])
{
  glColor3f(couleur[0]/(double)MAX_VAL_COULEUR,
              couleur[1]/(double)MAX_VAL_COULEUR,
                couleur[2]/(double)MAX_VAL_COULEUR);
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

void dessinerSegment(double x1, double y1, double x2, double y2, unsigned char couleur[NB_COULEURS])
{
    GL_changerCouleurTrait(couleur);
        glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
            glBegin(GL_LINES);
                glVertex2f(x1, y1);
                glVertex2f(x2, y2);
            glEnd();
        glDisable(GL_BLEND);
    GL_changerCouleurTrait(COULEUR_PARDEFAUT);    
}

void calculerDimensionsEchelle(double *propX, double *propY, Dimensions *dimLutin)
{
    *propX = dimLutin->x/(double)LARGEUR_FENETRE;
    *propY = dimLutin->y/(double)HAUTEUR_FENETRE;
}

void dessinerTexture(GLuint idTexture)
{
    /* On va la dessiner en carré, aux dimensions de la fenêtre virtuelle.  */
    double debut = 0., taille = 1.;
    double largeur = GL_VUE_LARGEUR, hauteur = GL_VUE_HAUTEUR;
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

void calculerCouleurTir(unsigned char couleurTir[], Tour *tour)
{
    double charge = tour->tempsTir_acc / (double) tour->tempsTir;
    /* on copie la couleur par défaut du trait */
    copierCouleur(couleurTir, COULEUR_TRAIT[tour->type]);
    homothetieCouleur(couleurTir, charge);
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
    strncat(cheminLutin, EXTENSION, MAX_TAILLE_NOM_FICHIER);
}

void correspondanceCheminRang(char *cheminRang, int indice)
{
    /* on construit le chemin du RANG */
    strcpy(cheminRang, REP_RANG);
    strcat(cheminRang, NOM_IMAGE_RANG[indice]);
    strncat(cheminRang, EXTENSION, MAX_TAILLE_NOM_FICHIER);
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