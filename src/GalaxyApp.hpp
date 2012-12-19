#pragma once

#include "MoveableCamera.hpp"
#include "Physics/PhysicManager.hpp"
#include <stein/Application.hpp>
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

    GLuint shaders[3];
    //SoundFileRead soundfile;

    MoveableCamera* initCamera(const float size, Vector3f position);
	void initPhysics();
    void loadShaders();
    void initGUI();
    void drawGUI();

    GLuint createSystem();
    void renderFrame();
    void animate();
	void mouseMotionEvent();
	void mouseButtonEvent();
    void keyEvent();
    void buildSkybox(size_t size);
    void buildWoman(float size);
    void buildStone(float size);
    void setSystem();
    void setDragon();
    void setPills();
};