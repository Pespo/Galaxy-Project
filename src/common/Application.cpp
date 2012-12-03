#include "Application.hpp"

#include "imguiRenderGL.h"
#include "GLCommon.hpp"
#include "GL/glfw.h"
#include <iostream>

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
}

Application::~Application(){
    glfwTerminate();
}

void Application::loop() {
    do {

        t = glfwGetTime();

        // Mouse button checking
        if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            std::cout << "LEFT CLICK!" << std::endl;
        if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
            std::cout << "MIDDLE CLICK!" << std::endl;
        if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            std::cout << "RIGHT CLICK!" << std::endl;

        // @TODO: Camera movements
            /*int altPressed = glfwGetKey(GLFW_KEY_LALT);
            if (!altPressed && (leftButton == GLFW_PRESS || rightButton == GLFW_PRESS || middleButton == GLFW_PRESS))
            {
                int x; int y;
                glfwGetMousePos(&x, &y);
                guiStates.lockPositionX = x;
                guiStates.lockPositionY = y;
            }
            if (altPressed == GLFW_PRESS)
            {
                int mousex; int mousey;
                glfwGetMousePos(&mousex, &mousey);
                int diffLockPositionX = mousex - guiStates.lockPositionX;
                int diffLockPositionY = mousey - guiStates.lockPositionY;
                if (guiStates.zoomLock)
                {
                    float zoomDir = 0.0;
                    if (diffLockPositionX > 0)
                        zoomDir = -1.f;
                    else if (diffLockPositionX < 0 )
                        zoomDir = 1.f;
                    camera.zoom(zoomDir * GUIStates::MOUSE_ZOOM_SPEED);
                }
                else if (guiStates.turnLock)
                {
                    camera.turn(diffLockPositionY * GUIStates::MOUSE_TURN_SPEED,
                                diffLockPositionX * GUIStates::MOUSE_TURN_SPEED);

                }
                else if (guiStates.panLock)
                {
                    camera.pan(diffLockPositionX * GUIStates::MOUSE_PAN_SPEED,
                               diffLockPositionY * GUIStates::MOUSE_PAN_SPEED);
                }
                guiStates.lockPositionX = mousex;
                guiStates.lockPositionY = mousey;
            }*/
        // @ENDTODO //

        // Get camera matrices
        /*float projection[16];
        float worldToView[16];
        float objectToWorld[16];
        float cameraPosition[4];
        float orthoProj[16];
        ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0, orthoProj);
        mat4fCopy(projection, camera.perspectiveProjection());
        mat4fCopy(worldToView, camera.worldToView());
        mat4fToIdentity(objectToWorld);
        vec4fCopy(cameraPosition, camera.position());

        float viewProjection[16];     
        float iviewProjection[16];       

        mat4fMul( worldToView, projection, viewProjection);
        mat4fInverse(viewProjection, iviewProjection);*/

        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

    // Reports any possible glError
    //printGlErrors();
}