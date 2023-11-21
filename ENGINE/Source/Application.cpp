#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>
#include <Shader.h>

#include "Application.h"
#include "IncludeGL.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Buffers.h"

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
	std::vector<std::string> faces {
		"Resources/Textures/skybox/right.jpg",
		"Resources/Textures/skybox/left.jpg",
		"Resources/Textures/skybox/top.jpg",
		"Resources/Textures/skybox/bottom.jpg",
		"Resources/Textures/skybox/front.jpg",
		"Resources/Textures/skybox/back.jpg",
	};

	Shader basicShader("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag"); // simple texture
	Shader defaultShader("Resources/Shaders/Default.vert", "Resources/Shaders/Default.frag");
	Shader skyboxShader("Resources/Shaders/Skybox.vert", "Resources/Shaders/Skybox.frag");
	Shader screenShader("Resources/Shaders/Screen.vert", "Resources/Shaders/Screen.frag");
	Shader depthShader("Resources/Shaders/ShadowMappingDepth.vert", "Resources/Shaders/ShadowMappingDepth.frag");
	Shader debugDepthQuad("Resources/Shaders/DebugQuad.vert", "Resources/Shaders/DebugQuad.frag");

	unsigned int wood = Texture::FromFile("wood.png", "Resources/Textures");
	unsigned int brick = Texture::FromFile("brickwall.jpg", "Resources/Textures");
	unsigned int cubemapTexture = Texture::LoadCubemap(faces);

	Mesh cube = Mesh::GenerateCube();
	cube.SetPosition({ 0.0f, 5.0f, 0.0f });
	cube.SetScale({ 5.0f, 5.0f, 5.0f });

	Mesh plane = Mesh::GeneratePlane(); 
	plane.SetPosition({ 0.0f, -50.0f, 0.0f });
	plane.SetScale({ 50.0f, 1.0f, 50.0f });

	Mesh skybox = Mesh::GenerateCube();

	Mesh screenQuad = Mesh::GenerateQuad();

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	unsigned int depthMapFBO = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	unsigned int depthMap = FrameBuffer::CreateDepthTextureAttachment(1024, 1024);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//  || Main Loop
	int lastFrameTime = 0;
	while (!quit) {
		int currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		PollEvents();

		// || Update
		camera->Update(deltaTime);

		// || Render
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		float near_plane = -1000.0f, far_plane = 1000.0f;
		float orthoSize = 50.0f;
		glm::vec3 lightPosition(80.0f, 500.0f, -77.0f);
		glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shadow Pass
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);;

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		// RENDER SCENE
		glm::mat4 model = glm::mat4(1.0f);
		model = plane.GetModelMatrix();
		depthShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wood);
		plane.Draw();

		model = cube.GetModelMatrix();
		depthShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brick);
		cube.Draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Main Pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glm::mat4 model = glm::mat4(1.0f); // Moved to shadow pass
		defaultShader.use();
		defaultShader.setInt("diffuseTexture", 0);
		defaultShader.setInt("shadowMap", 1);
		defaultShader.setMat4("view", view);
		defaultShader.setMat4("projection", projection);
		defaultShader.setVec3("viewPos", camera->GetPosition());
		defaultShader.setVec3("lightPos", lightPosition);
		defaultShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		model = plane.GetModelMatrix();
		defaultShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wood);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		plane.Draw();

		model = cube.GetModelMatrix();
		defaultShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brick);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		cube.Draw();

		// Skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);
		view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		skybox.Draw();
		glDepthFunc(GL_LESS);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		screenQuad.Draw();

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

}

void Application::Update() {

}

void Application::Shutdown() {
	delete window;
	delete camera;
}