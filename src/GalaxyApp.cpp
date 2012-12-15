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
    GLuint shaderID = loadProgram("shaders/1.glsl");
    _scene.setDefaultShaderID(shaderID);
    setTextureUnitsInShader(shaderID);
   /* Object &face = _scene.createObject(GL_TRIANGLES);
    MeshBuilder faceBuilder = MeshBuilder();
    buildSquare(face, 1., faceBuilder);

    GLuint left =_scene.addObjectToDraw(face.id);
    _scene.setDrawnObjectColor(left, Color(0., 0., 1.));
	//_scene.setDrawnObjectModel(left, translation(Vector3f(1., 0., 0.)) * yRotation(3.14/2.));

    GLuint right =_scene.addObjectToDraw(face.id);
    _scene.setDrawnObjectColor(right, Color(0., 1., 0.));
	_scene.setDrawnObjectModel(right,  translation(Vector3f(3., 0., 0.)) * yRotation(3.14/2.));*/
   

	Object &cubeObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder cubeBuilder = MeshBuilder();
    buildCube(cubeObject, 1., cubeBuilder);

    GLuint cube =_scene.addObjectToDraw(cubeObject.id);
    _scene.setDrawnObjectColor(cube, Color(1., 1., 1.));
	//_scene.setDrawnObjectModel(sky, scale(Vector3f( 5. , 5., 5.)));

    //SKYBOX
    GLuint texID = loadTexture("textures/sand.ppm");

    Object &skyObject = _scene.createObject(GL_TRIANGLES);
    skyObject.setTextureId(texID);

    MeshBuilder skyBuilder = MeshBuilder();
    buildSkybox(skyObject, 1., skyBuilder);

    GLuint sky =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectColor(sky, Color(1., 1., 1.));
	_scene.setDrawnObjectModel(sky, scale(Vector3f( 20. , 20., 20.)));
    
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