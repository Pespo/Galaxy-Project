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

void GLFWCALL mouseEvent(int x, int y) {
    // Mouse Camera Movement
    ((MoveableCamera*)GalaxyApp::_scene.pCamera)->setMouseMovement(x - GalaxyApp::_mouseXPos, y - GalaxyApp::_mouseYPos);

    // Updating mouse data
    GalaxyApp::_mouseXPos = x;
    GalaxyApp::_mouseYPos = y;
}

void GLFWCALL keyEvent(int key, int state) {
    // Get keboard motion + movement setting
    if(key == 'z' && state == GLFW_PRESS) { ((MoveableCamera*)GalaxyApp::_scene.pCamera)->setKeyMovement(Direction(FORWARD)); }
    if(key == 'q' && state == GLFW_PRESS) { ((MoveableCamera*)GalaxyApp::_scene.pCamera)->setKeyMovement(Direction(LEFT)); }
    if(key == 's' && state == GLFW_PRESS) { ((MoveableCamera*)GalaxyApp::_scene.pCamera)->setKeyMovement(Direction(BACKWARD)); }
    if(key == 'd' && state == GLFW_PRESS) { ((MoveableCamera*)GalaxyApp::_scene.pCamera)->setKeyMovement(Direction(RIGHT)); }
}

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

    initEventDetection();
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

void GalaxyApp::initEventDetection() {
    glfwSetMousePosCallback(mouseEvent);
    glfwSetKeyCallback(keyEvent);
}