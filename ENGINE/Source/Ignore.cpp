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
	std::vector<std::string> faces{
		"Resources/Textures/skybox/right.jpg",
		"Resources/Textures/skybox/left.jpg",
		"Resources/Textures/skybox/top.jpg",
		"Resources/Textures/skybox/bottom.jpg",
		"Resources/Textures/skybox/front.jpg",
		"Resources/Textures/skybox/back.jpg",
	};

	Shader shader("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag");
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
	plane.SetScale({ 100.0f, 3.0f, 100.0f });

	Mesh skybox = Mesh::GenerateCube();

	Mesh screenQuad = Mesh::GenerateQuad();

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
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		float near_plane = -1000.0f, far_plane = 1000.0f;
		float orthoSize = 50.0f;
		glm::vec3 lightPosition(80.0f, 500.0f, -77.0f);
		glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);;

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
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

		// reset viewport
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		defaultShader.use();
		defaultShader.setInt("diffuseTexture", 0);
		defaultShader.setInt("shadowMap", 1);
		defaultShader.setMat4("view", view);
		defaultShader.setMat4("projection", projection);
		defaultShader.setVec3("viewPos", camera->GetPosition());
		defaultShader.setVec3("lightPos", lightPosition);
		defaultShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		// RENDER SCENE
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
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		skybox.Draw();
		glDepthFunc(GL_LESS);

		debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//screenQuad.Draw();

		window->SwapBuffers();
	}

	Shutdown();

	return 1;
}