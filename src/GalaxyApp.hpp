#pragma once

#include "MoveableCamera.hpp"
#include "Physics/PhysicManager.hpp"
#include <stein/Application.hpp>
#include <stein/FramebufferGL.hpp>
#include <stein/math/Vector3f.hpp>

class GalaxyApp : public stein::Application {

public:
	const static size_t WIDTH = 800;
	const static size_t HEIGHT = 800;

	int _exMouseXPos; // Keeps the X mouse position of the previous image
	int _exMouseYPos; // Keeps the Y mouse position of the previous image
	bool _leftButtonPressed; // Keeps the left button of the mouse value (PRESS or RELEASE)
	float _bias;
	
    GalaxyApp();
    ~GalaxyApp();

    std::vector<PhysicManager> systems;

    FramebufferGL gbufferFB; 
    FramebufferGL shadowFB; 
    FramebufferGL lightFB;   
    FramebufferGL hblurFB;
    FramebufferGL vblurFB;
    FramebufferGL cocFB;
    FramebufferGL dofFB;
    //FramebufferGL gammaFB;

    float forceDragon;
    bool radiusSphere;
    GLuint shaders[11];
    GLuint finalScreen;
    //SoundFileRead soundfile;

    MoveableCamera* initCamera(const float size, Vector3f position);
	void initPhysics();
    void loadShaders();
    void initFramebuffer();
    void initGUI();
    void drawGUI();

    GLuint createSystem();
    void renderFrame();
    void animate();
	void mouseMotionEvent();
	void mouseButtonEvent();
    void keyEvent();
    void setSkybox(size_t size);
    void setWoman(float size);
    void setStone(float size);
    void setCube();
    void setSystem();
    void setSystem2();
    void setDragon();
    void setPills();
};