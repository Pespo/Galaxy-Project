#pragma once


#include "Scene.hpp"
#include "GLHeaders.hpp"

namespace stein {

class Application {
public :
	Application(size_t width, size_t height);
	virtual ~Application();

	void loop();

protected:
	Scene _scene;

	void resize(GLuint w, GLuint h);
    void setBackgroundColor(const Color &color);
    void setTitle(const char* title);
    void hideCursor();
    void printFPS();

    virtual void renderFrame();
    virtual void animate();

    virtual void eventDetection();

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

	// Mouse and keys management
	bool _bShowMouse; // True if mouse is seeable
	int _mouseXPos; // Mouse position - horizontal axis
	int _mouseYPos; // mouse position - vertical axis
	int _leftButton; // Mouse left button
	GLfloat _pressedMouseXPos; // Mouse position - updated only when left button down
    GLfloat _pressedMouseYPos; // mouse position - updated only when left button down
	int _mouseScroll; // scroll value (up : ++, down : --)
	int _zButton; // Z button
	int _qButton; // Q button
	int _sButton; // S button
	int _dButton; // D button

	// Frame management
	GLuint _frameCount; // Frame counter
	double _lastStartTime; // Time updated each frame
	double _lastSecondTime; // Time updated each second
	double _frameDuration; // Frame time duration
};
}