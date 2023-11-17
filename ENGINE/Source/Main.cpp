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
#include "Noise.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "ResourceManager.h"

SDL_Window* window;
SDL_GLContext glContext;

std::vector<Mesh*> objects;

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

void RenderScene(Shader& shader) {
	for (Mesh* obj : objects) {
		obj->Draw(shader);
	}
}

void Update() {}

void Render() {}

int Run() {
	glEnable(GL_DEPTH_TEST);

	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 720;
	int SHADOW_WIDTH = 1024;
	int SHADOW_HEIGHT = 1024;

	Camera camera;

	// Build and compile shaders
	Shader shader("Resources/Shaders/Shaders.vert", "Resources/Shaders/Shaders.frag");
	Shader screenShader("Resources/Shaders/FramebufferScreen.vert", "Resources/Shaders/FramebufferScreen.frag");
	Shader grayScaleShader("Resources/Shaders/Grayscale.vert", "Resources/Shaders/Grayscale.frag");
	Shader shadowMapShader("Resources/Shaders/ShadowMapping.vert", "Resources/Shaders/ShadowMapping.frag");
	Shader simpleDepthShader("Resources/Shaders/ShadowMappingDepth.vert", "Resources/Shaders/ShadowMappingDepth.frag");
	Shader debugDepthQuad("Resources/Shaders/DebugQuadDepth.vert", "Resources/Shaders/DebugQuadDepth.frag");

	// Load textures
	Texture* floorTexture = ResourceManager::LoadTextureFromFile("Resources/Textures/wood.png");
	Texture* cubeTexture = ResourceManager::LoadTextureFromFile("Resources/Textures/brickwall.jpg");

	// shader configuration
	shadowMapShader.use();
	shadowMapShader.setInt("diffuseTexture", 0);
	shadowMapShader.setInt("shadowMap", 1);

	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	Mesh* cube = new Mesh(Mesh::GenerateCube());
	cube->AddTexture(cubeTexture);
	cube->SetScale({ 10.0f, 10.0f, 10.0f });
	cube->SetPosition({ 20.0f, 10.0f, 30.0f });
	objects.push_back(cube);

	Mesh* cube1 = new Mesh(Mesh::GenerateCube());
	cube1->AddTexture(cubeTexture);
	cube1->SetScale({ 8.0f, 12.0f, 6.0f });
	cube1->SetPosition({ -15.0f, 20.0f, 40.0f });
	cube1->SetRotation({ 30.0f, 0.0f, 45.0f });
	objects.push_back(cube1);

	Mesh* cube2 = new Mesh(Mesh::GenerateCube());
	cube2->AddTexture(cubeTexture);
	cube2->SetScale({ 15.0f, 10.0f, 10.0f });
	cube2->SetPosition({ 35.0f, -10.0f, -20.0f });
	cube2->SetRotation({ 0.0f, 45.0f, 0.0f });
	objects.push_back(cube2);

	Mesh* cube3 = new Mesh(Mesh::GenerateCube());
	cube3->AddTexture(cubeTexture);
	cube3->SetScale({ 6.0f, 6.0f, 12.0f });
	cube3->SetPosition({ -25.0f, 30.0f, 15.0f });
	cube3->SetRotation({ 20.0f, 20.0f, 60.0f });
	objects.push_back(cube3);

	Mesh* cube4 = new Mesh(Mesh::GenerateCube());
	cube4->AddTexture(cubeTexture);
	cube4->SetScale({ 9.0f, 15.0f, 9.0f });
	cube4->SetPosition({ 10.0f, -20.0f, 50.0f });
	cube4->SetRotation({ 45.0f, 30.0f, 10.0f });
	objects.push_back(cube4);

	Mesh* plane = new Mesh(Mesh::GeneratePlane());
	plane->AddTexture(floorTexture);
	plane->SetPosition({ 0.0f, -50.0f, 0.0f });
	plane->SetScale({ 500.0f, 1.0f, 500.0f });
	objects.push_back(plane);

	Mesh quad = Mesh::GenerateQuad();

	FBO framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	framebuffer.AddColorTexture();
	framebuffer.AddDepthStencilRenderbuffer();
	framebuffer.Finalize();

	FBO depthMapFBO(SHADOW_WIDTH, SHADOW_HEIGHT);
	depthMapFBO.AddDepthTexture();
	depthMapFBO.Finalize();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 lightPos(80.0f, 500.0f, -77.0f);
	glm::vec3 lightDir = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
	float near_plane = -1000.0f, far_plane = 1000.0f;
	float orthoSize = 100.0f;

	int lastFrameTime = 0;
	bool rightMouseButtonPressed = false;
	bool quit = false;
	SDL_Event event;
	// MAIN LOOP
	while (!quit) {
		int currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		// EVENTS
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

		ImGui::Begin("Light Controls"); // Begin a new window named "Light Controls"
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::DragFloat3("Light Direction", &lightDir.x, 0.1f);
		ImGui::DragFloat("Near Plane", &near_plane, 0.1f, 0.0f, 0.0f, "%.2f"); // Slider for near plane
		ImGui::DragFloat("Far Plane", &far_plane, 0.1f, 0.0f, 0.0f, "%.2f"); // Slider for far plane
		ImGui::DragFloat("Ortho Size", &orthoSize, 0.1f, 0.0f, 0.0f, "%.2f"); // Slider for orthographic size
		ImGui::End(); // End the window

		// UPDATE
		camera.Update(deltaTime);

		// RENDER
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		// 1. Render depth of scene to texture (from light's perspetive)
		glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		// Render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// First Pass - Shadow Depth Map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		depthMapFBO.Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderScene(simpleDepthShader); // RenderSceneDepth() uses simpleDepthShader
		depthMapFBO.Unbind();

		// Reset viewport
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Second Pass - Render Scene with Shadows into the Framebuffer
		framebuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shadowMapShader.use();
		shadowMapShader.setMat4("projection", projection);
		shadowMapShader.setMat4("view", view);
		shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		depthMapFBO.BindTexture(1); // Use depth map for shadows
		RenderScene(shadowMapShader); // RenderScene() uses shadowMapShader

		framebuffer.Unbind();

		// Third Pass - Post Processing (Grayscale Effect)
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // Disable depth test for screen-space quad

		grayScaleShader.use();
		framebuffer.BindTexture(0); // Use the texture from framebuffer
		quad.Draw(grayScaleShader); // Draw fullscreen quad with grayscale effect

		glEnable(GL_DEPTH_TEST); // Re-enable depth test if needed later

		debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		depthMapFBO.BindTexture(0);
		//quad.Draw(shader);

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

	for (Mesh* obj : objects) {
		delete obj;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	if (!Init("Engine", 1280, 720, false))
		return -1;

	return Run();
}
