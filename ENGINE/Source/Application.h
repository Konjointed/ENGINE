#ifndef APPLICATION_H 
#define APPLICATION_H

#include <vector>

class GUI;
class Window;
class Camera;

class Application {
public:
	Application();
	~Application();

	bool Init();
	int Run();
	void Shutdown();

	void PollEvents();
	void Render();
	void Update();
private:
	bool quit;
	GUI* gui;
	Window* window;
	Camera* camera;
};

#endif 