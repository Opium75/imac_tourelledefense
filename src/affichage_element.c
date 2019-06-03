
#include "../include/affichage_element.h"

void afficherCarte(GLuint idAffichage, Dimensions *dimImage)
{
    /* La carte prend tout l'espace de la fenêtre */
    afficherElement(idAffichage, dimImage);
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
            afficherElement(idAffichage, dimImage);
        glPopMatrix();
        /* on réinitialise la couleur */
    GL_changerCouleurTrait(COULEUR_PARDEFAUT);
}

void afficherTour(Tour *tour, GLuint idAffichage, Dimensions *dimLutin, Dimensions *dimImage)
{
    /* la correspondance entre coordonnées (x,y) et position sur l'écran */
    double posX, posY;
    /* affichage prenant en compte les coordonn)ées de l'élémenent */
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
        /* On part du point en haut à gauche
        * de la fenêtre virtuelle,
        * d'où les deux soustractions
        */
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
        afficherElement(idAffichage, dimImage);
    glPopMatrix();
}


void afficherElement(GLuint idAffichage, Dimensions *dimImage)
{
    /* */
    glPushMatrix();
        /* on appelle la liste d'affichage qui contient le lutin */
        glCallList(idAffichage);
    glPopMatrix();
}

/////TEXTES////////////

void afficherTouche(char toucheCode)
{
    char texteTouche[MAX_TAILLE_TEXTE];
    sprintf(texteTouche, "%s : %c", TEXTE_TOUCHE, toucheCode);
    vBitmapOutput( texteTouche, &POSITION_TEXTE_TOUCHE, GLUT_BITMAP_HELVETICA_18);
}

void afficherTexte(char texte[], Point *origine, Dimensions *dimImage)
{
    vBitmapOutput(texte, origine, GLUT_BITMAP_HELVETICA_18);
}

void vBitmapOutput(char *chaine, Point *origine, void *police)
{
    glRasterPos2f((GLfloat)origine->x/LARGEUR_FENETRE, (GLfloat)origine->y/LARGEUR_FENETRE); // Positionne le premier caractère de la chaîne
    int c;
    for(c=0; !chaine[c]; c++)
        glutBitmapCharacter(police, chaine[c]); // Affiche chaque caractère de la chaîne
}

void vStrokeOutput(char *chaine, Point *origine, void *police, Dimensions *dimImage)
{
    double posX, posY;
    int c;
    calculerCoordonneesVirtuelles(origine, &posX, &posY, dimImage);
    glPushMatrix(); // glPushMatrix et glPopMatrix sont utilisées pour sauvegarder 
            // et restaurer les systèmes de coordonnées non translatées
        glTranslatef(posX, posY, 0.); // Positionne le premier caractère de la chaîne
        for(c=0; !chaine[c]; c++)
            glutStrokeCharacter(police, chaine[c]); // Affiche chaque caractère de la chaîne
    glPopMatrix();
}

///////////////////////

// int afficherCarte(void) 
// {

//     char txtP[25];
//     char touchecode = '\0';

//     /* Initialisation de la SDL */
//     if(-1 == SDL_Init(SDL_INIT_VIDEO)) 
//     {
//         fprintf(
//             stderr, 
//             "Impossible d'initialiser la SDL. Fin du programme.\n");
//         exit(EXIT_FAILURE);
//     }

//     int taille = 1;
//     char* vecteur[] = {"Cest le tableau pour mettre la taille des textes a afficher sur l ecran"};
  
//     glutInit(&taille, vecteur);
  
//     /* Ouverture d'une fenetre et creation d'un contexte OpenGL */
//     SDL_Surface* surface;
//     redimensionner(&surface, LARGEUR_FENETRE, HAUTEUR_FENETRE);

//     // Image 
//     SDL_Surface* image = IMG_Load("./images/carteTD.png");

//     if (image == NULL){
//         printf("Oh no :( \n");
//     } else {
//         printf("Ca marche loul\n");
//     }


//     //Texture

//     GLuint texture1;
//     glGenTextures(1, &texture1);

//     glBindTexture(GL_TEXTURE_2D, texture1);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
//         image->w, image->h, 0,
//     GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    
//     glBindTexture(GL_TEXTURE_2D, 0);



//     /* Initialisation du titre de la fenetre */
//     SDL_WM_SetCaption(TITRE_FENETRE, NULL);
  
//     /* Boucle principale */
//     int loop = 1;
//     while(loop) 
//     {
//         /* Recuperation du temps au debut de la boucle */
//         Uint32 startTime = SDL_GetTicks();
        
//         /* Placer ici le code de dessin */
//         glClear(GL_COLOR_BUFFER_BIT);

//         glEnable(GL_TEXTURE_2D);
    
//         glBindTexture(GL_TEXTURE_2D, texture1);

//         glMatrixMode(GL_MODELVIEW);
//         glLoadIdentity();

        
//         glPushMatrix();
//             glBegin(GL_QUADS);
//                 glTexCoord2f(0, 1);
//                 glVertex2f(-0.5, -0.5);
            
//                 glTexCoord2f(1, 1);
//                 glVertex2f(0.5, -0.5);
            
//                 glTexCoord2f(1, 0);
//                 glVertex2f(0.5, 0.5);
            
//                 glTexCoord2f(0, 0);
//                 glVertex2f(-0.5, 0.5);
//             glEnd();
//         glPopMatrix();

//         glPushMatrix();
//             glBegin(GL_QUADS);
//                 glTexCoord2f(0, 1);
//                 glVertex2f(-10, -10);
            
//                 glTexCoord2f(1, 1);
//                 glVertex2f(10, -10);
            
//                 glTexCoord2f(1, 0);
//                 glVertex2f(10, 10);
            
//                 glTexCoord2f(0, 0);
//                 glVertex2f(-10, 10);
//             glEnd();
//         glPopMatrix();

//         glBindTexture(GL_TEXTURE_2D, 0);
//         glDisable(GL_TEXTURE_2D);

//         // Texte
//         vBitmapOutput( -95, 65, "Argent du joueur : ", GLUT_BITMAP_HELVETICA_18);
//         vBitmapOutput( -95, 40, "Temps restant : ", GLUT_BITMAP_HELVETICA_18);

//         //TOUCHE AFFICHE
//         afficheTouche(txtP, touchecode);
//         ///

        
//         /* Echange du front et du back buffer : mise a jour de la fenetre */
//         SDL_GL_SwapBuffers();
        
//         /* Boucle traitant les evenements */
//         SDL_Event e;
//         while(SDL_PollEvent(&e)) 
//         {
//             /* L'utilisateur ferme la fenetre : */
//             if(e.type == SDL_QUIT) 
//             {
//                 loop = 0;
//                 break;
//             }

//             /* L'utilisateur ferme la fenetre : */
//             if(e.type == SDL_QUIT) 
//             {
//                 loop = 0;
//                 break;
//             }
        
//             if( e.type == SDL_KEYDOWN 
//                 && (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
//             {
//                 loop = 0; 
//                 break;
//             }
            
//             /* Quelques exemples de traitement d'evenements : */
//             switch(e.type) 
//             {
//                 /* Redimensionnement fenetre */
//                 case SDL_VIDEORESIZE:
//                     redimensionner(&surface, e.resize.w, e.resize.h);
//                     break;

//                 /* Clic souris */
//                 case SDL_MOUSEBUTTONUP:
//                     printf("clic en (%d, %d)\n", e.button.x, e.button.y);
//                     break;
                
//                 /* Touche clavier */
//                 case SDL_KEYDOWN:
//                     printf("touche pressee (code = %d)\n", e.key.keysym.sym);
//                     printf("touche pressee (code = %c)\n", e.key.keysym.sym);

//                     touchecode = e.key.keysym.sym;
//                     break;
                    
//                 default:
//                     break;
//             }
//         }

//         /* Calcul du temps ecoule */
//         Uint32 elapsedTime = SDL_GetTicks() - startTime;
//         /* Si trop peu de temps s'est ecoule, on met en pause le programme */
//         if(elapsedTime < FRAMERATE_MILLISECONDS) 
//         {
//             SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
//         }
//     }

//     glDeleteTextures(1, &texture1);

//     /* Liberation des ressources associees a la SDL */ 
//     SDL_FreeSurface(image);
//     SDL_Quit();
    
//     return EXIT_SUCCESS;
// }

