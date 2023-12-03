//#include <glm/ext/matrix_clip_space.hpp>
#include <glm/mat4x4.hpp>
#include <Shader.h>

#include "Renderer.h"
#include "IncludeGL.h"
#include "ResourceManager.h"
#include "Buffers.h"
#include "Window.h"
#include "Scene.h"
#include "GameObject.h"

Renderer::Renderer(Window& window) {
	framebuffer = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	textureColorBuffer = FrameBuffer::CreateTextureAttachment(window.GetWidth(), window.GetHeight());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	renderbuffer = FrameBuffer::CreateRenderBufferAttachment(window.GetWidth(), window.GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowMapFramebuffer = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer);

	shadowMapTexture = FrameBuffer::CreateDepthTextureAttachment(1024, 1024);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer() {}

void Renderer::Render(Window& window, Scene& scene) {
	if (scene.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// =============
	// S H A D O W S
	// =============
	glm::mat4 lightProjection = glm::ortho(-scene.orthoSize, scene.orthoSize, -scene.orthoSize, scene.orthoSize, scene.nearPlane, scene.farPlane);
	glm::mat4 lightView = glm::lookAt(scene.lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	ResourceManager::GetShader("shadow").Use();
	ResourceManager::GetShader("shadow").SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebuffer);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (auto& object : scene.sceneObjects) {
		object->UpdateSelfAndChild();
		object->DrawSelfAndChild(ResourceManager::GetShader("shadow"));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// =========
	// S C E N E
	// =========
	glm::mat4 projection = scene.camera->GetProjectionMatrix();
	glm::mat4 view = scene.camera->GetViewMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& object : scene.sceneObjects) {
		Shader shader = (object->type == STATICMESH) ?
			ResourceManager::GetShader("scene") :
			ResourceManager::GetShader("model");

		object->UpdateSelfAndChild();

		if (object->type == STATICMESH) {
			shader.Use();
			shader.SetMatrix4("projection", projection);
			shader.SetMatrix4("view", view);
			shader.SetVector3f("viewPosition", scene.camera->transform.GetLocalPosition());
			shader.SetVector3f("lightPosition", scene.lightPosition);
			shader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);
		}
		else if (object->type == SKINNEDMESH) {
			shader.Use();
			shader.SetMatrix4("projection", projection);
			shader.SetMatrix4("view", view);

			auto transforms = scene.animator->GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i) {
				std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
				shader.SetMatrix4(name.c_str(), transforms[i]);
			}
		}

		object->DrawSelfAndChild(shader, shadowMapTexture);
	}

	scene.skybox->Draw(view, projection);

	if (scene.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Resize() {

}

unsigned int Renderer::GetTextureColorBuffer() {
	return textureColorBuffer;
}