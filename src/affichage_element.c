
#include "../include/affichage_element.h"

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
    double angle;
    double xPos, yPos, xPosM, yPosM;
    Point coordCible;
    unsigned char couleurTir[NB_COULEURS];
    /* on crée le type de lutin de toute pièce */
    TypeLutin type;
    type.nature = LUT_tour;
    type.typeTour = tour->type;
    /* on calcule l'angle par rapport à la cible */
    if( tour->cible )
    {
        calculerPositionMonstre(tour->cible, &coordCible);
        angle = calculerAngle(tour->coord, &coordCible);
        calculerCoordonneesVirtuelles(tour->coord, &xPos, &yPos, dimImage);
        calculerCoordonneesVirtuelles(&coordCible, &xPosM, &yPosM, dimImage);

        /** Dessin **/
        /* ligne de mire*/
        calculerCouleurTir(couleurTir, tour);
        dessinerSegment(xPos, yPos, xPosM, yPosM, couleurTir);
        /* */
        glPushMatrix();
            /* on dirige la tour vers sa cible */
            glTranslatef(xPos, yPos, 0.);
            glRotatef(angle, 0., 0., 1.);
            glTranslatef(-xPos, -yPos, 0.);
             /* on  affiche le monstre  */
            afficherElement(&type, tour->coord, banqueAffichage, banqueTextures, dimImage);
        glPopMatrix();
    }
    else
    {
        afficherElement(&type, tour->coord, banqueAffichage, banqueTextures, dimImage);
    }
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

/////TEXTES/////////////

void vBitmapOutput(int x, int y, char *string, void *font)
{
    int len,i; // len donne la longueur de la chaîne de caractères

    glRasterPos2f(x,y); // Positionne le premier caractère de la chaîne
    len = (int) strlen(string); // Calcule la longueur de la chaîne
    for (i = 0; i < len; i++) glutBitmapCharacter(font,string[i]); // Affiche chaque caractère de la chaîne
}

void vStrokeOutput(GLfloat x, GLfloat y, char *string, void *font)
{
    char *p;

    glPushMatrix(); // glPushMatrix et glPopMatrix sont utilisées pour sauvegarder 
            // et restaurer les systèmes de coordonnées non translatés
    glTranslatef(x, y, 0); // Positionne le premier caractère de la chaîne
    for (p = string; *p; p++) glutStrokeCharacter(font, *p); // Affiche chaque caractère de la chaîne
    glPopMatrix();
}

void afficheTouche(char txtP[25], char touchecode){
    sprintf(txtP, "Touche pressee : %c", touchecode);
    vBitmapOutput( 20, 65, txtP, GLUT_BITMAP_HELVETICA_18);
}

///////////////////////

int afficherCarte(void) 
{

    char txtP[25];
    char touchecode = '\0';

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) 
    {
        fprintf(
            stderr, 
            "Impossible d'initialiser la SDL. Fin du programme.\n");
        exit(EXIT_FAILURE);
    }

    int taille = 1;
    char* vecteur[] = {"Cest le tableau pour mettre la taille des textes a afficher sur l ecran"};
  
    glutInit(&taille, vecteur);
  
    /* Ouverture d'une fenetre et creation d'un contexte OpenGL */
    SDL_Surface* surface;
    redimensionner(&surface, LARGEUR_FENETRE, HAUTEUR_FENETRE);

    // Image 
    SDL_Surface* image = IMG_Load("./images/carteTD.png");

    if (image == NULL){
        printf("Oh no :( \n");
    } else {
        printf("Ca marche loul\n");
    }


    //Texture

    GLuint texture1;
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        image->w, image->h, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    
    glBindTexture(GL_TEXTURE_2D, 0);



    /* Initialisation du titre de la fenetre */
    SDL_WM_SetCaption(TITRE_FENETRE, NULL);
  
    /* Boucle principale */
    int loop = 1;
    while(loop) 
    {
        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
    
        glBindTexture(GL_TEXTURE_2D, texture1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        
        glPushMatrix();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1);
                glVertex2f(-0.5, -0.5);
            
                glTexCoord2f(1, 1);
                glVertex2f(0.5, -0.5);
            
                glTexCoord2f(1, 0);
                glVertex2f(0.5, 0.5);
            
                glTexCoord2f(0, 0);
                glVertex2f(-0.5, 0.5);
            glEnd();
        glPopMatrix();

        /*glPushMatrix();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1);
                glVertex2f(-10, -10);
            
                glTexCoord2f(1, 1);
                glVertex2f(10, -10);
            
                glTexCoord2f(1, 0);
                glVertex2f(10, 10);
            
                glTexCoord2f(0, 0);
                glVertex2f(-10, 10);
            glEnd();
        glPopMatrix();*/

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        // Texte
        vBitmapOutput( -95, 65, "Argent du joueur : ", GLUT_BITMAP_HELVETICA_18);
        vBitmapOutput( -95, 40, "Temps restant : ", GLUT_BITMAP_HELVETICA_18);

        //TOUCHE AFFICHE
        afficheTouche(txtP, touchecode);
        ///

        
        /* Echange du front et du back buffer : mise a jour de la fenetre */
        SDL_GL_SwapBuffers();
        
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
            /* L'utilisateur ferme la fenetre : */
            if(e.type == SDL_QUIT) 
            {
                loop = 0;
                break;
            }

            /* L'utilisateur ferme la fenetre : */
            if(e.type == SDL_QUIT) 
            {
                loop = 0;
                break;
            }
        
            if( e.type == SDL_KEYDOWN 
                && (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
            {
                loop = 0; 
                break;
            }
            
            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) 
            {
                /* Redimensionnement fenetre */
                case SDL_VIDEORESIZE:
                    redimensionner(&surface, e.resize.w, e.resize.h);
                    break;

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;
                
                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    printf("touche pressee (code = %c)\n", e.key.keysym.sym);

                    touchecode = e.key.keysym.sym;
                    break;
                    
                default:
                    break;
            }
        }

        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    glDeleteTextures(1, &texture1);

    /* Liberation des ressources associees a la SDL */ 
    SDL_FreeSurface(image);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
