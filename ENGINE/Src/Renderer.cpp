//#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>

#include "Renderer.h"
#include "IncludeGL.h"
#include "ResourceManager.h"
#include "Buffers.h"
#include "GameObject.h"
#include "Window.h"
#include "Skybox.h"
#include "Animator.h"
#include "Camera.h"

Renderer::Renderer(Window& window) {
	framebuffer = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	textureColorBuffer = FrameBuffer::CreateTextureAttachment(window.GetWidth(), window.GetHeight());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	renderbuffer = FrameBuffer::CreateRenderBufferAttachment(window.GetWidth(), window.GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer() {}

void Renderer::Render(Window& window, Camera& camera, Animator& animator, Skybox& skybox, GameObject& scenegraph) {
	/*
	if (scene.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	*/

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ResourceManager::GetShader("model").Use();

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix();
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

	/*
	if (scene.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Resize() {

}

unsigned int Renderer::GetTextureColorBuffer() {
	return textureColorBuffer;
}