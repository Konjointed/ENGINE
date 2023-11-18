#pragma once

#include "Framebuffer.h"
#include "RenderPass.h"

class ShadowMapPass : public RenderPass {
public:
	ShadowMapPass(Shader* depthShader, glm::mat4 lightSpaceMatrix) : 
		depthShader(depthShader), 
		lightSpaceMatrix(lightSpaceMatrix), 
		width(1024), height(1024) 
	{
		// Create and bind the framebuffer
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Depth Attachment
		// Create texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// Attach texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TO-DO: Add error check
	}

	void SetSceneObjects(std::vector<Mesh*>& objects) override {
		sceneObjects = objects;
	}

	void Execute() override {
		depthShader->use();
		depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, width, height);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (auto& object : sceneObjects) {
			object->Draw(*depthShader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int GetDepthTexture() {
		return textureID;
	}

	~ShadowMapPass() {
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &textureID);
	}
private:
	std::vector<Mesh*> sceneObjects;
	Shader* depthShader;
	glm::mat4 lightSpaceMatrix;
	int width;
	int height;
	unsigned int fbo;
	unsigned int textureID;
};