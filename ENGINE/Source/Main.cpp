#include <iostream>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Shader.h>

#include "Mesh.h"
#include "Texture.h"
#include "Noise.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"

SDL_Window* window;
SDL_GLContext glContext;

bool ImGuiInit() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init();

	return true;
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

	ImGuiInit();

	return true;
}


int Run() {
	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 720;
	int SHADOW_WIDTH = 1024;
	int SHADOW_HEIGHT = 1024;

	glEnable(GL_DEPTH_TEST);

	// Build and compile shaders
	//Shader defaultShader("Resources/Shaders/Default.vert", "Resources/Shaders/Default.frag");
	//Shader colorShader("Resources/Shaders/Color.vert", "Resources/Shaders/Color.frag");
	//Shader textureShader("Resources/Shaders/Texture.vert", "Resources/Shaders/Texture.frag");
	//Shader lightingShader("Resources/Shaders/BasicLighting.vert", "Resources/Shaders/BasicLighting.frag");
	//Shader lightingShader("Resources/Shaders/LightingMap.vert", "Resources/Shaders/LightingMap.frag");
	//Shader lightingShader("Resources/Shaders/LightCasters.vert", "Resources/Shaders/LightCasters.frag");
	//Shader screenShader("Resources/Shaders/FramebufferScreen.vert", "Resources/Shaders/FramebufferScreen.frag");
	Shader shader("Resources/Shaders/ShadowMapping.vert", "Resources/Shaders/ShadowMapping.frag");
	Shader simpleDepthShader("Resources/Shaders/ShadowMappingDepth.vert", "Resources/Shaders/ShadowMappingDepth.frag");

	/*
	Mesh lamp = Mesh::GenerateCube();
	lamp.SetScale({ 0.2f, 0.2f, 0.2f });
	lamp.SetPosition({ 1.2f, 2.0f, 2.0f });

	Mesh cube = Mesh::GenerateCube();
	cube.SetScale({ 1.0f, 1.0f, 1.0f });
	cube.SetPosition({ 0.0f, 1.0f, 0.0f });
	*/

	Mesh plane = Mesh::GeneratePlane();
	plane.SetScale({ 50.0f, 50.0f, 50.0f });

	//Mesh quad = Mesh::GenerateQuad();

	// Load textures
	//Texture brickTexture("Resources/Textures/brickwall.jpg");
	Texture woodTexture("Resources/Textures/wood.png");
	//Texture containerDiffuseTexture("Resources/Textures/container.png");
	//Texture containerSpecularTexture("Resources/Textures/container_specular.png");

	// Shader configuration
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);

	//screenShader.use();
	//screenShader.setInt("screenTexture", 0);

	//lightingShader.use();
	//lightingShader.setInt("material.diffuse", 0);
	//lightingShader.setInt("material.specular", 1);

	//FBO framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	FBO depthMapFramebuffer(SHADOW_WIDTH, SHADOW_HEIGHT, true);

	Camera camera;

	/*
	int mapWidth = 100;
	int mapHeight = 100;
	float mapScale = 25.0f;

	int octaves = 4;
	float persistance = 0.5f;
	float lacunarity = 2.0f;

	int seed = 0;
	float offsetX = 0.0f, offsetY = 0.0f;

	d::vector<std::vector<float>> heightMap = Noise::GenerateNoiseMap(mapWidth, mapHeight, mapScale, seed, octaves, persistance, lacunarity, {offsetX, offsetY});
	//Texture noiseMap(heightMap);
	Mesh terrain = Mesh::GenerateTerrain(heightMap, 0.2f);
	*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
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
			ImGui_ImplSDL2_ProcessEvent(&event);

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();

		/*
		ImGui::Begin("Noise Controls");
		bool parametersChanged = ImGui::SliderInt("Width", &mapWidth, 1, 200) ||
			ImGui::SliderInt("Height", &mapHeight, 1, 200) ||
			ImGui::SliderFloat("Scale", &mapScale, 0.1f, 100.0f) ||
			ImGui::SliderInt("Octaves", &octaves, 1, 8) ||
			ImGui::SliderFloat("Persistence", &persistance, 0.0f, 1.0f) ||
			ImGui::SliderFloat("Lacunarity", &lacunarity, 0.0f, 4.0f) ||
			ImGui::InputInt("Seed", &seed) ||
			ImGui::SliderFloat("Offset X", &offsetX, -100.0f, 100.0f) ||
			ImGui::SliderFloat("Offset Y", &offsetY, -100.0f, 100.0f);
		ImGui::End();

		if (parametersChanged) {
			std::vector<std::vector<float>> heightMap = Noise::GenerateNoiseMap(mapWidth, mapHeight, mapScale, seed, octaves, persistance, lacunarity, { offsetX, offsetY });
			noiseMap.Update(heightMap);
		}
		*/

		glm::vec3 lightDir = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));

		// UPDATE
		camera.Update(deltaTime);

		// RENDER
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. Render depth of scene to texture (from light's perspetive)
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// Render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		depthMapFramebuffer.Bind();
			glClear(GL_DEPTH_BUFFER_BIT);
			// RENDER SCENE
			glm::mat4 planeModel = plane.GetModelMatrix();
			simpleDepthShader.setMat4("model", planeModel);
			woodTexture.Bind(0);
			plane.Draw();
			Texture::Unbind();
		depthMapFramebuffer.UnBind();

		// Reset viewport
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. Render scene as normal using the generated depth/shadow map
		shader.use();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// set light uniforms
		shader.setVec3("viewPos", camera.GetPosition());
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		// RENDER SCENE
		simpleDepthShader.setMat4("model", planeModel);
		woodTexture.Bind(0);
		// Bind depth map texture
		plane.Draw();

		/*
		// First pass
		glViewport(0, 0, 1024, 1024);
		depthMapFramebuffer.Bind();
			glClear(GL_DEPTH_BUFFER_BIT);
		depthMapFramebuffer.UnBind();

		glViewport(0, 0, 1280, 720);


		// Cube
		lightingShader.use();
		lightingShader.setVec3("light.direction", lightDir);
		lightingShader.setVec3("viewPos", camera.GetPosition());
		// light properties
		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		// material properties
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		glm::mat4 cubeModel = cube.GetModelMatrix();
		lightingShader.setMat4("model", cubeModel);
		containerDiffuseTexture.Bind(0);
		containerSpecularTexture.Bind(1);
		cube.Draw();
		Texture::Unbind(0);
		Texture::Unbind(1);

		textureShader.use();
		textureShader.setVec2("textureScale", 50.0f, 50.0f);
		textureShader.setMat4("projection", projection);
		textureShader.setMat4("view", view);
		glm::mat4 planeModel = plane.GetModelMatrix();
		textureShader.setMat4("model", planeModel);
		woodTexture.Bind(0);
		plane.Draw();

		Texture::Unbind();

		// Second pass
		framebuffer.UnBind();
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use(); // post processing shader goes here
		glBindTexture(GL_TEXTURE_2D, framebuffer.texture);
		quad.Draw();
		*/

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		SDL_GL_SwapWindow(window);
	}

	// Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

int main(int argc, char* argv[]) {
	if (!Init("Engine", 1280, 720, false))
		return -1;

	return Run();
}