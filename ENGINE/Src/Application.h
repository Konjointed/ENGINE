#ifndef APPLICATION_H 
#define APPLICATION_H

#include <vector>

class Editor;
class Window;
class Renderer;
class Camera;

class Application {
public:
	Application();
	~Application();

	bool Init();
	int Run();
	void Shutdown();
private:
	bool quit;
	Window* window;
};

#endif 