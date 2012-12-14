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
	
    GalaxyApp();
    ~GalaxyApp();
    void renderFrame();
    void animate();
	void mouseEvent();
    void keyEvent();
};