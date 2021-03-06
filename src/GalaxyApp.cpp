#include "GalaxyApp.hpp"
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
#include <stein/FramebufferGL.hpp>
#include <stein/math/StreamUtils.h>
#include <stein/math/Vector3f.hpp>
#include <windows.h>
#include <mmsystem.h>

#include <iostream>
#include <cmath>
#include <vector>

#define M_PI  3.14159265f

using namespace std;
using namespace stein;

enum shaderType {
    COLOR, SKY, TEXTURE, CUBEMAP, MATERIAL, SHADOW, LIGHT, BLUR, COC, DOF, GAMMA
};

enum materialType {
    JADE, CHROME, RUBY, EMERAULD, GOLD
};

enum systemType {
    SPHERE, DRAGON, PILLS    
};

GalaxyApp::GalaxyApp() : Application(WIDTH, HEIGHT) {

    setTitle("Galaxy-Project / Guillaume ALBESPY & Guillaume BREDA");
    
    initGUI();

    _exMouseXPos = WIDTH/2;
    _exMouseYPos = HEIGHT/2;
    forceDragon = 50;

    MoveableCamera * camera = initCamera(.06, Vector3f(0, 0, -5));

    // Loaders
    loadShaders();
    initFramebuffer();

     //init finalScreen
    Object &squareObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder squareBuilder = MeshBuilder();
    buildSquare(squareObject, 1, squareBuilder);
    finalScreen =_scene.addObjectToDraw(squareObject.id);

    //variable effect
    focus = Vector3f(5.0, 1.0, 10);
    sampleBlur = 4.;
    sobelCoef = 0.5;
    gamma = 1.3;
    gui=true;

    // Builds
    setSkybox(100);    
    setWoman(5.);
    setStone(6.);
	setSystem();
    setDragon();
	setPills();

   PlaySound("res/sounds/ambient.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
}

GalaxyApp::~GalaxyApp() {
    glfwTerminate();
}

// Load shaders
void GalaxyApp::loadShaders() {
    shaders[COLOR] = loadProgram("shaders/colorShader.glsl");
    shaders[SKY] = loadProgram("shaders/skyShader.glsl");
    shaders[TEXTURE] = loadProgram("shaders/textureShader.glsl");
    shaders[MATERIAL] = loadProgram("shaders/materialShader.glsl");
    shaders[SHADOW] = loadProgram("shaders/shadowShader.glsl");
    shaders[LIGHT] = loadProgram("shaders/lightShader.glsl");
    shaders[BLUR] = loadProgram("shaders/blurShader.glsl");
    shaders[COC] = loadProgram("shaders/cocShader.glsl");
    shaders[DOF] = loadProgram("shaders/dofShader.glsl");
    shaders[GAMMA] = loadProgram("shaders/gammaShader.glsl");
    _scene.setDefaultShaderID(shaders[COLOR]);
}

// Init frame buffers
void GalaxyApp::initFramebuffer() {
    int status = build_framebuffer(gbufferFB, WIDTH, HEIGHT, 2);

    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

    status = build_framebuffer(lightFB, WIDTH, HEIGHT, 1);

    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

    status = build_framebuffer(cocFB, WIDTH, HEIGHT, 1);

    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

    int blurWidth = WIDTH / 2;
    int blurHeight = HEIGHT / 2;

    status = build_framebuffer(hblurFB, blurWidth, blurHeight, 1);

    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

    status = build_framebuffer(vblurFB, blurWidth, blurHeight, 1);

    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

    status = build_framebuffer(cocFB, WIDTH, HEIGHT, 1);

    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }
    
    status = build_framebuffer(dofFB, WIDTH, HEIGHT, 1);
    
    if (status == -1)
    {
        fprintf(stderr, "Error on building framebuffer\n");
        exit( EXIT_FAILURE );
    }

}

MoveableCamera* GalaxyApp::initCamera(const float size, Vector3f position) {
    _scene.pCamera = new MoveableCamera();
    _scene.pCamera->setPerspectiveProjection(-size, size, -size*9/16, size*9/16, .1, 100);
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
    imguiBeginScrollArea("Settings", width - 210, height - 360, 200, 350, &logScroll);
    imguiSlider("Dragon speed", &forceDragon, 1, 60, 1);
    imguiSlider("Dragon stretch", &systems[DRAGON].attraction.m_force, 1, 50,1);
	imguiSlider("Shpere radius", &systems[SPHERE].hookSpring.m_freeLength, 0.1, 3, 0.1);
    imguiSlider("Shpere resistance", &systems[SPHERE].hookSpring.m_k, 1, 100, 1);
    imguiSlider("Shpere c.brake", &systems[SPHERE].cineticBrake.m_z, 0.0001, 0.01 , 0.0001 );
    imguiSlider("Focus", &focus.x, 0.1, 20.0, 0.05);
    imguiSlider("Near", &focus.y, 0.1, 20.0, 0.05);
    imguiSlider("Far", &focus.z, 0.1, 20.0, 0.05);
    imguiSlider("SampleCount", &sampleBlur, 1., 10. , 1. );
    imguiSlider("Gamma", &gamma, 0.0, 2.0, 0.1);
    imguiSlider("Sobel", &sobelCoef, 0.0, 1.0, 0.1);
    imguiSlider("Move step", &((MoveableCamera*)_scene.pCamera)->MOVE_STEP, 0.0, 0.2, 0.001);
    imguiSlider("Mouse step", &((MoveableCamera*)_scene.pCamera)->MOUSE_STEP, 0.0, 0.8, 0.01);

        
    imguiEndScrollArea();
    imguiEndFrame();

    imguiRenderGLDraw();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GalaxyApp::animate() {
       
    ((MoveableCamera*)_scene.pCamera)->move();

    ////////System 1 : Particule dans la main
        //apply spring

        systems[SPHERE].hookSpring.m_freeLength += (radiusSphere) ? 0.05 : -0.05;
        if(systems[SPHERE].hookSpring.m_freeLength < 0.7 && !radiusSphere) radiusSphere = true;
        if(systems[SPHERE].hookSpring.m_freeLength > 1.8 && radiusSphere) radiusSphere = false;
        for(int i = 1 ; i < systems[SPHERE].physicalObjects.size(); ++i) {
            for(int j=i+1; j < systems[SPHERE].physicalObjects.size(); ++j){
                systems[SPHERE].hookSpring.generateForces(systems[SPHERE].physicalObjects[i], systems[SPHERE].physicalObjects[j]);
                systems[SPHERE].cineticBrake.generateForces(systems[SPHERE].physicalObjects[i], systems[SPHERE].physicalObjects[j]);
            }
        }
        /*for(int i = 1 ; i < systems[SPHERE].physicalObjects.size(); ++i) {
           systems[SPHERE].attraction.generateForces(systems[SPHERE].physicalObjects[i], systems[SPHERE].physicalObjects[i-1]);   
        }*/
        systems[SPHERE].solve();

        //Set movement
        for(int j = 0; j<systems[SPHERE].physicalObjects.size(); ++j){
            systems[SPHERE].physicalObjects[j]->m_rotation.y +=0.1 + frand()/100;
            systems[SPHERE].physicalObjects[j]->m_rotation.x +=0.1 + frand()/100;
            systems[SPHERE].physicalObjects[j]->m_rotation.z +=0.1 + frand()/100;
            _scene.setDrawnObjectModel(systems[SPHERE].physicalObjects[j]->m_ObjectInstanceId, translation(systems[SPHERE].physicalObjects[j]->m_position) * yRotation(systems[SPHERE].physicalObjects[j]->m_rotation.y) * xRotation(systems[SPHERE].physicalObjects[j]->m_rotation.x) * zRotation(systems[SPHERE].physicalObjects[j]->m_rotation.z) * scale(Vector3f(systems[SPHERE].physicalObjects[j]->m_mass, systems[SPHERE].physicalObjects[j]->m_mass, systems[SPHERE].physicalObjects[j]->m_mass)) /*  zRotation(frand())*/); 
        }
    
    ///////System 2 : Cube aimant    
        float d = 0;
        Vector3f direction;
        for(int i = 0; i < systems[PILLS].physicalObjects.size(); ++i) {
            if((systems[DRAGON].physicalObjects[0]->m_position - systems[PILLS].physicalObjects[i]->m_position).norm() > (d)){
                d = (systems[DRAGON].physicalObjects[0]->m_position - systems[PILLS].physicalObjects[i]->m_position).norm();
                direction = systems[PILLS].physicalObjects[i]->m_position - systems[DRAGON].physicalObjects[0]->m_position;
            }
        }

    ///////System 3 : Dragon 
        systems[DRAGON].physicalObjects[0]->m_force = direction * forceDragon ; //Tête du dragon
        
        //Rotation en Y
        Vector3f zero = Vector3f(1., 0., 0.);
        float rotY = 0;
        Vector3f velocity = Vector3f(systems[DRAGON].physicalObjects[0]->m_velocity.x, 0. , systems[DRAGON].physicalObjects[0]->m_velocity.z);
        if(velocity.norm() ) {
            rotY = zero.dotP(velocity) / velocity.norm() ;
            rotY = (rotY<0) ? - acos(rotY) : acos(rotY);
            if(rotY < (systems[DRAGON].physicalObjects[0]->m_rotation.y - 0.1))
                systems[DRAGON].physicalObjects[0]->m_rotation.y -= 0.1;
            else if(rotY > (systems[DRAGON].physicalObjects[0]->m_rotation.y + 0.1))
    			systems[DRAGON].physicalObjects[0]->m_rotation.y += 0.1;
            else systems[DRAGON].physicalObjects[0]->m_rotation.y = rotY; 
        } else systems[DRAGON].physicalObjects[0]->m_rotation.y = 0;

        //apply spring
        for(int i = 1; i < systems[DRAGON].physicalObjects.size(); ++i) {
            systems[DRAGON].attraction.generateForces(systems[DRAGON].physicalObjects[i], systems[DRAGON].physicalObjects[i-1]);
        }
        systems[DRAGON].solve();

        //set movement
        for(int j = 0; j<systems[DRAGON].physicalObjects.size(); ++j){
            _scene.setDrawnObjectModel(systems[DRAGON].physicalObjects[j]->m_ObjectInstanceId, translation(systems[DRAGON].physicalObjects[j]->m_position)  * yRotation(systems[DRAGON].physicalObjects[j]->m_rotation.y) * scale(Vector3f(systems[DRAGON].physicalObjects[j]->m_mass, systems[DRAGON].physicalObjects[j]->m_mass, systems[DRAGON].physicalObjects[j]->m_mass))); 
        }
        //_scene.dragonPosition = systems[DRAGON].physicalObjects[1]->m_position;
}       

void GalaxyApp::renderFrame() {
    //gbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gbufferFB.fbo);
        glDrawBuffers(gbufferFB.outCount, gbufferFB.drawBuffers);
        glViewport( 0, 0, WIDTH, HEIGHT);

        // Default states
        glEnable(GL_DEPTH_TEST);

        // Clear the front buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draws scene
        _scene.drawObjectsOfScene();

        /*glEnable(GL_TEXTURE_CUBE_MAP);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);

        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

        glUseProgram(_scene.drawnObjects[1].shaderId); // From now on, this shader will be used.
        // We use the specific values of model per object
        setMatricesInShader(_scene.drawnObjects[1].shaderId, _scene.drawnObjects[1].transformation, _scene.pCamera->getView(), _scene.pCamera->getPosition(), _scene.pCamera->getProjection());

            //Selects our current texture for unit 0
            glUniform1i(glGetUniformLocation(_scene.drawnObjects[1].shaderId, "textureUnitDiffuse"), 0);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _scene.drawnObjects[1].textureId0);
           
            setTextureUnitsInShader(_scene.drawnObjects[1].shaderId);
            _scene.storedObjects[_scene.drawnObjects[1].objectId]->drawObject();*/

    Matrix4f orthoProj = orthoP(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
    Matrix4f inv;
    _scene.pCamera->getProjection().inverse(inv);

/*    //shadow
    // Bind shadow fbo
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFB.fbo);
        glDrawBuffers(shadowFB.outCount, shadowFB.drawBuffers);
        glViewport( 0, 0, WIDTH, HEIGHT);

        // Clear the front buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute light positions
        Vector3f lightPosition(0.0, 10.0, 0.0);
        Vector3f lightTarget( 0.0, 0.0, 0.0);
        Vector3f lightDirection = lightTarget - lightPosition;
        
        Vector3f lightUp(0.0, 1.0, 0.0);
        lightDirection.normalize();
        Vector3f lightColor(1.0, 1.0, 1.0);
        float lightIntensity = 1.0;

        // Build shadow matrices
        Matrix4f shadowProjection = perspective(60.f, 1.f, 1.0f, 1000.f);
        Matrix4f worldToLight = lookAt(lightPosition, lightTarget, lightUp);
        
        Matrix4f projectionLight = worldToLight * shadowProjection;     
        Matrix4f projectionLightBias = projectionLight * Matrix4f(
            0.5f, 0.f, 0.f, 0.5f,
            0.f, 0.5f, 0.f, 0.5f,
            0.f, 0.f, 0.5f, 0.5f,
            0.f, 0.f, 0.f, 1.f);

        Matrix4f objectToWorld = Matrix4f::identity();
        float shadowBias = 0.001f;
        float shadowSamples = 1.0;
        float shadowSampleSpread = 800.0;

        // Bind shadowgen shader
        glUseProgram(shaders[SHADOW]);
        // Upload uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaders[SHADOW], "Projection"), 1, 0, shadowProjection);
        glUniformMatrix4fv(glGetUniformLocation(shaders[SHADOW], "View"), 1, 0, worldToLight);
        glUniformMatrix4fv(glGetUniformLocation(shaders[SHADOW], "Object"), 1, 0, objectToWorld);
        glUniform1i(glGetUniformLocation(shaders[SHADOW], "Normal"), 0);

        // Bind normal to unit 1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.colorTexId[1]);  
        // Render vaos
        glCullFace(GL_FRONT);
        _scene.drawSimpleObjectsOfScene(shaders[SHADOW]);*/

    //Light

    /*glBindFramebuffer(GL_FRAMEBUFFER, lightFB.fbo);
        glDrawBuffers(lightFB.outCount, lightFB.drawBuffers);

        glViewport( 0, 0, WIDTH, HEIGHT);

        // Clears the window with current clearing color, clears also the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaders[LIGHT]);

        glUniformMatrix4fv(glGetUniformLocation(shaders[LIGHT], "projection"), 1, GL_FALSE, (const float*) orthoProj);
        glUniformMatrix4fv(glGetUniformLocation(shaders[LIGHT], "inverseProjection"), 1, GL_FALSE, (const float*) inv);
        glUniform3fv(glGetUniformLocation(shaders[LIGHT], "cameraPosition"), 1, (const float*) _scene.pCamera->getPosition());
        glUniform1i(glGetUniformLocation(shaders[LIGHT], "Diffuse"), 0);
        glUniform1i(glGetUniformLocation(shaders[LIGHT], "Normal"), 1);
        glUniform1i(glGetUniformLocation(shaders[LIGHT], "Depth"), 2);
       
        // Bind color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.colorTexId[0]); 
        // Bind normal to unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.colorTexId[1]);    
        // Bind depth to unit 2
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.depthTexId);  

        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        _scene.storedObjects[0]->drawObject();

        glDisable(GL_BLEND);*/
   
    //horizontal blur
    glBindFramebuffer(GL_FRAMEBUFFER, hblurFB.fbo);
        glDrawBuffers(hblurFB.outCount, hblurFB.drawBuffers);
        glViewport( 0, 0, WIDTH/2, HEIGHT/2);

        // Clears the window with current clearing color, clears also the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaders[BLUR]);

        glUniformMatrix4fv(glGetUniformLocation(shaders[BLUR], "projection"), 1, GL_FALSE, (const float*) orthoProj);
        glUniform2f(glGetUniformLocation(shaders[BLUR], "direction"), 1.0, 0.0);
        glUniform1f(glGetUniformLocation(shaders[BLUR], "sampleCount"), sampleBlur);
        glUniform1i(glGetUniformLocation(shaders[BLUR], "Color"), 0);
       
        // Bind color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.colorTexId[0]); 

        _scene.storedObjects[0]->drawObject();

    //Vertical blur
    glBindFramebuffer(GL_FRAMEBUFFER, vblurFB.fbo);
    glDrawBuffers(vblurFB.outCount, vblurFB.drawBuffers);
        glViewport( 0, 0, WIDTH/2, HEIGHT/2);

        // Clears the window with current clearing color, clears also the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaders[BLUR]);

        glUniformMatrix4fv(glGetUniformLocation(shaders[BLUR], "projection"), 1, GL_FALSE, (const float*) orthoProj);
        glUniform2f(glGetUniformLocation(shaders[BLUR], "direction"), 0.0, 1.0);
        glUniform1i(glGetUniformLocation(shaders[BLUR], "SampleCount"), sampleBlur);
        glUniform1i(glGetUniformLocation(shaders[BLUR], "Color"), 0);

        // Bind color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hblurFB.colorTexId[0]); 

        _scene.storedObjects[0]->drawObject();

    //coc 
    glBindFramebuffer(GL_FRAMEBUFFER, cocFB.fbo);
        glDrawBuffers(cocFB.outCount, cocFB.drawBuffers);
        glViewport( 0, 0, WIDTH, HEIGHT);

        // Clears the window with current clearing color, clears also the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaders[COC]);

    
        glUniformMatrix4fv(glGetUniformLocation(shaders[COC], "projection"), 1, GL_FALSE, (const float*) orthoProj);
        glUniformMatrix4fv(glGetUniformLocation(shaders[COC], "inverseProjection"), 1, GL_FALSE, (const float*) inv);
        glUniform3fv(glGetUniformLocation(shaders[COC], "focus"), 1, focus);
        glUniform1i(glGetUniformLocation(shaders[COC], "Depth"), 0);
           
        // Bind color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.depthTexId);  

        _scene.storedObjects[0]->drawObject();
    
    //dof 
    glBindFramebuffer(GL_FRAMEBUFFER, dofFB.fbo);
        glDrawBuffers(dofFB.outCount, dofFB.drawBuffers);
        glViewport( 0, 0, WIDTH, HEIGHT);

        // Clears the window with current clearing color, clears also the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaders[DOF]);

        glUniformMatrix4fv(glGetUniformLocation(shaders[DOF], "projection"), 1, GL_FALSE, (const float*) orthoProj);
        glUniform1i(glGetUniformLocation(shaders[DOF], "Color"), 0);
        glUniform1i(glGetUniformLocation(shaders[DOF], "Blur"), 1);
        glUniform1i(glGetUniformLocation(shaders[DOF], "BlurCoef"), 2);

        // Bind color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbufferFB.colorTexId[0]);  
        // Bind color texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, vblurFB.colorTexId[0]);   
        // Bind color texture
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cocFB.colorTexId[0]);

        _scene.storedObjects[0]->drawObject();

    //Gamma
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport( 0, 0, WIDTH, HEIGHT);

        // Clears the window with current clearing color, clears also the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaders[GAMMA]);

        glUniformMatrix4fv(glGetUniformLocation(shaders[GAMMA], "projection"), 1, GL_FALSE, (const float*) orthoProj);
        glUniform1f(glGetUniformLocation(shaders[GAMMA], "SobelCoef"), sobelCoef);
        glUniform1f(glGetUniformLocation(shaders[GAMMA], "Gamma"), gamma);
        glUniform1i(glGetUniformLocation(shaders[GAMMA], "Color"), 0);

        // Bind color texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dofFB.colorTexId[0]);

        _scene.storedObjects[0]->drawObject();

    // Draws GUI
    gui ? drawGUI() : NULL;

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
    if(glfwGetKey('G') == GLFW_PRESS) gui = (glfwGetKey('G') == GLFW_RELEASE) ? !gui : gui;
    if(glfwGetKey('H') == GLFW_PRESS) hideCursor('H');
}

// Builds the skybox
void GalaxyApp::setSkybox(size_t size) {
    Object &skyObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder skyBuilder = MeshBuilder();
    buildSquare(skyObject, size, skyBuilder);

    GLuint right =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(right, translation(Vector3f(size/2, 0., 0.)) * yRotation(-M_PI/2) );
    _scene.setDrawnObjectTextureID(right, 0, loadTexture( "textures/skybox/right.tga"));
    _scene.setDrawnObjectShaderID(right, shaders[SKY]);
     
    GLuint left =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(left, translation(Vector3f(-1. * size/2, 0., 0.)) * yRotation(M_PI/2) );
    _scene.setDrawnObjectTextureID(left, 0,loadTexture( "textures/skybox/left.tga"));
    _scene.setDrawnObjectShaderID(left, shaders[SKY]);

    GLuint front =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(front,translation(Vector3f(0., 0., size/2)) * yRotation(M_PI) );
    _scene.setDrawnObjectTextureID(front, 0, loadTexture( "textures/skybox/front.tga"));
    _scene.setDrawnObjectShaderID(front, shaders[SKY]);

    GLuint back =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(back, translation(Vector3f(0., 0., -1. * size/2)) );
    _scene.setDrawnObjectTextureID(back, 0, loadTexture( "textures/skybox/back.tga"));
    _scene.setDrawnObjectShaderID(back, shaders[SKY]);

    GLuint top =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(top,translation(Vector3f(0., size/2, 0.)) * xRotation(-M_PI/2) );
    _scene.setDrawnObjectTextureID(top, 0, loadTexture( "textures/skybox/top.tga"));
    _scene.setDrawnObjectShaderID(top, shaders[SKY]);

    GLuint bot =_scene.addObjectToDraw(skyObject.id);
    _scene.setDrawnObjectModel(bot,translation(Vector3f(0., -1. * size/2, 0.)) * xRotation(M_PI/2));
    _scene.setDrawnObjectTextureID(bot, 0, loadTexture( "textures/skybox/bot.tga"));
    _scene.setDrawnObjectShaderID(bot, shaders[SKY]);
}

// Builds the woman
void GalaxyApp::setWoman(float size) {
    GLuint diff = loadTexture( "textures/woman/womanDiffuse.tga");
    GLuint spec = loadTexture( "textures/woman/womanSpecular.tga");
    GLuint norm = loadTexture( "textures/woman/womanNormal.tga");
    GLuint disp = loadTexture( "textures/woman/womanDisp.tga");

    /*char* files[] = {"textures/skybox/right.tga", "textures/skybox/left.tga", "textures/skybox/front.tga", "textures/skybox/back.tga", "textures/skybox/top.tga","textures/skybox/bot.tga"} ;
    */ //GLuint tex = loadTextureCube(/*files*/);
    Object &womanObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder womanBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(womanObject, "res/objs/woman2.obj", false, false, womanBuilder);
    GLuint woman =_scene.addObjectToDraw(womanObject.id);
    _scene.setDrawnObjectModel(woman, translation(Vector3f( -3.2 , -7.7, -2.5)) * yRotation(-M_PI/8) * scale(Vector3f( size , size, size)));
    // _scene.setDrawnObjectShaderID(woman, shaders[CUBEMAP]);
    _scene.setDrawnObjectShaderID(woman, shaders[TEXTURE]);
    //_scene.setDrawnObjectTextureID(woman, 0, tex);
    _scene.setDrawnObjectTextureID(woman, 0, diff);
    _scene.setDrawnObjectTextureID(woman, 1, spec);
    _scene.setDrawnObjectTextureID(woman, 2, norm);
    _scene.setDrawnObjectTextureID(woman, 2, disp);

}

void GalaxyApp::setStone(float size) {
    GLuint diff = loadTexture( "textures/pedestral/pedestalDiffuse.tga");
    GLuint spec = loadTexture( "textures/pedestral/pedestalSpecular.tga");
    GLuint norm = loadTexture( "textures/pedestral/pedestalNormal.tga");
    GLuint disp = loadTexture( "textures/pedestral/pedestalDisp.tga");


    Object &stoneObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder stoneBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(stoneObject, "res/objs/stone.obj", false, false, stoneBuilder);

    GLuint stone =_scene.addObjectToDraw(stoneObject.id);
    _scene.setDrawnObjectModel(stone, translation(Vector3f( -2.9 , -9.5, -2.5)) * yRotation(-M_PI/8) * scale(Vector3f( size , size, size)));
    _scene.setDrawnObjectShaderID(stone, shaders[TEXTURE]);
    _scene.setDrawnObjectTextureID(stone, 0, diff);
    _scene.setDrawnObjectTextureID(stone, 1, spec);
    _scene.setDrawnObjectTextureID(stone, 2, norm);
    _scene.setDrawnObjectTextureID(stone, 2, disp);
}

void GalaxyApp::setSystem(){
   GLuint diff = loadTexture( "textures/sphere/mercureDiffuse.tga");
   GLuint spec = loadTexture( "textures/sphere/mercureSpecular.tga");
   GLuint norm = loadTexture( "textures/sphere/mercureNormal.tga");
   GLuint disp = loadTexture( "textures/sphere/mercureDisp.tga");

   GLuint diffCube = loadTexture( "textures/cube/cubeDiffuse.tga");
   GLuint specCube = loadTexture( "textures/cube/cubeSpecular.tga");
   GLuint normCube = loadTexture( "textures/cube/cubeNormal.tga");
   GLuint dispCube = loadTexture( "textures/cube/cubeDisp.tga");

    Object &cubeObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder cubeBuilder = MeshBuilder();
    buildSkybox(cubeObject, 0.2, cubeBuilder);

    
    Object &sphereObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder sphereBuilder = MeshBuilder();
    //buildCube(sphereObject, 1, sphereBuilder);
    buildObjectGeometryFromOBJ(sphereObject, "res/objs/sphere.obj", false, false, sphereBuilder);

    GLuint s = createSystem();

    GLuint instance =_scene.addObjectToDraw(cubeObject.id);
    _scene.setDrawnObjectShaderID(instance, shaders[TEXTURE]);
    _scene.setDrawnObjectTextureID(instance, 0, diffCube);
    _scene.setDrawnObjectTextureID(instance, 1, specCube);
    _scene.setDrawnObjectTextureID(instance, 2, normCube);
    _scene.setDrawnObjectTextureID(instance, 3, dispCube);  

    GLuint particle = systems[s].addPhysicToObject(instance);
    //systems[s].setPhysicObjectMass(particle, 0.2);

    for(int i = 0 ; i<40; ++i) {

        GLuint instance =_scene.addObjectToDraw(sphereObject.id);
        _scene.setDrawnObjectShaderID(instance, shaders[TEXTURE]);
        _scene.setDrawnObjectTextureID(instance, 0, diff);
        _scene.setDrawnObjectTextureID(instance, 1, spec);
        _scene.setDrawnObjectTextureID(instance, 2, norm);
        _scene.setDrawnObjectTextureID(instance, 3, disp);
        _scene.setDrawnObjectModel(instance, translation(Vector3f((0.5 - frand())/2., (0.5 - frand())/2., (0.5 - frand())/2.)));
        

        particle = systems[s].addPhysicToObject(instance);
        systems[s].setPhysicObjectPosition(particle, Vector3f((0.5 - frand())/2., (0.5 - frand())/2., (0.5 - frand())/2.));//(i/5., 0., 0.));
        systems[s].setPhysicObjectRotation(particle, Vector3f(frand()*6.18, frand()*6.18, frand()*6.18));
        systems[s].setPhysicObjectMass(particle, 0.1);
    }

    //systems[s].setAttraction(20);
    systems[s].setHookSpring(22, 0.7);   
    radiusSphere = true;
    systems[s].setCineticBrake(0.0008);
}

void GalaxyApp::setPills(){

    GLuint s = createSystem();
    for(int i = 0 ; i<30; ++i) {
        //GLuint instance =_scene.addObjectToDraw(sphereObject.id);
        GLuint particle = systems[s].addPhysicToObject(200 + i);
        systems[s].setPhysicObjectPosition(particle, Vector3f((0.2 - frand())*15., (0.5 - frand())*20., (0.8 - frand())*5.));//(i/5., 0., 0.));
    }
}

void GalaxyApp::setDragon(){

    GLuint diff = loadTexture( "textures/dragon/dragonScaleDiffuse.tga");
    GLuint diff2 = loadTexture( "textures/dragon/dragon.tga");
   GLuint spec = loadTexture( "textures/dragon/dragonScaleSpecular.tga");
    GLuint norm = loadTexture( "textures/dragon/dragonScaleNormal.tga");
    GLuint disp = loadTexture( "textures/dragon/dragonScaleDisp.tga");
    
    GLuint s = createSystem();

    Object &dragonObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder dragonBuilder = MeshBuilder();
    buildObjectGeometryFromOBJ(dragonObject, "res/objs/dragonHead.obj", false, false, dragonBuilder);

    GLuint instance =_scene.addObjectToDraw(dragonObject.id);
    _scene.setDrawnObjectShaderID(instance, shaders[TEXTURE]);
    _scene.setDrawnObjectTextureID(instance, 0, diff2);
    _scene.setDrawnObjectTextureID(instance, 1, spec);
    _scene.setDrawnObjectTextureID(instance, 2, norm);
    _scene.setDrawnObjectTextureID(instance, 3, disp);
    GLuint particle = systems[s].addPhysicToObject(instance);
    systems[s].setPhysicObjectPosition(particle, Vector3f(-1., 0., 0.));
    systems[s].setPhysicObjectMass(particle, 1);

    Object &sphereObject = _scene.createObject(GL_TRIANGLES);
    MeshBuilder sphereBuilder = MeshBuilder();
    //buildCube(sphereObject, 0.5, sphereBuilder);
    buildObjectGeometryFromOBJ(sphereObject, "res/objs/sphere.obj", false, false, sphereBuilder);

    for(int i = 0 ; i<20; ++i) {
        instance =_scene.addObjectToDraw(sphereObject.id);
        _scene.setDrawnObjectColor(instance, Color(cos(i * 50.),  sin(i * 50.),  sin(i * 50.)));
        _scene.setDrawnObjectShaderID(instance, shaders[TEXTURE]);
        _scene.setDrawnObjectTextureID(instance, 0, diff);
        _scene.setDrawnObjectTextureID(instance, 1, spec);
        _scene.setDrawnObjectTextureID(instance, 2, norm);
        _scene.setDrawnObjectTextureID(instance, 3, disp);
        particle = systems[s].addPhysicToObject(instance);
        systems[s].setPhysicObjectPosition(particle, Vector3f(-1 - i, 0., 0.));
        systems[s].setPhysicObjectMass(particle, 0.1);
    }

    systems[s].setAttraction(4);
}

GLuint GalaxyApp::createSystem() {
    const size_t size = systems.size();
    if (size >= 20)
        throw std::runtime_error("maximum number of system reached");
    systems.push_back(PhysicManager());
    return size;
}
