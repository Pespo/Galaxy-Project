#pragma once

#include "Physics/PhysicManager.hpp"
#include <stein/Application.hpp>

class GalaxyApp : public stein::Application {
public:
	const static size_t WIDTH = 800;
	const static size_t HEIGHT = 800;

	PhysicManager physicManager;

	int _exMouseXPos; // Keeps the X mouse position of the previous image
	int _exMouseYPos; // Keeps the Y mouse position of the previous image
	bool _leftButtonPressed; // Keeps the left button of the mouse value (PRESS or RELEASE)
	float _bias;
	
    GalaxyApp();
    ~GalaxyApp();
    void initGUI();
    void drawGUI();
    void renderFrame();
    void animate();
	void mouseMotionEvent();
	void mouseButtonEvent();
    void keyEvent();
};