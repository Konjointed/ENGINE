#ifndef WINDOW_H 
#define WINDOW_H 

#include <string>

#include "IncludeGL.h"

struct WindowSettings {
	std::string title;
	int width;
	int height;
	bool fullscreen;
	bool vsync;

	WindowSettings() :
		title("Engine"),
		width(1280), height(720),
		fullscreen(true),
		vsync(false)
	{}
};

class Window {
public:
	Window(int& success, WindowSettings settings = WindowSettings());
	~Window();

	void SwapBuffers();
	SDL_Window* GetWindow();
	SDL_GLContext GetContext();
private:
	SDL_Window* window;
	SDL_GLContext glContext;
};

#endif 
