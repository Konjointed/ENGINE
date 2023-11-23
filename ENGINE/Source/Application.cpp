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
	camera = new Camera;

	return true;
}

int Application::Run() {
	// Setup the scene
	SceneElements scene;
	scene.lightPosition = { 80.0f, 500.0f, -77.0f };
	scene.camera = camera;
	scene.wireframe = false;

	// Every object now has access to the scene (granted they inherit from DrawableObject)
	DrawableObject::scene = &scene;

	Shader basicShader("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag"); // simple texture
	Shader defaultShader("Resources/Shaders/Default.vert", "Resources/Shaders/Default.frag");
	Shader skyboxShader("Resources/Shaders/Skybox.vert", "Resources/Shaders/Skybox.frag");
	Shader screenShader("Resources/Shaders/Screen.vert", "Resources/Shaders/Screen.frag");
	Shader depthShader("Resources/Shaders/ShadowMappingDepth.vert", "Resources/Shaders/ShadowMappingDepth.frag");
	Shader debugDepthQuad("Resources/Shaders/DebugQuad.vert", "Resources/Shaders/DebugQuad.frag");
	Shader ourShader("Resources/Shaders/AnimModel.vert", "Resources/Shaders/AnimModel.frag");

	unsigned int wood = Texture::FromFile("wood.png", "Resources/Textures");
	unsigned int brick = Texture::FromFile("brickwall.jpg", "Resources/Textures");

	Mesh plane = Mesh::GeneratePlane();
	plane.SetPosition({ 0.0f, -1.0f, 0.0f });
	plane.SetScale({ 5.0f, 1.0f, 5.0f });

	Mesh cube = Mesh::GenerateCube();
	cube.SetPosition({ 0.0f, 5.0f, 0.0f });
	cube.SetScale({ 1.0f, 1.0f, 1.0f });

	PostProcessor postProcessor;
	Skybox skybox;

	stbi_set_flip_vertically_on_load(true);
	Model ourModel("Resources/Models/Maria/Maria J J Ong.dae", false);
	Animation idleAnimation("Resources/Animations/Idle.dae", &ourModel);
	Animator animator(&idleAnimation);

	unsigned int framebuffer = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int textureColorbuffer = FrameBuffer::CreateTextureAttachment(1280, 720);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	unsigned int rbo = FrameBuffer::CreateRenderBufferAttachment(1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int depthMapFBO = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	unsigned int depthMap = FrameBuffer::CreateDepthTextureAttachment(1024, 1024);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//  || Main Loop
	int lastFrameTime = 0;
	while (!quit) {
		int currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		PollEvents();
	
		// || Update
		scene.camera->Update(deltaTime);
		animator.UpdateAnimation(deltaTime);

		// || Render
		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = scene.camera->GetViewMatrix();
		glm::mat4 projection = scene.camera->GetProjectionMatrix();

		float near_plane = -1000.0f, far_plane = 1000.0f;
		float orthoSize = 50.0f;
		glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(scene.lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shadow Pass
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		depthShader.setMat4("view", view);
		depthShader.setMat4("projection", projection);

		// Render cube
		model = cube.GetModelMatrix();
		depthShader.setMat4("model", model);
		cube.Draw(depthShader);

		// Render plane
		model = plane.GetModelMatrix();
		depthShader.setMat4("model", model);
		plane.Draw(depthShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Main Pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		defaultShader.use();
		defaultShader.setMat4("view", view);
		defaultShader.setMat4("projection", projection);
		defaultShader.setVec3("viewPos", scene.camera->GetPosition());
		defaultShader.setVec3("lightPos", scene.lightPosition);
		defaultShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// Render cube
		model = cube.GetModelMatrix();
		defaultShader.setMat4("model", model);
		cube.Draw(defaultShader, depthMap);

		// Render plane
		model = plane.GetModelMatrix();
		defaultShader.setMat4("model", model);
		plane.Draw(defaultShader, depthMap);

		// Render model
		ourShader.use();

		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		//glm::mat4 model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		skyboxShader.use();
		view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		skybox.Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Post processing pass
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		static float accumulatedTime = 0.0f;
		accumulatedTime += deltaTime;
		screenShader.use();
		screenShader.setFloat("time", accumulatedTime);
		screenShader.setInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		postProcessor.Draw();

		gui->Draw();

		window->SwapBuffers();
	}

	Shutdown();

	return 1;
}

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