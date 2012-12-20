#include "GalaxyApp.hpp"
#include "physics/PhysicManager.hpp"
#include "MoveableCamera.hpp"

#include "gui/GLcommon.hpp"
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
#include <cmath>
#include <vector>

#define M_PI  3.14159265f

using namespace std;
using namespace stein;

enum shaderType {
    COLOR, TEXTURE, MATERIAL
};

enum materialType {
    JADE, CHROME, RUBY, EMERAULD, GOLD
};

GalaxyApp::GalaxyApp() : Application(WIDTH, HEIGHT) {
    
    initGUI();

    _exMouseXPos = WIDTH/2;
    _exMouseYPos = HEIGHT/2;
    forceDragon = 50;

    MoveableCamera * camera = initCamera(.06, Vector3f(0, 0, -5));

    // Physics
    initPhysics();
    // Loaders
    loadShaders();
    // Builds
    buildSkybox(100);
    buildWoman(5.);
    buildStone(6.);

    setSystem();
    //setSystem2();
    setDragon();
	setPills();
	//PlaySound("res/chimes.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
   
}

GalaxyApp::~GalaxyApp() {
    glfwTerminate();
}

void GalaxyApp::initPhysics(){
    _bias = 0.1;
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

void GalaxyApp::initGUI() {
    // Init UI
    if (!imguiRenderGLInit("DroidSans.ttf"))
    {
        fprintf(stderr, "Could not init GUI renderer.\n");
        exit(EXIT_FAILURE);
    }

    //_bias = 0.001f;
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
    imguiSlider("Dragon speed", &forceDragon, 50, 500, 5);
    imguiSlider("Dragon speed", &systems[1].attraction.m_force, 1, 50,1);
	imguiSlider("Shpere radius", &systems[0].hookSpring.m_freeLength, 0.1, 5, 0.1);
    imguiSlider("Shpere resistance", &systems[0].hookSpring.m_k, 1, 100, 1);
    imguiSlider("Shpere cinetic brake", &systems[0].cineticBrake.m_z, 0.0000000001, 1. , 0.000001 );
    imguiEndScrollArea();
    imguiEndFrame();

    imguiRenderGLDraw();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GalaxyApp::animate() {
       
    ((MoveableCamera*)_scene.pCamera)->move();

   
  
    ////////System 1
    systems[0].applySprings();
    systems[0].solve();
    for(int j = 0; j<systems[0].physicalObjects.size(); ++j){
        _scene.setDrawnObjectModel(systems[0].physicalObjects[j]->m_ObjectInstanceId, translation(systems[0].physicalObjects[j]->m_position) * yRotation(frand()) * xRotation(frand()) * zRotation(frand())); 
    }

    ////////System 2
    
    // systems[1].applySprings();
    // systems[1].attraction.generateForces(systems[1].physicalObjects[0], systems[1].physicalObjects[ systems[1].physicalObjects.size() - 1]);
    // systems[1].solve();
    // for(int j = 0; j<systems[1].physicalObjects.size(); ++j){
    //     _scene.setDrawnObjectModel(systems[1].physicalObjects[j]->m_ObjectInstanceId, translation(systems[1].physicalObjects[j]->m_position) ); 
    // }
    

            // zero = Vector3f(0., 1., 0.);
            // velocity = Vector3f(systems[i].physicalObjects[0]->m_velocity.x, systems[i].physicalObjects[0]->m_velocity.y , systems[i].physicalObjects[0]->m_velocity.z);
            // velocity = velocity.normalize();
            // if(velocity.norm() ) {
            //     rotY = zero.dotP(velocity) / (velocity.norm() * zero.norm())  ;
            //     rotY = acos(rotY);
            // } else systems[i].physicalObjects[0]->m_rotation.y = 0;

            // Matrix4f rotate = Matrix4f(zero.x*zero.x + (1 - zero.x*zero.x) * cos(rotY), zero.x*zero.y * (1 - cos(rotY)) - zero.z * sin(rotY), zero.x*zero.z * (1 - cos(rotY))+zero.y * sin(rotY), 0,
            //                            zero.x*zero.y * (1 - cos(rotY)) + zero.z * sin(rotY), zero.y*zero.y + (1 - zero.y*zero.y) * cos(rotY), zero.y*zero.z * (1 - cos(rotY))-zero.x * sin(rotY), 0, 
            //                            zero.x*zero.z * (1 - cos(rotY)) - zero.y * sin(rotY), zero.y*zero.z * (1 - cos(rotY)) + zero.x * sin(rotY), zero.z*zero.z + (1 - zero.z*zero.z) * cos(rotY), 0,
            //                            0, 0, 0, 1);

        
    float d = 0;
    Vector3f direction;
    for(int i = 0; i < systems[2].physicalObjects.size(); ++i) {
        if((systems[1].physicalObjects[0]->m_position - systems[2].physicalObjects[i]->m_position).norm() > (d)){
            d = (systems[1].physicalObjects[0]->m_position - systems[2].physicalObjects[i]->m_position).norm();
            direction = systems[2].physicalObjects[i]->m_position;
        }
    }

    systems[1].physicalObjects[0]->m_force = direction * forceDragon;

    Vector3f zero(1., 0., 0.);
    float rotY = 0;
    //float rotX = 0;
    Vector3f velocity = zero;
    //for (int i = 0; i < systems.size() ; ++i) {

        //Rotation en Y
        zero = Vector3f(1., 0., 0.);
        velocity = Vector3f(systems[1].physicalObjects[0]->m_velocity.x, /*systems[1].physicalObjects[0]->m_velocity.y*/0 , systems[1].physicalObjects[0]->m_velocity.z);
        //velocity = velocity.normalize();
        if(velocity.norm() ) {
            rotY = zero.dotP(velocity) / velocity.norm() ;
            rotY = (rotY<0) ? - acos(rotY) : acos(rotY);
            if(rotY < (systems[1].physicalObjects[0]->m_rotation.y - 0.1))
                systems[1].physicalObjects[0]->m_rotation.y -= 0.1;
            else if(rotY > (systems[1].physicalObjects[0]->m_rotation.y + 0.1))
				systems[1].physicalObjects[0]->m_rotation.y += 0.1;
            else systems[1].physicalObjects[0]->m_rotation.y = rotY; 
        } else systems[1].physicalObjects[0]->m_rotation.y = 0;

        systems[1].applySprings();
        systems[1].solve();
        for(int j = 0; j<systems[1].physicalObjects.size(); ++j){
            _scene.setDrawnObjectModel(systems[1].physicalObjects[j]->m_ObjectInstanceId, translation(systems[1].physicalObjects[j]->m_position)  * yRotation(systems[1].physicalObjects[j]->m_rotation.y) * scale(Vector3f(systems[1].physicalObjects[j]->m_mass, systems[1].physicalObjects[j]->m_mass, systems[1].physicalObjects[j]->m_mass))); 
        }

            //Rotation X
            //zero = Vector3f(1., 0., 0.);
            // velocity = Vector3f(systems[i].physicalObjects[j]->m_velocity.x, systems[i].physicalObjects[j]->m_velocity.y, /*systems[i].physicalObjects[j]->m_velocity.z)*/0. );
            // //velocity = velocity.normalize();
            // if(velocity.norm() ) {
            //     rotX = zero.dotP(velocity) / velocity.norm() ;
            //     if(rotX > M_PI/2) {
            //         rotX = acos(M_PI/2);  
            //     } else if(rotX < -M_PI/2) {
            //         rotX = - acos(M_PI/2);
            //     } else {
            //         rotX = (rotX < 0 ) ? - acos(rotX) : acos(rotX);
            //     }
                
            //     if(rotX < (systems[i].physicalObjects[j]->m_rotation.x - 0.1))
            //         systems[i].physicalObjects[j]->m_rotation.x -= 0.1;
            //     else if(rotY > (systems[i].physicalObjects[j]->m_rotation.x + 0.1))
            //         systems[i].physicalObjects[j]->m_rotation.x += 0.1;
            //     else systems[i].physicalObjects[j]->m_rotation.x = rotX; 
            // } else systems[i].physicalObjects[j]->m_rotation.x = 0;

}

void GalaxyApp::renderFrame() {
    // Clears the window with current clearing color, clears also the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws scene
    _scene.drawObjectsOfScene();

    // Draws GUI
    drawGUI();

    // Performs the buffer swap between the current shown buffer, and the one we just worked on
    glfwSwapBuffers();
}

void GalaxyApp::mouseMotionEvent() {
    // Updating mouse data
    _exMouseXPos = _mouseXPos;
    _exMouseYPos = _mouseYPos;
    glfwGetMousePos(&_mouseXPos, &_mouseYPos);

    if(!_bShowMouse)
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
    _scene.setDrawnObjectModel(right, translation(Vector3f(size/2, 0., 0.)) * yRotation(-M_PI/2) );
    _scene.setDrawnObjectTextureID(right, 0, loadTexture( "textures/skybox/right.tga"));
    _scene.setDrawnObjectShaderID(right, shaders[TEXTURE]);
     
    GLuint left =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(left, translation(Vector3f(-1. * size/2, 0., 0.)) * yRotation(M_PI/2) );
    _scene.setDrawnObjectTextureID(left, 0,loadTexture( "textures/skybox/left.tga"));
    _scene.setDrawnObjectShaderID(left, shaders[TEXTURE]);

    GLuint front =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(front,translation(Vector3f(0., 0., size/2)) * yRotation(M_PI) );
    _scene.setDrawnObjectTextureID(front, 0, loadTexture( "textures/skybox/front.tga"));
    _scene.setDrawnObjectShaderID(front, shaders[TEXTURE]);

    GLuint back =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(back, translation(Vector3f(0., 0., -1. * size/2)) );
    _scene.setDrawnObjectTextureID(back, 0, loadTexture( "textures/skybox/back.tga"));
    _scene.setDrawnObjectShaderID(back, shaders[TEXTURE]);

    GLuint top =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(top,translation(Vector3f(0., size/2, 0.)) * xRotation(-M_PI/2) );
    _scene.setDrawnObjectTextureID(top, 0, loadTexture( "textures/skybox/top.tga"));
    _scene.setDrawnObjectShaderID(top, shaders[TEXTURE]);

    GLuint bot =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(bot,translation(Vector3f(0., -1. * size/2, 0.)) * xRotation(M_PI/2));
    _scene.setDrawnObjectTextureID(bot, 0, loadTexture( "textures/skybox/bot.tga"));
    _scene.setDrawnObjectShaderID(bot, shaders[TEXTURE]);
}

// Builds the woman
void GalaxyApp::buildWoman(float size) {
    Object &womanObject = _scene.createObject(GL_TRIANGLES);

    MeshBuilder womanBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(womanObject, "res/objs/woman.obj", false, false, womanBuilder);

    GLuint woman =_scene.addObjectToDraw(womanObject.id);
    _scene.setDrawnObjectShaderID(woman, shaders[MATERIAL]);
    _scene.setDrawnObjectColor(woman, Color(1., 1., 0.));
    _scene.setDrawnObjectModel(woman, translation(Vector3f( -3.2 , -7.7, -2.5)) * yRotation(-M_PI/8) * scale(Vector3f( size , size, size)));
    _scene.setDrawnObjectMaterialID(woman, JADE);
}

void GalaxyApp::buildStone(float size) {
    Object &stoneObject = _scene.createObject(GL_TRIANGLES);

    MeshBuilder stoneBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(stoneObject, "res/objs/stone.obj", false, false, stoneBuilder);

    GLuint stone =_scene.addObjectToDraw(stoneObject.id);
    _scene.setDrawnObjectShaderID(stone, shaders[MATERIAL]);
    _scene.setDrawnObjectColor(stone, Color(1., 1., 0.));
    _scene.setDrawnObjectModel(stone, translation(Vector3f( -2.9 , -7.1, -2.5)) * yRotation(-M_PI/8) * scale(Vector3f( size , size, size)));
    _scene.setDrawnObjectMaterialID(stone, CHROME);
}

void GalaxyApp::setSystem(){

    Object &sphereObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder sphereBuilder = MeshBuilder();
    buildCube(sphereObject, 0.05, sphereBuilder);

    GLuint s = createSystem();
    GLuint mat = 2;
    for(int i = 0 ; i<50; ++i) {
        mat = mat < 3 ? 3 :2;
        GLuint instance =_scene.addObjectToDraw(sphereObject.id);
        //_scene.setDrawnObjectColor(instance, Color(cos(i * 50.),  sin(i * 50.),  sin(i * 50.)));
        _scene.setDrawnObjectShaderID(instance, shaders[MATERIAL]);
        _scene.setDrawnObjectMaterialID(instance, mat);
        GLuint particle = systems[s].addPhysicToObject(instance);
        systems[s].setPhysicObjectPosition(particle, Vector3f((0.5 - frand())/2., (0.5 - frand())/2., (0.5 - frand())/2.));//(i/5., 0., 0.));
    }

    systems[s].setHookSpring(500, 0.5);   
    systems[s].setCineticBrake(0.00001);
}

void GalaxyApp::setSystem2(){

    Object &sphereObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder sphereBuilder = MeshBuilder();
    buildCube(sphereObject, 0.05, sphereBuilder);

    GLuint s = createSystem();
    GLuint mat = 2;
    for(int i = 0 ; i<25; ++i) {
        mat = mat < 3 ? 3 :2;
        GLuint instance =_scene.addObjectToDraw(sphereObject.id);
        //_scene.setDrawnObjectColor(instance, Color(cos(i * 50.),  sin(i * 50.),  sin(i * 50.)));
        _scene.setDrawnObjectShaderID(instance, shaders[MATERIAL]);
        _scene.setDrawnObjectMaterialID(instance, mat);
        GLuint particle = systems[s].addPhysicToObject(instance);
        systems[s].setPhysicObjectPosition(particle, Vector3f(cos(i * 50.), sin(i * 50.), sin(i * 50.)));//(i/5., 0., 0.));
    }

    //systems[s].setHookSpring(1, 0.5);   
    //systems[s].setCineticBrake(0.001);
    systems[s].setAttraction(10);
}

void GalaxyApp::setPills(){

    Object &sphereObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder sphereBuilder = MeshBuilder();
    buildCube(sphereObject, 0.1, sphereBuilder);

    GLuint s = createSystem();
    for(int i = 0 ; i<10; ++i) {
        GLuint instance =_scene.addObjectToDraw(sphereObject.id);
        _scene.setDrawnObjectColor(instance, Color(cos(i * 50.),  sin(i * 50.),  sin(i * 50.)));
        _scene.setDrawnObjectShaderID(instance, shaders[MATERIAL]);
        _scene.setDrawnObjectMaterialID(instance, JADE);
        GLuint particle = systems[s].addPhysicToObject(instance);
        systems[s].setPhysicObjectPosition(particle, Vector3f((0.5 - frand())*5., (0.5 - frand())*5., (0.5 - frand())*5.));//(i/5., 0., 0.));
    }
}

void GalaxyApp::setDragon(){

    GLuint s = createSystem();

    Object &dragonObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder dragonBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(dragonObject, "res/objs/dragonHead.obj", false, false, dragonBuilder);

    GLuint instance =_scene.addObjectToDraw(dragonObject.id);
    _scene.setDrawnObjectShaderID(instance, shaders[MATERIAL]);
    _scene.setDrawnObjectMaterialID(instance, GOLD);
    GLuint particle = systems[s].addPhysicToObject(instance);
    systems[s].setPhysicObjectPosition(particle, Vector3f(-1., 0., 0.));
    systems[s].setPhysicObjectMass(particle, 1);

    Object &sphereObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder sphereBuilder = MeshBuilder();
    //buildCube(sphereObject, 0.05, sphereBuilder);
    buildObjectGeometryFromOBJ(sphereObject, "res/objs/sphere.obj", false, false, sphereBuilder);

    for(int i = 0 ; i<20; ++i) {
        instance =_scene.addObjectToDraw(sphereObject.id);
        _scene.setDrawnObjectColor(instance, Color(cos(i * 50.),  sin(i * 50.),  sin(i * 50.)));
        _scene.setDrawnObjectShaderID(instance, shaders[MATERIAL]);
        _scene.setDrawnObjectMaterialID(instance, GOLD);
        particle = systems[s].addPhysicToObject(instance);
        systems[s].setPhysicObjectPosition(particle, Vector3f(-1 - i, 0., 0.));
        systems[s].setPhysicObjectMass(particle, 0.1);
    }

    systems[s].setAttraction(20);
}

GLuint GalaxyApp::createSystem() {
    const size_t size = systems.size();
    if (size >= 20)
        throw std::runtime_error("maximum number of system reached");
    systems.push_back(PhysicManager());
    return size;
}
