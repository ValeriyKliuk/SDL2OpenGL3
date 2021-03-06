//
//  Tutorial2.h
//  SDL2OpenGL3
//
//

#ifndef SDL2OpenGL3_Tutorial2_h
#define SDL2OpenGL3_Tutorial2_h

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>


// http://www.opengl.org/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
class Tutorial2
{
    
public:
    
    Tutorial2(){}
    
    ~Tutorial2(){}
    
    char* filetobuf(char *file);
    
    void sdldie(char *msg);
    
    void setupwindow();
    
    void drawscene();
    
    void destroywindow();
    
    
private:
    
    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    
};


#endif
