#pragma once
#include "common/GLCommon.hpp"
#include "common/ShaderGLSL.hpp"

#include "Scene.hpp"
#include "GLCommon.hpp"
#include "ShaderGLSL.hpp"

class Application {
public :
	Application(size_t width, size_t height, char* title);
	~Application();

	void initGlfw();
	void loop();
	void initTimers();
	void renderFrame();
	void eventDetection();
	void animate();

	Scene _scene;
	char* _title;
	double _time;

	// Window size
	size_t _width;
	size_t _height;

	// Mouse and keys management
	int _mousex;
	int _mousey;
	int _leftButton;
	int _zButton;
	int _qButton;
	int _sButton;
	int _dButton;

	// Frame management
	double _lasStartTime;
	double _frameDuration;
};
