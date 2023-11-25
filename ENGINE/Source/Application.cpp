/*
TODO:
- Make a scene class that will (maybe) hold the camera and lighting stuff
- Add imgui panels for controlling the scene, lighting, camera, etc (DONE)
- Create a logger (maybe) 
- Move console panel code in GUI class to its own class
*/

#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>
#include <stb_image.h>
#include <Shader.h>

#include "Application.h"
#include "IncludeGL.h"
#include "GUI.h"
#include "Window.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Animation.h"
#include "Animator.h"
#include "Texture.h"
#include "Buffers.h"
#include "SceneElements.h"
#include "PostProcessor.h"
#include "Skybox.h"
#include "DrawableObject.h"
#include "GameObject.h"
#include "ResourceManager.h"

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

	// Made a window class and doing absolutely nothing with it 💀
	int success;
	window = new Window(success);
	if (!success) return false;

	gui = new GUI(*window);
	renderer = new Renderer;
	camera = new Camera;

	return true;
}

int Application::Run() {
	ResourceManager::LoadShader("Resources/Shaders/Skybox.vert", "Resources/Shaders/Skybox.frag", "", "skybox");
	ResourceManager::LoadShader("Resources/Shaders/AnimModel.vert", "Resources/Shaders/AnimModel.frag", "", "model");

	ResourceManager::LoadCubemap({
		"Resources/Textures/skybox/right.jpg",
		"Resources/Textures/skybox/left.jpg",
		"Resources/Textures/skybox/top.jpg",
		"Resources/Textures/skybox/bottom.jpg",
		"Resources/Textures/skybox/front.jpg",
		"Resources/Textures/skybox/back.jpg",
		}, "skybox");

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	Skybox skybox;

	Model playerModel = Model("Resources/Models/Maria/Maria J J Ong.dae", false);
	GameObject playerObject(playerModel);
	playerObject.transform.SetLocalPosition({ 10, 0, 0 });
	const float scale = 0.75;
	playerObject.transform.SetLocalScale({ scale, scale, scale });

	{
		GameObject* lastEntity = &playerObject;

		for (unsigned int i = 0; i < 10; ++i)
		{
			lastEntity->AddChild(playerModel);
			lastEntity = lastEntity->children.back().get();

			//Set transform values
			lastEntity->transform.SetLocalPosition({ 10, 0, 0 });
			lastEntity->transform.SetLocalScale({ scale, scale, scale });
		}
	}
	playerObject.UpdateSelfAndChild();


	Animation idleAnimation("Resources/Animations/Idle.dae", &playerModel);
	Animator animator(&idleAnimation);

	//  || Main Loop
	static int lastFrameTime = 0;
	while (!quit) {
		int currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		// || Events
		PollEvents();
	
		// || Update
		camera->Update(deltaTime);

		// || Render
		// Reset
		//glDepthFunc(GL_LESS); // For skybox
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ResourceManager::GetShader("model").Use();

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();
		ResourceManager::GetShader("model").SetMatrix4("projection", projection);
		ResourceManager::GetShader("model").SetMatrix4("view", view);

		auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i) {
			std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
			ResourceManager::GetShader("model").SetMatrix4(name.c_str(), transforms[i]);
		}

		// Draw scene graph
		GameObject* lastObject = &playerObject;
		while (lastObject->children.size()) {
			ResourceManager::GetShader("model").SetMatrix4("model", lastObject->transform.GetModelMatrix());
			lastObject->model->Draw(ResourceManager::GetShader("model"));
			lastObject = lastObject->children.back().get();
		}
		//playerObject.transform.setLocalRotation({ 0.f, playerObject.transform.getLocalRotation().y + 20 * deltaTime, 0.f });
		playerObject.UpdateSelfAndChild();

		skybox.Draw(view, projection);

		gui->Draw();

		window->SwapBuffers();
	}

	Shutdown();

	return 1;
}

/* TODO:
* - Create a EventHandler/EventManager class with a HandleEvent method
* - This method will pass the event to it and then stuff will happen from their
*/
void Application::PollEvents() {
	static bool rightMouseButtonPressed = false;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

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

}

void Application::Update() {

}

void Application::Shutdown() {
	delete gui;
	delete window;
	delete camera;
}