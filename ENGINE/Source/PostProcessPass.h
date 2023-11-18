#pragma once

#include "RenderPass.h"

class PostProcessPass : public RenderPass {
public:
	PostProcessPass(Shader* coolEffectShader): shader(coolEffectShader), width(1280), height(720) {
		quad = new Mesh(Mesh::GenerateQuad());

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Execute() override {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		shader->use();
		shader->setInt("screenTexture", 0);
		glBindTexture(GL_TEXTURE_2D, texture);
		quad->Draw(*shader);
		glEnable(GL_DEPTH_TEST);
	}
private:
	Mesh* quad;
	Shader* shader;
	unsigned int fbo;
	unsigned int texture;
	int width, height;
};