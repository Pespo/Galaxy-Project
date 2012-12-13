#pragma once

#include <stein/Application.hpp>

class GalaxyApp : public stein::Application {
public:
	//const static size_t WIDTH = 800;
	//const static size_t HEIGHT = 800;

	int _exMouseXPos;
	int _exMouseYPos;

    GalaxyApp();
    ~GalaxyApp();
    void GalaxyApp::renderFrame();
    void animate();
    void GalaxyApp::eventDetection();
};