#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

SDL_Window* window;
SDL_GLContext glContext;

bool Init(const char* windowTitle, int windowWidth, int windowHeight, bool fullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL error on initialization: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create sdl window
	const Uint32 windowFlags = (SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_RESIZABLE : 0));
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowFlags);
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

	/* Disable vsync
	if (SDL_GL_SetSwapInterval(0) < 0) {
		std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
	}
	*/

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

int Run() {
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if (!Init("Engine", 1280, 720, false))
		return -1;

	return Run();
}