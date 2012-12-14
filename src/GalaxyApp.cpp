#include "GalaxyApp.hpp"
#include "physics/PhysicManager.hpp"
#include "MoveableCamera.hpp"

#include "GL/glfw.h"
#include <stein/Scene.hpp>
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

    _exMouseXPos = 0;
    _exMouseYPos = 0;

    _pScene = new Scene();

    _pScene->pCamera = new MoveableCamera();
    _pScene->pCamera->setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _pScene->pCamera->setPosition(Vector3f(0, 0, 5));
    _pScene->setDefaultShaderID(loadProgram("shaders/1.glsl"));

    // Particle 1
    Object &object = _pScene->createObject(GL_TRIANGLES);
	MeshBuilder meshBuilder = MeshBuilder();
    buildSphere(object, 0.1, 5, 5, meshBuilder);

    GLuint instanceId = _pScene->addObjectToDraw(object.id);
    GLuint particleId = physicManager.addPhysicToObject(instanceId);
    _pScene->setDrawnObjectColor(instanceId, Color(1., 1., 0.));
    _pScene->setDrawnObjectModel(instanceId, translation(Vector3f(1., 0., 0.)));

    // Particle2
    Object &object2 = _pScene->createObject(GL_TRIANGLES);
    MeshBuilder meshBuilder2 = MeshBuilder();
    buildSphere(object2, 0.1, 5, 5, meshBuilder2);

    GLuint instance2Id = _pScene->addObjectToDraw(object2.id);
    GLuint particle2Id = physicManager.addPhysicToObject(instance2Id);
    _pScene->setDrawnObjectColor(instance2Id, Color(0., 1., 1.));
    _pScene->setDrawnObjectModel(instance2Id, translation(Vector3f(1., 0., 0.)));
}

GalaxyApp::~GalaxyApp() {
    glfwTerminate();
}

void GalaxyApp::animate() {
    //_pScene->setDrawnObjectModel(0,xRotation(frand()));
    ((MoveableCamera*)_pScene->pCamera)->move();
    physicManager.applySpring();
    physicManager.solve();

    _pScene->setDrawnObjectModel(0, translation(physicManager.physicalObjects[0]->m_position));
    _pScene->setDrawnObjectModel(1, translation(physicManager.physicalObjects[1]->m_position));

}

void GalaxyApp::mouseEvent() {
    // Updating mouse data
    _exMouseXPos = _mouseXPos;
    _exMouseYPos = _mouseYPos;
    glfwGetMousePos(&_mouseXPos, &_mouseYPos);

    // Mouse Camera Movement
    ((MoveableCamera*)_pScene->pCamera)->setMouseMovement(_mouseXPos - _exMouseXPos, _mouseYPos - _exMouseYPos);
}

void GalaxyApp::keyEvent() {
    // Get keboard motion + movement setting
    if(glfwGetKey('Z') == GLFW_PRESS) ((MoveableCamera*)_pScene->pCamera)->setKeyMovement(FORWARD);
    if(glfwGetKey('Q') == GLFW_PRESS) ((MoveableCamera*)_pScene->pCamera)->setKeyMovement(LEFT);
    if(glfwGetKey('S') == GLFW_PRESS) ((MoveableCamera*)_pScene->pCamera)->setKeyMovement(BACKWARD);
    if(glfwGetKey('D') == GLFW_PRESS) ((MoveableCamera*)_pScene->pCamera)->setKeyMovement(RIGHT);
    if(glfwGetKey('H') == GLFW_PRESS) hideCursor('H');
}