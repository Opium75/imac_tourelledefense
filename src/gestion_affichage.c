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
    libererRessourcesAffichage(ressources);
    free(ressources);
}

void lancerAffichage(SDL_Surface **scene)
{
    /* Initialisation SDL*/
    if( -1 == SDL_Init(SDL_INIT_VIDEO) )
    {
        printf("SDL_Init : échec de lancement de la vidéo.\n");
        printf("SDL_Init : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    /* initialisation du texte */
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

void chargerRessourcesAffichage(Ressources *ressources, Dimensions *dimImage, bool possedeDecor, char nomDecor[])
{
    
    /* création des listes des types de lutins, et dimensions */
    TypeLutin listeType[NB_LUTINS];
    remplirListeType(listeType);
    remplirListeDimensions(ressources->listeDim, listeType);
    /* */


    /* chargement des textures */
    chargerTextureLutins(ressources->lutins, ressources->banqueTextures);
    chargerAffichageLutins(ressources->banqueAffichage, ressources->banqueTextures);
    if( possedeDecor )
    {
        chargerTextureDecor(&(ressources->decor), &(ressources->textureDecor), nomDecor);
        chargerAffichageArrierePlan(&(ressources->affichageDecor), ressources->textureDecor, dimImage);
    }
    /* */

    /* chargement des arrière-plans */
    chargerTextureArrierePlans(ressources->arrierePlans, ressources->arrierePlanTextures);
    chargerAffichageArrierePlans(ressources->arrierePlanAffichage,  ressources->arrierePlanTextures, dimImage);

    /* les boutons */
    chargerTextureBoutons(ressources->boutons, ressources->boutonTextures);
    chargerAffichageBoutons(ressources->boutonAffichage,  ressources->boutonTextures);

    /* chargement des rangs */
    chargerTextureRangs(ressources->rangs, ressources->rangTextures);
    chargerAffichageRangs(ressources->rangAffichage,  ressources->rangTextures);
}

void chargerAffichage(GLuint *idAffichage, GLuint idTexture)
{
    /* création d'une liste d'affichage */
    *idAffichage = glGenLists(1);
    /* dessiner */
    glNewList(*idAffichage, GL_COMPILE);
        dessinerTexture(idTexture);
    glEndList();
}

void  chargerAffichageArrierePlan(GLuint *idAffichage, GLuint idTexture, Dimensions *dimImage)
{
    chargerAffichage(idAffichage, idTexture);
}

void chargerAffichageArrierePlans(GLuint arrierePlanAffichage[], GLuint arrierePlanTextures[], Dimensions *dimImage)
{
     int k;
    /* on crée la liste d'affichage pour leS lutinS */
   
    for( k=0; k<NB_ARRIERE_PLANS; k++)
    {
        chargerAffichageArrierePlan( &(arrierePlanAffichage[k]), arrierePlanTextures[k], dimImage);
    }
}

void chargerAffichageLutins(GLuint banqueAffichage[], GLuint banqueTextures[])
{
    int k;
    /* on crée la liste d'affichage pour leS lutinS */
    for( k=0; k<NB_LUTINS; k++)
    {
        chargerAffichage( &(banqueAffichage[k]), banqueTextures[k]);
    }
}

void chargerAffichageRangs(GLuint rangAffichage[], GLuint rangTextures[])
{
    int k;
    for( k=0; k<NB_RANGS; k++)
    {
       chargerAffichage( &(rangAffichage[k]), rangTextures[k]);
    }
}

void chargerAffichageBoutons(GLuint boutonAffichage[], GLuint boutonTextures[])
{
    int k;
    for( k=0; k<NB_BOUTONS; k++)
    {
       chargerAffichage( &(boutonAffichage[k]), boutonTextures[k]);
    }
}

void chargerTexture( SDL_Surface **image,  GLuint *idTexture, char *chemin)
{
    *image = IMG_Load(chemin);
    if( !(*image) )
    {
        printf("SDL_Image -- Échec d'ouverture au chemin : %s.\n", chemin);
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
                    (*image)->w,
                    (*image)->h,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    (*image)->pixels
                    );
    glBindTexture(GL_TEXTURE_2D, 0);
}

void chargerTextureLutins(SDL_Surface *lutins[], GLuint banqueTextures[])
{
    /** SUITE À UNE ERREUR RÉSOLUE, QUI CORROMPT banqueTextures (chargée en tas, si c'est utile à savoir),
    * on utilise une banqueInter (chargée donc en pile) pour charger les textures,
    * puis on la copie dans banqueTextures.
    * Ça fonctionne donc bon..
    **/
    /*** ERREUR RÉSOLUE : on donnait en argument l'identifiant et pas son adresse
    *** le compilateur se débrouille quand même pour faire fonctionner
    *** le code juqu'à un certain point
    ***/
    int k;
    TypeLutin type;
    /* on génère les id des textures à un autre endroit */
   // glGenTextures( (GLsizei) NB_LUTINS, banqueTextures);
    for( k=0; k<NB_LUTINS; k++ )
    {
        type = correspondanceTypeLutin(k);
        lutins[k]  = chargerTextureLutin( &(banqueTextures[k]), &type );
    }
}

void chargerTextureBoutons(SDL_Surface *boutons[], GLuint boutonTextures[])
{
    int k;
    /* ON GÉNÉRE LES TEXTURES DANS chargerTexture */
    //glGenTextures( (GLsizei) NB_BOUTONS, boutonTextures);
    for( k=0; k<NB_BOUTONS; k++ )
    {
        boutons[k]  = chargerTextureBouton( &(boutonTextures[k]), k );
    }
}
 
void chargerTextureRangs(SDL_Surface *rangs[], GLuint rangTextures[])
{
    int k;
    /* ON GÉNÉRE LES TEXTURES DANS chargerTexture */
    //glGenTextures( (GLsizei) NB_RANGS, rangTextures);
    for( k=0; k<NB_RANGS; k++ )
    {
        rangs[k]  = chargerTextureRang( &(rangTextures[k]), k);
    }
}

void chargerTextureArrierePlans(SDL_Surface *arrierePlans[], GLuint arrierePlanTextures[])
{
    int k;
    /* ON GÉNÉRE LES TEXTURES DANS chargerTexture */
    //glGenTextures( (GLsizei) NB_ARRIERE_PLANS, arrierePlanTextures);
    for( k=0; k<NB_ARRIERE_PLANS; k++ )
    {
        arrierePlans[k]  = chargerTextureArrierePlan( &(arrierePlanTextures[k]), k);
    }
}

SDL_Surface* chargerTextureRang(GLuint *idTexture, int indice)
{
    SDL_Surface *rang;
    /** CHEMIN **/
    char cheminRang[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminRang(cheminRang, indice);
    /** VÉRIF **/
    printf("Chemin rang : %s\n", cheminRang);
    /** CHARGEMENT MÉMOIRE VIVE **/
    chargerTexture(&rang, idTexture, cheminRang);
    return rang;
}



SDL_Surface* chargerTextureArrierePlan(GLuint *idTexture, int indice)
{
    SDL_Surface *arrierePlan;
    /** CHEMIN **/
    char cheminArrierePlan[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminArrierePlan(cheminArrierePlan, indice);
    /** VÉRIF **/
    printf("Chemin arrière-plan : %s\n", cheminArrierePlan);
    /** CHARGEMENT MÉMOIRE VIVE **/
    chargerTexture(&arrierePlan, idTexture, cheminArrierePlan);
    return arrierePlan;
}

SDL_Surface* chargerTextureBouton(GLuint *idTexture, int indice)
{
    SDL_Surface *bouton;
    /** CHEMIN **/
    char cheminBouton[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminBouton(cheminBouton, indice);
    /** VÉRIF **/
    printf("Chemin bouton : %s\n", cheminBouton);
    /** CHARGEMENT MÉMOIRE VIVE **/
    chargerTexture(&bouton, idTexture, cheminBouton);
    return bouton;
}

void chargerTextureDecor(SDL_Surface **decor, GLuint *idTexture, char nomDecor[])
{
    /* CHEMIN */
    char cheminDecor[MAX_TAILLE_CHEMIN_FICHIER];
    sprintf(cheminDecor,"%s%s",REP_DECOR_CARTE, nomDecor);
    printf("Chemin décor : %s\n", cheminDecor);
    chargerTexture(decor, idTexture, cheminDecor);
}



SDL_Surface* chargerTextureLutin(GLuint *idTexture, TypeLutin *type)
{
    SDL_Surface *lutin;
    /** CHEMIN **/
    char cheminLutin[MAX_TAILLE_CHEMIN_FICHIER];
    /* on construit le chemin du lutin */
    correspondanceCheminLutin(cheminLutin, type);
    /** VÉRIF **/
    printf("Chemin lutin : %s\n", cheminLutin);
    /** CHARGEMENT MÉMOIRE VIVE **/
    chargerTexture(&lutin, idTexture, cheminLutin);
    return lutin;
}

void libererRessourcesAffichage(Ressources *ressources)
{
    int k;
    /* ON LIBÈRE LES TEXTURES PUIS LES IMAGES */
    if( ressources->decor )
    {
        glDeleteTextures(1, &(ressources->textureDecor));
        glDeleteLists(ressources->affichageDecor, 1);

        SDL_FreeSurface(ressources->decor);
    }
    /* les arrière-plan */
    glDeleteTextures(NB_ARRIERE_PLANS, ressources->arrierePlanTextures);
    glDeleteLists(ressources->arrierePlanAffichage[0], NB_ARRIERE_PLANS);
    /* */
    for( k=0; k<NB_ARRIERE_PLANS; k++ )
    {
        SDL_FreeSurface(ressources->arrierePlans[k]);
    }

    /* les boutons */
    glDeleteTextures(NB_BOUTONS, ressources->boutonTextures);
    glDeleteLists(ressources->boutonAffichage[0], NB_BOUTONS);
    for( k=0; k<NB_BOUTONS; k++ )
    {
        SDL_FreeSurface(ressources->boutons[k]);
    }

    /* les lutins */
    glDeleteTextures(NB_LUTINS, ressources->banqueTextures);
    glDeleteLists(ressources->banqueAffichage[0], NB_LUTINS);
    /* */
    for( k=0; k<NB_LUTINS; k++ )
    {
        SDL_FreeSurface(ressources->lutins[k]);
    }
    /* les rangs */
    glDeleteTextures(NB_RANGS, ressources->rangTextures);
    glDeleteLists(ressources->rangAffichage[0], NB_RANGS);
    /* */
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

void calculerDimensionsEchelle(double *propX, double *propY, Dimensions *dimElement)
{
    *propX = dimElement->x/(double)LARGEUR_FENETRE;
    *propY = dimElement->y/(double)HAUTEUR_FENETRE;
}

void calculerDimensionsEchelleImage(double *propX, double *propY, Dimensions *dimElement, Dimensions *dimImage)
{
    *propX = dimElement->x/(double)dimImage->x;
    *propY = dimElement->y/(double)dimImage->y;
}

void dessinerTexture(GLuint idTexture)
{
    /* On va la dessiner en carré, aux dimensions de la fenêtre virtuelle.  */
    
    /* ON DESSINE LA TEXTURE CENTRÉE À L'ORIGINE */
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

void correspondanceCheminBouton(char *cheminBouton, int indice)
{
    /* on construit le chemin du RANG */
    strcpy(cheminBouton, REP_BOUTON);
    strcat(cheminBouton, NOM_IMAGE_BOUTON[indice]);
    strncat(cheminBouton, EXTENSION, MAX_TAILLE_NOM_FICHIER);
}

void correspondanceCheminArrierePlan(char *cheminArrierePlan, int indice)
{
    /* on construit le chemin du RANG */
    strcpy(cheminArrierePlan, REP_ARRIEREPLAN);
    strcat(cheminArrierePlan, NOM_ARRIEREPLAN[indice]);
    strncat(cheminArrierePlan, EXTENSION, MAX_TAILLE_NOM_FICHIER);
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