#include "../include/bouton.h"

void drawSquare(int filled){

    float squareX = 0.5;
    float squareXX = -0.5;
    float squareY = 0.5;
    float squareYY = -0.5;

    /*if(filled) 
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0, 0.0);
    }
    else 
    {*/

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
    

    glVertex2f( squareX , squareYY);
   // printf("%f %f\n", squareX, squareYY);
    glVertex2f( squareX , squareY);
    //printf("%f %f\n", squareX, squareY);
    glVertex2f( squareXX , squareY);
    //printf("%f %f\n", squareXX, squareY);
    glVertex2f( squareXX, squareYY);
    //printf("%f %f\n", squareXX, squareYY);
    //glVertex2f( squareX, squareYY);
    //printf("%f %f\n", squareX, squareYY);

    glEnd();
}


// ESSAI EVENT BOUTON
void isItButton(SDL_Event e, Dimensions *dimImage){
    Point coord;
    coord.x = e.button.x;
    coord.y = e.button.y;
    double posX;
    double posY;
    calculerCoordonneesVirtuelles(&coord, &posX, &posY, dimImage);

    if(posX <= 0.5 && posX >= -0.5 && posY <= 0.5 && posY >= -0.5){
        printf("C'est un bouton !");
    }
}