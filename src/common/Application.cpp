#include "Application.hpp"

#include "common/Scene.hpp"
#include "stb_image/stb_image.h"
#include "imguiRenderGL.h"
#include "stb_image/stb_image.h"
#include "GLCommon.hpp"
#include "GL/glfw.h"
#include <iostream>

Application::Application(size_t width, size_t height, char* title) : _width(width), _height(height), _title(title), _frameDuration(0.1) {
    initGlfw();
}

Application::~Application() {
    delete [] _title;
    glfwTerminate();
}

void Application::initGlfw() {
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
        
    #ifdef __APPLE__
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
        glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    // Open a window and create its OpenGL context
    if(!glfwOpenWindow(_width, _height, 0,0,0,0, 24,0, GLFW_WINDOW)) {
        fprintf(stderr, "Failed to open GLFW window\n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowTitle(_title);

    #ifdef __APPLE__
        glewExperimental = GL_TRUE;
    #endif

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
          /* Problem: glewInit failed, something is seriously wrong. */
          fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
          exit(EXIT_FAILURE);
    }

    // Hide mouse cursor
    glfwDisable(GLFW_MOUSE_CURSOR);

    // Ensure we can capture the escape key being pressed below
    glfwEnable(GLFW_STICKY_KEYS);

    // Enable vertical sync (on cards that support it)
    glfwSwapInterval(1);

    // Init UI
    if (!imguiRenderGLInit("DroidSans.ttf"))
    {
        fprintf(stderr, "Could not init GUI renderer.\n");
        exit(EXIT_FAILURE);
    }
}

void Application::initTimers() {
    // Get time
    _time = glfwGetTime();
}

void Application::renderFrame() {
    // Clears the window with current clearing color, clears also the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws scene
    _scene.drawObjectsOfScene();
    
    // Performs the buffer swap between the current shown buffer, and the one we just worked on
    glfwSwapBuffers();
}

void Application::eventDetection() {
    // Get mouse motion
    _mousex;
    _mousey;
    glfwGetMousePos(&_mousex, &_mousey);

    // Get keys
    _leftButton = glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT );
    _zButton = glfwGetKey('Z');
    _qButton = glfwGetKey('Q');
    _sButton = glfwGetKey('S');
    _dButton = glfwGetKey('D');
}

void Application::animate() {

}

void Application::loop() {
    do {
        // Update camera view
        //scene.updateCamera(_mousex, _mousey, _zButton, _qButton, _sButton, _dButton);
        initTimers();
        eventDetection();

        double t = glfwGetTime();
        if(t - _lasStartTime >= _frameDuration) {
            animate();
            renderFrame();
            _lasStartTime = t;
            // Reports any possible glError
            //printGlErrors();
        }
    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
}