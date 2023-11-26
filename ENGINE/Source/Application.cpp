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

#include "Editor/GUI.h"
#include "Editor/Console.h"
#include "Editor/Scenegraph.h"

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
#include "Buffers.h"
#include "SceneElements.h"
#include "PostProcessor.h"
#include "Skybox.h"
//#include "DrawableObject.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "LuaEnvironment.h"

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
	LuaEnvironment luaenv;
	luaenv.DoFile("Resources/Scripts/ILoveLua.lua");

	unsigned int framebuffer = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int textureColorbuffer = FrameBuffer::CreateTextureAttachment(1280, 720);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	unsigned int rbo = FrameBuffer::CreateRenderBufferAttachment(1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	Console console(*gui);
	SceneGraph sceneGraph(*gui); 

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	GameObject scenegraph; // root
	scenegraph.name = "Root";

	SceneElements scene(scenegraph);
	scene.lightPosition = { 80.0f, 90.0f, -77.0f };
	scene.camera = camera; 
	scene.wireframe = false;
	GameObject::scene = &scene;

	Skybox skybox;

	Model playerModel = Model("Resources/Models/Maria/Maria J J Ong.dae", false);
	GameObject playerObject(playerModel);
	playerObject.name = "Player";
	playerObject.transform.SetLocalPosition({ 10, 0, 0 });
	const float scale = 0.75;
	playerObject.transform.SetLocalScale({ scale, scale, scale });

	scenegraph.AddChild(std::move(playerObject));

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
		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

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
		scenegraph.UpdateSelfAndChild();
		scenegraph.DrawSelfAndChild(ResourceManager::GetShader("model"));

		skybox.Draw(view, projection);

		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		gui->DrawWindows();

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