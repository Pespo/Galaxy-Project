#pragma once
#include "GLCommon.hpp"
#include "ShaderGLSL.hpp"

class Application {

public :
	Application(size_t width, size_t height, char* title);
	~Application();

	GLuint loadTexture(const char* fileName, int comp);
	void loadShader(ShaderGLSL & shader,  const char * path);

};
