#ifndef APPLICATION_H 
#define APPLICATION_H

#include "Window.h"

class Application {
public:
	Application();
	~Application();

	bool Init();
	int Run();
	void Shutdown();
private:
	Window* window;
	bool quit;
};

#endif 