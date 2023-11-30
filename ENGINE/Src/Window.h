#ifndef WINDOW_H 
#define WINDOW_H 

#include <string>

#include "IncludeGL.h"

struct WindowProperties {
	std::string title;
	int width;
	int height;
	bool fullscreen;
	bool vsync;

	WindowProperties() :
		title("Application"),
		width(1280), height(720),
		fullscreen(true),
		vsync(false)
	{}
};

class Window {
public:
	Window(int& success, WindowProperties properties = WindowProperties());
	~Window();

	void SwapBuffers();
	//void UpdateSize(int width, int height);

	int GetWidth();
	int GetHeight();
	SDL_Window* GetWindow();
	SDL_GLContext GetContext();
private:
	SDL_Window* window;
	SDL_GLContext glContext;
	WindowProperties properties;
};

#endif 
