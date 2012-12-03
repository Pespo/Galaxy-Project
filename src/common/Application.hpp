#pragma once

class Application {
public :
	Application(size_t width, size_t height, char* title);
	~Application();
	void loop();

	double t;
};
