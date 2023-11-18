#pragma once

#include <vector>

#include <glm/ext/matrix_float4x4.hpp>

#include "RenderPass.h"

class Shader;
class Mesh;

class ShadowMapPass : public RenderPass {
public:
	ShadowMapPass(Shader* depthShader, glm::mat4 lightSpaceMatrix);

	void SetSceneObjects(std::vector<Mesh*>& objects) override;

	void Execute() override;

	unsigned int GetDepthTexture();

	~ShadowMapPass();
private:
	std::vector<Mesh*> sceneObjects;
	Shader* depthShader;
	glm::mat4 lightSpaceMatrix;
	int width;
	int height;
	unsigned int fbo;
	unsigned int textureID;
};