/* TODO:
	- Application class will create the scene and renderer objects
	- Create a new class where the scene is passed to this new class will be in charge of adding stuff to the scene
		- SceneManager/SceneBuilder/WorldBuilder/Simulation
	- Renderer will get a reference of the scene and use the data in the scene to render everything

	REFERENCE
	https://github.com/nlguillemot/dof/blob/master/viewer/simulation.cpp
	https://github.com/nlguillemot/dof/blob/master/viewer/scene.cpp
	https://github.com/nlguillemot/dof/blob/master/viewer/renderer.cpp
*/

#include <iostream>
#include <algorithm> 

#include <glm/ext/matrix_clip_space.hpp>
#include <stb_image.h>
#include <Shader.h>

#include "Editor/Editor.h"

#include "Application.h"
#include "IncludeGL.h"
#include "Window.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Animation.h"
#include "Animator.h"
#include "Texture.h"
#include "SceneElements.h"
#include "PostProcessor.h"
#include "Skybox.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "DiscordRichPresence.h"

Application::Application() : window(nullptr), quit(false) {}
Application::~Application() {}

bool Application::Init() {
	WindowProperties customSettings;
	customSettings.title = "KGE";
	customSettings.width = 1280;
	customSettings.height = 720;
	customSettings.fullscreen = true;
	customSettings.vsync = true;

	int success;
	window = new Window(success, customSettings);
	if (!success) return false;

	return true;
}

int Application::Run() {
	// Preload Resources
	ResourceManager::LoadShader("Resources/Shaders/Skybox.vert", "Resources/Shaders/Skybox.frag", "", "skybox");
	ResourceManager::LoadShader("Resources/Shaders/AnimModel.vert", "Resources/Shaders/AnimModel.frag", "", "model");
	ResourceManager::LoadShader("Resources/Shaders/Default.vert", "Resources/Shaders/Default.frag", "", "default");

	ResourceManager::LoadTexture("Resources/Textures/wood.png", "wood");
	ResourceManager::LoadTexture("Resources/Textures/brickwall.jpg", "brick");

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

	DiscordRichPresence drp;
	Editor editor(*window);
	Renderer renderer(*window);
	Camera camera;
	camera.name = "Camera";

	//====================TEMPORARY=================
	GameObject scenegraph; // root
	scenegraph.name = "Root";

	SceneElements scene(scenegraph);
	scene.lightPosition = { 80.0f, 90.0f, -77.0f };
	scene.camera = &camera; 
	scene.wireframe = false;
	GameObject::scene = &scene;

	Model playerModel = Model("Resources/Models/Maria/Maria J J Ong.dae", false);
	GameObject playerObject(playerModel);
	playerObject.name = "Player";
	//playerObject.transform.SetLocalPosition({ 10, 0, 0 });
	//const float scale = 0.75;
	//playerObject.transform.SetLocalScale({ scale, scale, scale });

	Model planeModel = Model::GeneratePlane();
	GameObject planeObject(planeModel);
	planeObject.name = "Plane";

	scenegraph.AddChild(std::move(planeObject));
	scenegraph.AddChild(std::move(playerObject));
	scenegraph.AddChild(std::move(camera));

	Animation idleAnimation("Resources/Animations/Idle.dae", &playerModel);
	Animator animator(&idleAnimation);

	Skybox skybox;
	//==============================================

	//  || Main Loop
	bool rightMouseButtonPressed = false;

	float t = 0.0f;
	float dt = 1 / 60.0f;

	float currentTime = SDL_GetTicks();
	float accumulator = 0.0f;

	while (!quit) {
		float newTime = SDL_GetTicks();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		// || Events/Input
		/* TODO:
			- Create a EventHandler/EventManager class with a HandleEvent method
			- This method will pass the event to it and then stuff will happen from their
		*/
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);

			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					int newWidth = event.window.data1;
					int newHeight = event.window.data2;
					glViewport(0, 0, newWidth, newHeight);
					//window->UpdateSize(newWidth, newHeight);
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
					camera.ProcessMouseMovement(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
				}
				break;
			}
		}

		while (accumulator >= dt) {
			// || Update
			drp.Update(); // discord rich presence
			camera.Update(dt);

			accumulator -= dt;
			t += dt;
		}

		// || Render
		renderer.Render(*window, camera, animator, skybox, scenegraph);

		editor.Draw(renderer.GetTextureColorBuffer());

		window->SwapBuffers();
	}

	Shutdown();

	return 1;
}

void Application::Shutdown() {
	delete window;
}