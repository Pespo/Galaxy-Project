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

    _exMouseXPos = WIDTH/2;
    _exMouseYPos = HEIGHT/2;

    MoveableCamera * camera = initCamera(.06, Vector3f(0, 0, -5));

    GLuint shaderID = loadProgram("shaders/1.glsl");
    _scene.setDefaultShaderID(shaderID);

    setSkybox(100);
    

}

GalaxyApp::~GalaxyApp() {
    glfwTerminate();
}

MoveableCamera* GalaxyApp::initCamera(const float size, Vector3f position){
    _scene.pCamera = new MoveableCamera();
    _scene.pCamera->setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _scene.pCamera->setPosition(position);
    return (MoveableCamera*)_scene.pCamera;
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

void GalaxyApp::setSkybox(size_t size) {
    Object &skyObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder skyBuilder = MeshBuilder();
    buildSquare(skyObject, size, skyBuilder);

    GLuint right =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(right, translation(Vector3f(size/2, 0., 0.)) * yRotation(-3.14/2) * zRotation(3.14));
    _scene.setDrawnObjectTextureID(right, 0, loadTexture( "textures/skybox/right.tga", 3));
     
    GLuint left =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(left, translation(Vector3f(-1. * size/2, 0., 0.)) * yRotation(3.14/2) * zRotation(3.14));
    _scene.setDrawnObjectTextureID(left, 0,loadTexture( "textures/skybox/left.tga", 3));

    GLuint front =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(front,translation(Vector3f(0., 0., size/2)) * yRotation(3.14) * zRotation(3.14));
    _scene.setDrawnObjectTextureID(front, 0, loadTexture( "textures/skybox/front.tga", 3));

    GLuint back =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(back, translation(Vector3f(0., 0., -1. * size/2)) * zRotation(3.14));
    _scene.setDrawnObjectTextureID(back, 0, loadTexture( "textures/skybox/back.tga", 3));

    GLuint top =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(top,translation(Vector3f(0., size/2, 0.)) * xRotation(-3.14/2) * zRotation(-3.14/2));
    _scene.setDrawnObjectTextureID(top, 0, loadTexture( "textures/skybox/top.tga", 3));

    GLuint bot =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(bot,translation(Vector3f(0., -1. * size/2, 0.)) * xRotation(3.14/2) * zRotation(3.14/2));
    _scene.setDrawnObjectTextureID(bot, 0, loadTexture( "textures/skybox/bot.tga", 3));
}