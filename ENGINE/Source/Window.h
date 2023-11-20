#pragma once

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
		fullscreen(false),
		vsync(false)
	{}
};

class Window {
public:
	Window();
	bool Init(WindowSettings settings = WindowSettings());
	void Update();
	SDL_Window* GetWindow();
	SDL_GLContext GetContext();
	void SetVSync(bool enabled);
	~Window();
private:
	SDL_Window* window;
	SDL_GLContext glContext;
};
