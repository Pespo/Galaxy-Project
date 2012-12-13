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

    exMouseXPos = 0;
    exMouseYPos = 0;

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

void GalaxyApp::mouseEvent() {
    // Updating mouse data
    exMouseXPos = _mouseXPos;
    exMouseYPos = _mouseYPos;
    glfwGetMousePos(&_mouseXPos, &_mouseYPos);

    // Mouse Camera Movement
    ((MoveableCamera*)_scene.pCamera)->setMouseMovement(_mouseXPos - exMouseXPos, _mouseYPos - exMouseYPos);
}

void GalaxyApp::keyEvent() {
    // Get keboard motion + movement setting
    if(glfwGetKey('Z') == GLFW_PRESS) { ((MoveableCamera*)_scene.pCamera)->setKeyMovement(FORWARD); }
    if(glfwGetKey('Q') == GLFW_PRESS) { ((MoveableCamera*)_scene.pCamera)->setKeyMovement(LEFT); }
    if(glfwGetKey('S') == GLFW_PRESS) { ((MoveableCamera*)_scene.pCamera)->setKeyMovement(BACKWARD); }
    if(glfwGetKey('D') == GLFW_PRESS) { ((MoveableCamera*)_scene.pCamera)->setKeyMovement(RIGHT); }
    if(glfwGetKey('H') == GLFW_PRESS) { hideCursor('H'); }
}