#include "GalaxyApp.hpp"
#include "MoveableCamera.hpp"

#include "GL/glfw.h"
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

    exMouseXPos = WIDTH/2;
    exMouseYPos = HEIGHT/2;

    _scene.pCamera = new MoveableCamera();
    _scene.pCamera->setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _scene.pCamera->setPosition(Vector3f(0, 0, -5));
    _scene.setDefaultShaderID(loadProgram("shaders/1.glsl"));

 //   GLuint tex = loadTexture();

    /*Object &object1 = _scene.createObject(GL_TRIANGLES);
	MeshBuilder meshBuilder = MeshBuilder();
    buildSphere(object1, 0.1, 5, 5, meshBuilder);

    GLuint obj1 =  _scene.addObjectToDraw(object1.id);
    _scene.setDrawnObjectColor(obj1, Color(1., 1., 0.));
    _scene.setDrawnObjectModel(obj1, translation(Vector3f( 2.0, 0., 0.)));

    GLuint obj2 =_scene.addObjectToDraw(object1.id);
    _scene.setDrawnObjectColor(obj2, Color(1., 0., 0.));
    _scene.setDrawnObjectModel(obj2, translation(Vector3f( 1.0 , 0., 0.)));
   
	Object &object2 = _scene.createObject(GL_TRIANGLES);
	MeshBuilder meshBuilder2 = MeshBuilder();
    buildRepere(object2, meshBuilder2);

    GLuint obj3 =_scene.addObjectToDraw(object2.id);
    _scene.setDrawnObjectColor(obj3, Color(1., 0., 0.));*/

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
    //cout << " Camera : " << _scene.pCamera->getPosition() << endl;
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