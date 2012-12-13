#include "GalaxyApp.hpp"
#include "MoveableCamera.hpp"

#include "GL/glfw.h"
#include <stein/Object.hpp>
#include <stein/Tools.hpp>
#include <stein/Builders.hpp>
#include <stein/math/StreamUtils.h>

#include <iostream>

#include <vector>

using namespace std;
using namespace stein;

GalaxyApp::GalaxyApp() : Application(800, 800) {

    const float size = .06;

    _scene.pCamera = new MoveableCamera();
    _scene.pCamera->setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _scene.pCamera->setPosition(Vector3f(0, 0, 55));
    _scene.setDefaultShaderID(loadProgram("shaders/1.glsl"));

    Object &object = _scene.createObject(GL_TRIANGLES);
	MeshBuilder meshBuilder = MeshBuilder();
    buildSphere(object, 0.1, 5, 5, meshBuilder);

    _scene.addObjectToDraw(object.id);
    _scene.setDrawnObjectColor(object.id, Color(1., 1., 0.));
}

GalaxyApp::~GalaxyApp() {
    glfwTerminate();
}

void GalaxyApp::renderFrame() {
    // Clears the window with current clearing color, clears also the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws scene
    _scene.drawObjectsOfScene();
    
    // Performs the buffer swap between the current shown buffer, and the one we just worked on
    glfwSwapBuffers();
}

void GalaxyApp::animate() {
    //_scene.setDrawnObjectModel(0,xRotation(frand()));
    ((MoveableCamera*)_scene.pCamera)->move();
}

void GalaxyApp::eventDetection() {
    // Get mouse motion
    _exMouseXPos = _mouseXPos;
    _exMouseYPos = _mouseYPos;
    glfwGetMousePos(&_mouseXPos, &_mouseXPos);

    // Get keys
    _leftButton = glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT );
    _zButton = glfwGetKey('Z');
    _qButton = glfwGetKey('Q');
    _sButton = glfwGetKey('S');
    _dButton = glfwGetKey('D');

    //std::cout << "_dButton : " << _dButton << std::endl;  

    char key = NULL;
    if(glfwGetKey('Z')) { key = 'z'; }
    else if(glfwGetKey('Q')) { key = 'q'; }
    else if(glfwGetKey('S')) { key = 's'; }
    else if(glfwGetKey('D')) { key = 'd'; }

    std::cout << "key : " << key << std::endl;  
            
    Direction to = NOWHERE;
    switch(key) {
        case 'z' :
            to = FORWARD; break;
        case 's' :
            to = BACKWARD; break;
        case 'q' :
            to = LEFT;  break;
        case 'd' :
            to = RIGHT; break;
        default : break;
    }
    
    std::cout << "to : " << to << std::endl;  
    
    if(to != NOWHERE)
        ((MoveableCamera*)_scene.pCamera)->setMovement(to, true);
    else
        ((MoveableCamera*)_scene.pCamera)->setMovement(to, false);
}