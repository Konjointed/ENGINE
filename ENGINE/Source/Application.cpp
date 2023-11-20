#include <iostream>

#include "Application.h"
#include "IncludeGL.h"

Application::Application() : window(nullptr), quit(false) {}
Application::~Application() {}

bool Application::Init() {
	int success;
	window = new Window(success);
	if (!success) return false;

	return true;
}

int Application::Run() {
	// Main Loop
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					std::cout << "Window Resized\n";

					int newWidth = event.window.data1;
					int newHeight = event.window.data2;

					glViewport(0, 0, newWidth, newHeight);
				}
				break;
			}
		}

		window->SwapBuffers();
	}

	Shutdown();

	return 1;
}

void Application::Shutdown() {
	delete window;
}