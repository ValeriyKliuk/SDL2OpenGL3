//
//  Tutorial3.h
//  SDL2OpenGL3
//
//

#ifndef SDL2OpenGL3_Tutorial3_h
#define SDL2OpenGL3_Tutorial3_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h> /* must include for the offsetof macro */

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

#include <math.h>
#include "Utils.h"

// http://www.opengl.org/wiki/Tutorial3:_Rendering_3D_Objects_(C_/SDL)
class Tutorial3
{
    
public:
    
    Tutorial3(){}
    
    ~Tutorial3(){}
    
    char* filetobuf(char *file);
    
    void sdldie(char *msg);
    
    void setupwindow();
    
    void drawscene();
    
    void destroywindow();
    
    struct Vertex
    {
        GLdouble position[3];
        GLfloat color[3];
    };
    
    
private:
    
    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    
};


#endif
