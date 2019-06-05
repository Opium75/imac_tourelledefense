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

bool boutonEstClique(Point *coordBouton, Dimensions *dimBouton, Point *coordClique, Dimensions *dimImage)
{
    bool estClique;
    /* calcul des coordonnées et dimensions virtuelles */
    /* */
    /*seule différence : le texte n'est pas centré en abscisse, mais il l'est en ordonnée (observation empirique) */
    estClique = (coordClique->x >= coordBouton->x - (int)dimBouton->x/2 && coordClique->x <= (int)coordBouton->x + (int)dimBouton->x/2)
                && (coordClique->y >= coordBouton->y - (int)dimBouton->y/2 && coordClique->y <= (int)coordBouton->y + (int)dimBouton->y/2);
    return estClique;           
}

bool texteEstClique(Point *coordTexte, Dimensions *dimTexte, Point *coordClique, Dimensions *dimImage)
{
    bool estClique;
    /* calcul des coordonnées et dimensions virtuelles */
    /* */
    /*l'image est centrée à l'origine, donc on soustrait, ajoute la moitié des dimensions */
    estClique = (coordClique->x >= coordTexte->x && coordClique->x <= (int)coordTexte->x + (int)dimTexte->x)
                && (coordClique->y >= coordTexte->y - (int)dimTexte->y/2 && coordClique->y <= (int)coordTexte->y + (int)dimTexte->y/2);
    return estClique; 
}

bool boutonEstSurvole(Point *coordBouton, Dimensions *dimBouton, Point *coordSouris, Dimensions *dimImage)
{
    return boutonEstClique(coordBouton, dimBouton, coordSouris, dimImage);
}