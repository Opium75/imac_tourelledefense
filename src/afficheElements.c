
#include "../include/afficheElements.h"
#include "../include/afficheCarte.h"

void matrixAffich(GLuint idElements, int posX, int posY){

        int coordHG_X = posX-(image->w/2);
        int coordHG_Y = posY+(image->h/2);
        int coordBG_X = posX+(image->w/2);
        int coordBG_Y = posY-(image->w/2);
        int coordHD_X = posX+(image->w/2);
        int coordHD_Y = posY+(image->w/2);
        int coordBD_X = posX+(image->w/2);
        int coordBD_Y = posY-(image->w/2);

        glPushMatrix();

        glScalef(100.,100.,0.);

        glEnable(GL_TEXTURE_2D); // FAIRE POUR RECTANGLE ET A PARTIR DU CENTRE AVOIR COORD DES 4 PTS
        glBindTexture(GL_TEXTURE_2D, idElements);
        glBegin(GL_QUADS);  //GL_TRIANGLE_FAN
            glTexCoord2f(0, 1);
            glVertex2f(coordBG_X, coordBG_Y);
            
            glTexCoord2f(1, 1);
            glVertex2f(coordBD_X, coordBD_Y);
            
            glTexCoord2f(1, 0);
            glVertex2f(coordHD_X, coordHD_Y);
            
            glTexCoord2f(0, 0);
            glVertex2f(coordHG_X, coordHG_Y);
        glEnd(); 
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

}


int afficheElements(void) {


    if (-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "[Error] SDL not init\n");
        return EXIT_FAILURE;
    }


    /* Create OpenGl context */
    SDL_Surface* surface;
    surface = SDL_SetVideoMode(WINDOW_WIDTH , WINDOW_HEIGHT, 8, 
        SDL_OPENGL | SDL_RESIZABLE | SDL_GL_DOUBLEBUFFER);
    if (NULL == surface) {
        fprintf(stderr, "[Error] SDL window is null\n");
        return EXIT_FAILURE;
    }
    if (!gladLoadGL()) {
        printf("[Error] Glad not init\n");
		return EXIT_FAILURE;
	}

    printf("OpenGL version %s\n", glGetString(GL_VERSION));
    printf("Glu version %s\n", gluGetString(GLU_VERSION));
    SDL_WM_SetCaption(WINDOW_TITLE, NULL);
  
    // ELEMENTS
    GLuint idElements = glGenLists(6);

    //TOURB
    glNewList(idElements, GL_COMPILE);
    SDL_Surface* image = IMG_Load("imageTD/tourB.png");
    if(image == NULL) {
        printf("[Error] Logo not init\n");
    }
    glEndList();

    //TOURJ
    glNewList(idElements+1, GL_COMPILE);
    SDL_Surface* image = IMG_Load("imageTD/tourJ.png");
    if(image == NULL) {
        printf("[Error] Logo not init\n");
    }
    glEndList();

    //TOURV
    glNewList(idElements+2, GL_COMPILE);
    SDL_Surface* image = IMG_Load("imageTD/tourV.png");
    if(image == NULL) {
        printf("[Error] Logo not init\n");
    }
    glEndList();

    //TOURR
    glNewList(idElements+3, GL_COMPILE);
    SDL_Surface* image = IMG_Load("imageTD/tourR.png");
    if(image == NULL) {
        printf("[Error] Logo not init\n");
    }
    glEndList();

    //VIRUS1
    glNewList(idElements+4, GL_COMPILE);
    SDL_Surface* image = IMG_Load("imageTD/virus.png");
    if(image == NULL) {
        printf("[Error] Logo not init\n");
    }
    glEndList();    

    //VIRUS2
    glNewList(idElements+5, GL_COMPILE);
    SDL_Surface* image = IMG_Load("imageTD/virus2.png");
    if(image == NULL) {
        printf("[Error] Logo not init\n");
    }
    glEndList();


    glViewport(0, 0, (surface)->w, (surface)->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        -GL_VIEW_WIDTH / 2, GL_VIEW_WIDTH / 2, 
        -GL_VIEW_HEIGHT / 2, GL_VIEW_HEIGHT / 2);


    glGenTextures(6, &idElements);
    glBindTexture(GL_TEXTURE_2D, idElements); // FAIRE POUR CHAQUE TEXTURE
    glBindTexture(GL_TEXTURE_2D, idElements+1);
    glBindTexture(GL_TEXTURE_2D, idElements+2);
    glBindTexture(GL_TEXTURE_2D, idElements+3);
    glBindTexture(GL_TEXTURE_2D, idElements+4);
    glBindTexture(GL_TEXTURE_2D, idElements+5);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
    GL_TEXTURE_2D , 0, GL_RGBA,
    image->w, image->h, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    
    /* Main loop */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
    



    const Uint32 TARGET_FRAMERATE_MS = 1000 / 60;
    int loop = 1;
    while (loop) {   
        Uint32 startTime = SDL_GetTicks();

        glClear(GL_COLOR_BUFFER_BIT);

    
       /* glPushMatrix();

        glScalef(100.,100.,0.);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, idLogo);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f( 0.5 , -0.5);
            glTexCoord2f(0,0);
            glVertex2f( 0.5 , 0.5);
            glTexCoord2f(0,1);
            glVertex2f( -0.5 , 0.5);
            glTexCoord2f(1,1);
            glVertex2f( -0.5 , -0.5);
            glTexCoord2f(1,0);
            glVertex2f( 0.5 , -0.5);
          glEnd(); 
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);*/


        /* Update window */
        SDL_GL_SwapBuffers();
        
        /* Handle events */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            
            switch (e.type) {
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    // FONCTION POUR LES PUSH MATRIX          
                    matrixAffich(idElements, e.button.x, e.button.y);
                    break;

                default:
                    break;
            }

        }

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Pause the program if the framerate is too low */
        if (elapsedTime < TARGET_FRAMERATE_MS) 
        {
            SDL_Delay(TARGET_FRAMERATE_MS - elapsedTime);
        }
    }
    
    SDL_FreeSurface(image);
    glDeleteTextures(1, &idElements);
    SDL_Quit();
    return EXIT_SUCCESS;
}
