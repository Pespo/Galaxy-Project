#pragma once


#include "Scene.hpp"
#include "GLHeaders.hpp"
#include "GL/glfw.h"

namespace stein {

class Application {
public :
	Application(size_t width, size_t height);
	virtual ~Application();

	void loop();

protected:
	Scene* _pScene;

	// Mouse and keys management
	bool _bShowMouse; // True if mouse is seeable
	int _mouseXPos; // Mouse position - horizontal axis
	int _mouseYPos; // mouse position - vertical axis	
	GLfloat _pressedMouseXPos; // Mouse position - updated only when left button down
    GLfloat _pressedMouseYPos; // mouse position - updated only when left button down
	int _mouseScroll; // scroll value (up : ++, down : --)

	void resize(GLuint w, GLuint h);
    void setBackgroundColor(const Color &color);
    void setTitle(const char* title);
    void hideCursor(char key);
    void printFPS();

    virtual void mouseEvent();
    virtual void keyEvent();
    virtual void animate();
    void renderFrame();

    size_t frameCount() const;

private:
	void initGlfw();

	// Window
	size_t _windowedWidth; // Window dimensions when not fullscreen - horizontal axis
    size_t _windowedHeight; // Window dimensions when not fullscreen - vertical axis
    size_t _fullScreenWidth; // Screen dimensions - horizontal axis
    size_t _fullScreenHeight; // Screen dimensions - vertical axis
    size_t _width; // Window actual dimensions - horizontal axis
    size_t _height; // Window actual dimensions - vertical axis

	// Frame management
	GLuint _frameCount; // Frame counter
	double _lastStartTime; // Time updated each frame
	double _lastSecondTime; // Time updated each second
	double _frameDuration; // Frame time duration
};
}