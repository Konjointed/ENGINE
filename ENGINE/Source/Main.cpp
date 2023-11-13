#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <Shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Mesh.h"
#include "Texture.h"
#include "Noise.h"
#include "Camera.h"

SDL_Window* window;
SDL_GLContext glContext;

std::vector<std::vector<float>> GenerateHeightMap(int width, int height, float scale, float maxHeight) {
	std::vector<std::vector<float>> heightMap(height, std::vector<float>(width));

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float noise = glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f; // Adjust scale and bias
			heightMap[y][x] = noise * maxHeight;
		}
	}

	return heightMap;
}

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
	Shader defaultShader("Resources/Shaders/Default.vert", "Resources/Shaders/Default.frag");
	Shader textureShader("Resources/Shaders/Texture.vert", "Resources/Shaders/Texture.frag");
	
	int mapWidth = 100;
	int mapHeight = 100;
	float mapScale = 25.0f; // zoom in or out

	std::vector<std::vector<float>> noiseMap = Noise::GenerateNoiseMap(mapWidth, mapHeight, mapScale);
	Texture texture(noiseMap);
	Mesh plane = Mesh::GeneratePlane();

	Camera camera;

	int lastFrameTime = 0;
	bool rightMouseButtonPressed = false;
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		int currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;
		//std::cout << deltaTime << "\n";

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			//case SDL_KEYDOWN:
				//break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_RIGHT) {
					rightMouseButtonPressed = true;
					SDL_SetRelativeMouseMode(SDL_TRUE); 
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_RIGHT) {
					rightMouseButtonPressed = false;
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				break;
			//case SDL_MOUSEWHEEL:
				//break;
			case SDL_MOUSEMOTION:
				if (rightMouseButtonPressed) {
					camera.ProcessMouseMovement(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Update
		camera.Update(deltaTime);

		// Render
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();
		glm::mat4 model = plane.GetModelMatrix();

		textureShader.use();
		textureShader.setMat4("projection", projection);
		textureShader.setMat4("view", view);
		textureShader.setMat4("model", model);

		texture.Bind();
		plane.Draw();
		Texture::Unbind();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}

int main(int argc, char* argv[]) {
	if (!Init("Engine", 1280, 720, false))
		return -1;

	return Run();
}