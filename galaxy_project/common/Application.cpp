#include "Application.hpp"

#include "GLCommon.hpp"
#include "GL/glfw.h"
#include <iostream>

#include "imguiRenderGL.h"

Application::Application(size_t width, size_t height, char* title) {
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