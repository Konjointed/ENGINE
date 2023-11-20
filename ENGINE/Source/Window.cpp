#include "Window.h"

#include <iostream>

#include "IncludeGL.h"

Window::Window() {}

bool Window::Init(WindowSettings settings) {
	// Create sdl window
	const Uint32 windowFlags = (SDL_WINDOW_OPENGL | (settings.fullscreen ? SDL_WINDOW_RESIZABLE : 0));
	window = SDL_CreateWindow(settings.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.width, settings.height, windowFlags);
	if (!window) {
		std::cout << "SDL error on create window: " << SDL_GetError() << "\n";
		return false;
	}

	// Create opengl context
	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cout << "SDL GL error on create context: " << SDL_GetError() << "\n";
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	// Load opengl functions
	if (!gladLoadGL()) {
		std::cout << "GLAD Initialization Error\n";
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	return true;
}

void Window::Update() {

}

SDL_Window* Window::GetWindow() {
	return window;
}

SDL_GLContext Window::GetContext() {
	return glContext;
}

void Window::SetVSync(bool enabled) {
	if (SDL_GL_SetSwapInterval(enabled ? 1 : 0) < 0) {
		std::cerr << "Error setting VSync: " << SDL_GetError() << std::endl;
	}
}

Window::~Window() {}