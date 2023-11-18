#pragma once

#include <vector>

#include <glm/ext/matrix_float4x4.hpp>

#include "RenderPass.h"

class Shader;
class Mesh;
class Camera;

class MainRenderPass : public RenderPass {
public:
	MainRenderPass(Shader* mainShader, unsigned int depthMap, glm::mat4& lightSpaceMatrix, Camera& camera);

	void SetSceneObjects(std::vector<Mesh*>& objects) override;

	unsigned int GetColorTexture();

	void Execute() override;
private:
	std::vector<Mesh*> sceneObjects;
	Shader* mainShader;
	unsigned int depthMap;
	glm::mat4 lightSpaceMatrix;
	Camera& camera;
	unsigned int fbo;
	unsigned int rbo;
	unsigned int texture;
	int width, height;
};