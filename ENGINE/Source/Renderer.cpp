#include "Renderer.h"
#include "IncludeGL.h"
#include "GameObject.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::Render() {
	ResetRenderState();

	// Draw scene graph
	/*
	GameObject* lastGameObject = nullptr;
	while (lastGameObject->children.size()) {
		shader.SetMat4("model", lastGameObnject->transform.GetModelMatrix());
		lastGameObject->model->Draw(shader);
		lastGameObject = lastGameObject->children.back().get();
	}
	*/
}

void Renderer::ResetRenderState() {
	glDepthFunc(GL_LESS); // For skybox
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}