#include <algorithm>

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

Renderer::Renderer(Window& window, Scene& scene) {
	shadowCascadeLevels = { scene.camera->farPlane / 50.0f, scene.camera->farPlane / 25.0f, scene.camera->farPlane / 10.0f, scene.camera->farPlane / 2.0f };

	framebuffer = FrameBuffer::CreateFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	textureColorBuffer = FrameBuffer::CreateTextureAttachment(window.GetWidth(), window.GetHeight());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	renderbuffer = FrameBuffer::CreateRenderBufferAttachment(window.GetWidth(), window.GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &lightFBO);

	glGenTextures(1, &lightDepthMaps);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
	glTexImage3D(
		GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, depthMapResolution, depthMapResolution, int(shadowCascadeLevels.size()) + 1,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lightDepthMaps, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
		throw 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Renderer::~Renderer() {}

void Renderer::Render(Window& window, Scene& scene) {
	shadowCascadeLevels = { scene.camera->farPlane / 50.0f, scene.camera->farPlane / 25.0f, scene.camera->farPlane / 10.0f, scene.camera->farPlane / 2.0f };

	if (scene.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto lightMatrices = getLightSpaceMatrices(scene);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	for (size_t i = 0; i < lightMatrices.size(); ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// =============
	// S H A D O W S
	// =============
	//glm::mat4 lightProjection = glm::ortho(-scene.orthoSize, scene.orthoSize, -scene.orthoSize, scene.orthoSize, scene.nearPlane, scene.farPlane);
	//glm::mat4 lightView = glm::lookAt(scene.lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	//glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	ResourceManager::GetShader("shadow2").Use();
	//ResourceManager::GetShader("shadow").SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glViewport(0, 0, depthMapResolution, depthMapResolution);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT); // peter panning

	for (auto& object : scene.sceneObjects) {
		object->UpdateSelfAndChild();
		object->DrawSelfAndChild(ResourceManager::GetShader("shadow2"));
	}

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ==============
	// RESET VIEWPORT
	// ==============
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// =========
	// S C E N E
	// =========
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = scene.camera->GetProjectionMatrix();
	glm::mat4 view = scene.camera->GetViewMatrix();

	for (auto& object : scene.sceneObjects) {
		Shader shader = (object->type == STATICMESH) ?
			ResourceManager::GetShader("scene") :
			ResourceManager::GetShader("model");

		object->UpdateSelfAndChild();

		if (object->type == STATICMESH) {
			shader.Use();
			shader.SetMatrix4("projection", projection);
			shader.SetMatrix4("view", view);
			shader.SetVector3f("viewPos", scene.camera->transform.GetLocalPosition());
			shader.SetVector3f("lightDir", scene.lightDirection);
			shader.SetFloat("farPlane", scene.camera->farPlane);
			shader.SetInteger("cascadeCount", shadowCascadeLevels.size());
			for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
			{
				std::string uniformName = "cascadePlaneDistances[" + std::to_string(i) + "]";
				shader.SetFloat(uniformName.c_str(), shadowCascadeLevels[i]);
			}
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

		object->DrawSelfAndChild(shader, lightDepthMaps);
	}

	scene.skybox->Draw(view, projection);

	if (scene.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Resize() {

}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview)
{
	const auto inv = glm::inverse(projview);

	std::vector<glm::vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	return getFrustumCornersWorldSpace(proj * view);
}

glm::mat4 Renderer::getLightSpaceMatrix(Scene& scene, const float nearPlane, const float farPlane)
{
	const auto proj = glm::perspective(
		glm::radians(scene.camera->fov), scene.camera->aspectRatio, nearPlane,
		farPlane);
	const auto corners = getFrustumCornersWorldSpace(proj, scene.camera->GetViewMatrix());

	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	const auto lightView = glm::lookAt(center + scene.lightDirection, center, glm::vec3(0.0f, 1.0f, 0.0f));

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 10.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	return lightProjection * lightView;
}

std::vector<glm::mat4> Renderer::getLightSpaceMatrices(Scene& scene)
{
	std::vector<glm::mat4> ret;
	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
	{
		if (i == 0)
		{
			ret.push_back(getLightSpaceMatrix(scene, scene.camera->nearPlane, shadowCascadeLevels[i]));
		}
		else if (i < shadowCascadeLevels.size())
		{
			ret.push_back(getLightSpaceMatrix(scene, shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
		}
		else
		{
			ret.push_back(getLightSpaceMatrix(scene, shadowCascadeLevels[i - 1], scene.camera->farPlane));
		}
	}
	return ret;
}

unsigned int Renderer::GetTextureColorBuffer() {
	return textureColorBuffer;
}