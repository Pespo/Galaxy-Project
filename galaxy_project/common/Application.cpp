#include "Application.hpp"
#include "GLCommon.hpp"
#include "ShaderGLSL.hpp"
#include "stb_image/stb_image.h"


#include "GL/glfw.h"
#include <iostream>

#include "imguiRenderGL.h"

Application::Application(size_t width, size_t height, char* title) {
   
    /************* init GLFW *************/
        if( !glfwInit() ) {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            exit( EXIT_FAILURE );
        }
            
        #ifdef __APPLE__
            glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
            glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
            glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif

        // Open a window and create its OpenGL context
        if( !glfwOpenWindow( width, height, 0,0,0,0, 24,0, GLFW_WINDOW ) ) {
            fprintf( stderr, "Failed to open GLFW window\n" );

            glfwTerminate();
            exit( EXIT_FAILURE );
        }

        glfwSetWindowTitle( title );

        #ifdef __APPLE__
            glewExperimental = GL_TRUE;
        #endif

        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
              /* Problem: glewInit failed, something is seriously wrong. */
              fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
              exit( EXIT_FAILURE );
        }

        // Ensure we can capture the escape key being pressed below
        glfwEnable( GLFW_STICKY_KEYS );

        // Enable vertical sync (on cards that support it)
        glfwSwapInterval( 1 );

        // Init UI
        if (!imguiRenderGLInit("DroidSans.ttf"))
        {
            fprintf(stderr, "Could not init GUI renderer.\n");
            exit(EXIT_FAILURE);
        }

};

Application::~Application(){
    glfwTerminate();
}

GLuint Application::loadTexture(const char* fileName, int comp){
    int w;
    int h;
    int c = comp;
    unsigned char *data;

    try {
        // Loads the image from a ppm file to an unsigned char array
        data = stbi_load(fileName, &w, &h, &c, c);
     }
     catch (...) {
         return 0;
    }   
    //Selects our current unit texture
    glActiveTexture(GL_TEXTURE0);

    // Allocates a texture id
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    // Selects our current texture
    glBindTexture(GL_TEXTURE_2D, textureID);


    // How to handle not normalised uvs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // How to handle interpolation from texels to fragments
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specifies which image will be used for this texture objet
    switch(comp) {
        case 1 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
        case 2 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, w, h, 0, GL_RG, GL_UNSIGNED_BYTE, data);
            break;
        case 3 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4 :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
    }

    fprintf(stderr, "Texture %dx%d:%d\n", w, h, c);
    
    return textureID;
};

void Application::loadShader(ShaderGLSL & shader,  const char * path){

    int status = load_shader_from_file(shader, path, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER);
    if ( status == -1 )
    {
        std::cerr << "Error on loading " << path << std::endl;
        //exit( EXIT_FAILURE );
    }

}

