//
//  Tutorial3.cpp
//  SDL2OpenGL3
//
//

#include "Tutorial3.h"


/* A simple function that will read a file into an allocated char pointer buffer */
char* Tutorial3::filetobuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;
    
    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) {/* Return NULL on failure */
        return NULL;
    }
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
    
    return buf; /* Return the buffer */
}

/* A simple function that prints a message, the error code returned by SDL, and quits the application */
void Tutorial3::sdldie(char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}

void Tutorial3::setupwindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {/* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */
    }
    
    /* Request an opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    /* Request for GLSL 150 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    
    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    /* Enable multisampling for a nice antialiased effect */
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    
    
    /* Create our window centered at 512x512 resolution */
    mainwindow = SDL_CreateWindow("Tutorial3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!mainwindow) {/* Die if creation failed */
        sdldie("Unable to create window");
    }
    
    /* Create our opengl context and attach it to our window */
    maincontext = SDL_GL_CreateContext(mainwindow);
    
    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);
    
    /* Enable Z depth testing so objects closest to the viewpoint are in front of objects further away */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Tutorial3::drawscene()
{
    int i; /* Simple iterator */
    GLuint vao, vbo[1]; /* Create handles for our Vertex Array Object and One Vertex Buffer Object */
    
    int IsCompiled_VS, IsCompiled_FS;
    int IsLinked;
    int maxLength;
    char *vertexInfoLog;
    char *fragmentInfoLog;
    char *shaderProgramInfoLog;
    
    GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
    GLfloat modelmatrix[16]; /* Our model matrix  */
    /* An identity matrix we use to perform the equivalant of glLoadIdentity */
    const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;
    
    /* An array of 12 Vertices to make 4 coloured triangles in the shape of a tetrahedron*/
    const struct Vertex tetrahedron[12] =
    {
        {
            {  1.0,  1.0,  1.0  },{  1.0f,  0.0f,  0.0f  }
        },
        {
            { -1.0, -1.0,  1.0  },{  1.0f,  0.0f,  0.0f  }
        },
        {
            { -1.0,  1.0, -1.0  },{  1.0f,  0.0f,  0.0f  }
        },
        {
            {  1.0,  1.0,  1.0  },{  0.0f,  1.0f,  0.0f  }
        },
        {
            { -1.0, -1.0,  1.0  },{  0.0f,  1.0f,  0.0f  }
        },
        {
            {  1.0, -1.0, -1.0  },{  0.0f,  1.0f,  0.0f  }
        },
        {
            {  1.0,  1.0,  1.0  },{  0.0f,  0.0f,  1.0f  }
        },
        {
            { -1.0,  1.0, -1.0  },{  0.0f,  0.0f,  1.0f  }
        },
        {
            {  1.0, -1.0, -1.0  },{  0.0f,  0.0f,  1.0f  }
        },
        {
            { -1.0, -1.0,  1.0  },{  1.0f,  1.0f,  1.0f  }
        },
        {
            { -1.0,  1.0, -1.0  },{  1.0f,  1.0f,  1.0f  }
        },
        {
            {  1.0, -1.0, -1.0  },{  1.0f,  1.0f,  1.0f  }
        }
    };
    
    /* These pointers will receive the contents of our shader source code files */
    GLchar *vertexsource, *fragmentsource;
    
    /* These are handles used to reference the shaders */
    GLuint vertexshader, fragmentshader;
    
    /* This is a handle to the shader program */
    GLuint shaderprogram;
    
    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);
    
    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);
    
    /* Allocate and assign One Vertex Buffer Object to our handle */
    glGenBuffers(1, vbo);
    
    /* Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    
    /* Copy the vertex data from tetrahedron to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 Vertex values */
    glBufferData ( GL_ARRAY_BUFFER, 12 * sizeof ( struct Vertex ), tetrahedron, GL_STATIC_DRAW );
    
    /* Specify that our coordinate data is going into attribute index 0, and contains three doubles per vertex */
    /* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0 */
    glVertexAttribPointer ( ( GLuint ) 0, 3, GL_DOUBLE, GL_FALSE,  sizeof ( struct Vertex ), ( const GLvoid* ) offsetof (struct Vertex,position) );
	
    /* Enable attribute index 0 as being used */
    glEnableVertexAttribArray(0);
    
    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
    /* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
     occupied by the first attribute (position) */
    glVertexAttribPointer ( ( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex, color) ) ;
    
    /* Enable attribute index 1 as being used */
    glEnableVertexAttribArray ( 1 );/* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
    
    /* Read our shaders into the appropriate buffers */
    vertexsource = filetobuf("/Users/valeriy/tutorial3.vert");
    fragmentsource = filetobuf("/Users/valeriy/tutorial3.frag");
    
    /* Assign our handles a "name" to new shader objects */
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    
    /* Associate the source code buffers with each handle */
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    
    /* Compile our shader objects */
    glCompileShader(vertexshader);
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
    if(IsCompiled_VS == false)
    {
        glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
        
        /* The maxLength includes the NULL character */
        vertexInfoLog = (char *)malloc(maxLength);
        
        glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
        
        /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
        /* In this simple program, we'll just leave */
        free(vertexInfoLog);
        return;
    }
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == false)
    {
        glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
        
        /* The maxLength includes the NULL character */
        fragmentInfoLog = (char *)malloc(maxLength);
        
        glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
        
        /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
        /* In this simple program, we'll just leave */
        free(fragmentInfoLog);
        return;
    }
    /* Assign our program handle a "name" */
    shaderprogram = glCreateProgram();
    
    /* Attach our shaders to our program */
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);
    
    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");
    
    /* Link our program, and set it as being actively used */
    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);
    
    /* Create our projection matrix with a 45 degree field of view
     * a width to height ratio of 1 and view from .1 to 100 infront of us */
    perspective(projectionmatrix, 45.0, 1.0, 0.1, 100.0);
    
    /* Loop our display rotating our model more each time. */
    for (i=0; i < 360; i++)
    {
        /* Load the identity matrix into modelmatrix. rotate the model, and move it back 5 */
        memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
        rotate(modelmatrix, (GLfloat)i * -1.0, X_AXIS);
        rotate(modelmatrix, (GLfloat)i * 1.0, Y_AXIS);
        rotate(modelmatrix, (GLfloat)i * 0.5, Z_AXIS);
        translate(modelmatrix, 0, 0, -5.0);
        
        /* multiply our modelmatrix and our projectionmatrix. Results are stored in modelmatrix */
        multiply4x4(modelmatrix, projectionmatrix);
        
        /* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
        glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, modelmatrix);
        
        /* Make our background black */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* Invoke glDrawArrays telling that our data consists of individual triangles */
        glDrawArrays(GL_TRIANGLES, 0, 12);
        
        /* Swap our buffers to make our changes visible */
        SDL_GL_SwapWindow(mainwindow);
        
        /* Sleep for roughly 33 milliseconds between frames */
        SDL_Delay(33);
    }
    
    /* Cleanup all the things we bound and allocated */
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
    free(vertexsource);
    free(fragmentsource);
//    vertexsource = filetobuf("/Users/valeriy/tutorial2.vert");
//    fragmentsource = filetobuf("/Users/valeriy/tutorial2.frag");

}

void Tutorial3::destroywindow()
{
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
}

