#pragma once

#include <stein/Application.hpp>

class GalaxyApp : public stein::Application {
public:
	//const static size_t WIDTH = 800;
	//const static size_t HEIGHT = 800;

	int exMouseXPos;
	int exMouseYPos;

    GalaxyApp();
    ~GalaxyApp();
    void renderFrame();
    void animate();
	void mouseEvent();
    void keyEvent();
};