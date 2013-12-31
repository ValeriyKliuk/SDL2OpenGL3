//
//  Tutorial.h
//  SDL2OpenGL3
//
//  Created by Valeriy Kliuk on 12/30/2013.
//  Copyright (c) 2013 Valeriy Kliuk. All rights reserved.
//

#ifndef SDL2OpenGL3_Tutorial_h
#define SDL2OpenGL3_Tutorial_h

#include <stdio.h>
#include <stdlib.h>


#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

//http://www.opengl.org/wiki/Tutorial1%3a_Creating_a_Cross_Platform_OpenGL_3.2_Context_in_SDL_%28C_/_SDL%29
class Tutorial
{
    
public:
    Tutorial(){}
    ~Tutorial(){}
    
    /* A simple function that prints a message, the error code returned by SDL,
     * and quits the application */
    void sdldie(const char *msg);
    
    void checkSDLError(int line = -1);
    
    void execute();
    //SDL_GL_SwapWindow(<#SDL_Window *window#>)
    
    
private:

    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    
};

#endif
