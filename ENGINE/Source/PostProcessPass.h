#pragma once

#include "RenderPass.h"

class Shader;
class Mesh;

class PostProcessPass : public RenderPass {
public:
	PostProcessPass(Shader* coolEffectShader, unsigned int colorTexture);

	void Execute() override;
private:
	Mesh* quad;
	Shader* shader;
	unsigned int colorTexture;
};