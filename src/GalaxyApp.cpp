#include "GalaxyApp.hpp"
#include "physics/PhysicManager.hpp"
#include "MoveableCamera.hpp"

#include "gui/imgui.h"
#include "gui/imguiRenderGL.h"

#include "GL/glfw.h"
#include <stein/Material.hpp>
#include <stein/Light.hpp>
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

enum shaderType {
    COLOR, TEXTURE, MATERIAL
};

enum materialType {
    JADE, CHROME
};

GalaxyApp::GalaxyApp() : Application(WIDTH, HEIGHT) {

    _exMouseXPos = WIDTH/2;
    _exMouseYPos = HEIGHT/2;

    MoveableCamera * camera = initCamera(.06, Vector3f(0, 0, -5));

    // Loaders
    loadShaders();
    // Builds
    //buildSkybox(100);
    buildWoman(10);

    //initGUI();

}

GalaxyApp::~GalaxyApp() {
    //delete [] shaders;
    glfwTerminate();
}

// Load shaders
void GalaxyApp::loadShaders() {
    shaders[COLOR] = loadProgram("shaders/colorShader.glsl");
    shaders[TEXTURE] = loadProgram("shaders/textureShader.glsl");
    shaders[MATERIAL] = loadProgram("shaders/materialShader.glsl");
    _scene.setDefaultShaderID(shaders[COLOR]);
}

MoveableCamera* GalaxyApp::initCamera(const float size, Vector3f position) {
    _scene.pCamera = new MoveableCamera();
    _scene.pCamera->setPerspectiveProjection(-size, size, -size, size, .1, 100);
    _scene.pCamera->setPosition(position);
    return (MoveableCamera*)_scene.pCamera;
}

/*void GalaxyApp::initGUI() {
    // Init UI
    if (!imguiRenderGLInit("DroidSans.ttf"))
    {
        fprintf(stderr, "Could not init GUI renderer.\n");
        exit(EXIT_FAILURE);
    }

    _bias = 0.001f;
}

void GalaxyApp::drawGUI() {
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	size_t width = getWindowWidth();
	size_t height = getWindowHeight();
    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float orthoUI[16];
    ortho(0, width, 0, height, 0.0, 1.0, orthoUI);
    glLoadMatrixf(orthoUI);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glUseProgram(0);

    unsigned char mbut = 0;
    int mscroll = 0;
    int mousex; int mousey;
    glfwGetMousePos(&mousex, &mousey);
    mousey = height - mousey;

    if(_leftButtonPressed)
        mbut |= IMGUI_MBUT_LEFT;

    imguiBeginFrame(mousex, mousey, mbut, mscroll);
    const char msg[] = "GUI Helper";
    int logScroll = 0;
    imguiBeginScrollArea("Settings", width - 210, height - 310, 200, 300, &logScroll);
    imguiSlider("bias", &_bias, 0.0000, 0.1, 0.0005);
    imguiEndScrollArea();
    imguiEndFrame();

    imguiRenderGLDraw();
    glDisable(GL_BLEND);
}*/

void GalaxyApp::animate() {
    //_scene.setDrawnObjectModel(0,xRotation(frand()));
    ((MoveableCamera*)_scene.pCamera)->move();
    physicManager.applySprings();
    physicManager.solve();
}

void GalaxyApp::renderFrame() {
    // Clears the window with current clearing color, clears also the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws scene
    _scene.drawObjectsOfScene();

    // Draws GUI
    //drawGUI();

    // Performs the buffer swap between the current shown buffer, and the one we just worked on
    glfwSwapBuffers();
}

void GalaxyApp::mouseMotionEvent() {
    // Updating mouse data
    _exMouseXPos = _mouseXPos;
    _exMouseYPos = _mouseYPos;
    glfwGetMousePos(&_mouseXPos, &_mouseYPos);

    // Mouse Camera Movement
    ((MoveableCamera*)_scene.pCamera)->setMouseMovement(_mouseXPos - _exMouseXPos, _mouseYPos - _exMouseYPos);
}

void GalaxyApp::mouseButtonEvent() {
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) _leftButtonPressed = true;
    else _leftButtonPressed = false;
}

void GalaxyApp::keyEvent() {
    // Get keboard motion + movement setting
    if(glfwGetKey('Z') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(FORWARD);
    if(glfwGetKey('Q') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(LEFT);
    if(glfwGetKey('S') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(BACKWARD);
    if(glfwGetKey('D') == GLFW_PRESS) ((MoveableCamera*)_scene.pCamera)->setKeyMovement(RIGHT);
    if(glfwGetKey('H') == GLFW_PRESS) hideCursor('H');
}

// Builds the skybox
void GalaxyApp::buildSkybox(size_t size) {
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

// Builds the woman
void GalaxyApp::buildWoman(size_t size) {
    Object &womanObject = _scene.createObject(GL_TRIANGLES);

    MeshBuilder womanBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(womanObject, "res/objs/woman.obj", false, false, womanBuilder);

    GLuint woman =_scene.addObjectToDraw(womanObject.id);
    _scene.setDrawnObjectShaderID(woman, shaders[MATERIAL]);
    _scene.setDrawnObjectColor(woman, Color(1., 1., 0.));
    _scene.setDrawnObjectModel(woman, translation(Vector3f( 0. , 0., 0.)) * scale(Vector3f( 5. , 5., 5.)));
    _scene.setDrawnObjectMaterialID(woman, JADE);
}