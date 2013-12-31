//
//  main.cpp
//  SDL2OpenGL3


#include <stdio.h>
#include <stdlib.h>
/* If using gl3.h */
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

#include "Tutorial.h"
#include "Tutorial2.h"
#include "Tutorial3.h"

/* Our program's entry point */
int main(int argc, char *argv[])
{
    int tutorialIndex = 3;
    
    
    switch (tutorialIndex) {
        case 1:
        {
            /****************************************
             *                                      *
             *             Tutorial1                *
             *                                      *
             ****************************************/
            
            Tutorial *tutorial = new Tutorial();
            tutorial->execute();
        
        }
            break;
        case 2:
        {
            /****************************************
             *                                      *
             *             Tutorial2                *
             *                                      *
             ****************************************/
            
            Tutorial2 *tutorial2 = new Tutorial2();
            
            /* Create our window, opengl context, etc... */
            tutorial2->setupwindow();
            
            /* Call our function that performs opengl operations */
            tutorial2->drawscene();
            
            /* Delete our opengl context, destroy our window, and shutdown SDL */
            tutorial2->destroywindow();
        }
            break;
        case 3:
        {
            /****************************************
             *                                      *
             *             Tutorial3                *
             *                                      *
             ****************************************/
            
            Tutorial3 *tutorial3 = new Tutorial3();
            
            /* Create our window, opengl context, etc... */
            tutorial3->setupwindow();
            
            /* Call our function that performs opengl operations */
            tutorial3->drawscene();
            
            /* Delete our opengl context, destroy our window, and shutdown SDL */
            tutorial3->destroywindow();
        }
            break;
        default:
            break;
    }

    
    
    return 0;
}

