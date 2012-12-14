#include "GalaxyApp.hpp"
#include "physics/PhysicManager.hpp"
#include "MoveableCamera.hpp"

#include "GL/glfw.h"
#include <stein/Scene.hpp>
#include <stein/Object.hpp>
#include <stein/Tools.hpp>
#include <stein/Builders.hpp>
#include <stein/GLHeaders.hpp>
#include <stein/math/StreamUtils.h>
#include <stein/math/Vector3f.hpp>

#include <iostream>

#include <vector>

using namespace std;
using namespace stein;

GalaxyApp::GalaxyApp() : Application(WIDTH, HEIGHT) {

    const float size = .06;

    _exMouseXPos = WIDTH/2;
    _exMouseYPos = HEIGHT/2;

    _scene.pCamera = new MoveableCamera();
    _scene.pCamera->setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _scene.pCamera->setPosition(Vector3f(0, 0, -5));
    _scene.setDefaultShaderID(loadProgram("shaders/1.glsl"));

    Object &object3 = _scene.createObject(GL_TRIANGLES);
    MeshBuilder meshBuilder3 = MeshBuilder();
    buildSkybox(object3, 1.,meshBuilder3);

    GLuint obj4 =_scene.addObjectToDraw(object3.id);
    _scene.setDrawnObjectColor(obj4, Color(1., 1., 1.));
   // _scene.setDrawnObjectModel(obj4, scale(Vector3f( 5. , 5., 5.)));

}

GalaxyApp::~GalaxyApp() {
    glfwTerminate();
}

void GalaxyApp::animate() {
    //_scene.setDrawnObjectModel(0,xRotation(frand()));
    ((MoveableCamera*)_scene.pCamera)->move();
    physicManager.applySpring();
    physicManager.solve();
}

void GalaxyApp::mouseEvent() {
    // Updating mouse data
    _exMouseXPos = _mouseXPos;
    _exMouseYPos = _mouseYPos;
    glfwGetMousePos(&_mouseXPos, &_mouseYPos);

    // Mouse Camera Movement
    ((MoveableCamera*)_scene.pCamera)->setMouseMovement(_mouseXPos - _exMouseXPos, _mouseYPos - _exMouseYPos);
}

void GalaxyApp::keyEvent() {
    // Get keboard motion + movement setting
    if(glfwGetKey('Z') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(FORWARD);
    if(glfwGetKey('Q') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(LEFT);
    if(glfwGetKey('S') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(BACKWARD);
    if(glfwGetKey('D') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(RIGHT);
    if(glfwGetKey('H') == GLFW_PRESS) hideCursor('H');
}