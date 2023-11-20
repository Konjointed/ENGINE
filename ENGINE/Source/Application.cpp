#include <iostream>

#include <Shader.h>

#include "Application.h"
#include "IncludeGL.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"

Application::Application() : window(nullptr), quit(false) {}
Application::~Application() {}

bool Application::Init() {
	/*
	WindowSettings customSettings;
	customSettings.title = "My Custom Window";
	customSettings.width = 800;
	customSettings.height = 600;
	customSettings.fullscreen = false;
	customSettings.vsync = true;
	*/

	int success;
	window = new Window(success);
	if (!success) return false;

	camera = new Camera;

	return true;
}

int Application::Run() {
	Shader shader("Resources/Shaders/Default.vert", "Resources/Shaders/Default.frag");

	Mesh mesh = Mesh::GenerateCube();

	//  Main Loop 
	int lastFrameTime = 0;
	while (!quit) {
		int currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		PollEvents();

		Update();
			camera->Update(deltaTime);

		Render();
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera->GetViewMatrix();
			glm::mat4 projection = camera->GetProjectionMatrix();

			shader.use();
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			shader.setMat4("model", model);

			mesh.Draw();

		window->SwapBuffers();
	}

	Shutdown();

	return 1;
}

void Application::PollEvents() {
	static bool rightMouseButtonPressed = false;
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
		case SDL_MOUSEMOTION:
			if (rightMouseButtonPressed) {
				camera->ProcessMouseMovement(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
			}
			break;
		}
	}
}

void Application::Render() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void Application::Update() {

}

void Application::Shutdown() {
	delete window;
	delete camera;
}