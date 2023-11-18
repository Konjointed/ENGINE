#pragma once

#include "RenderPass.h"

class MainRenderPass : public RenderPass {
public:
	MainRenderPass(Shader* mainShader, GLuint depthMap, glm::mat4& lightSpaceMatrix, Camera& camera)
		: shader(mainShader), depthMap(depthMap), lightSpaceMatrix(lightSpaceMatrix), camera(camera),
		width(1280), height(720)
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void SetSceneObjects(std::vector<Mesh*>& objects) override {
		sceneObjects = objects;
	}

	void Execute() override {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); // Bind the off-screen framebuffer
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();
		shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader->setMat4("view", camera.GetViewMatrix());
		shader->setMat4("projection", camera.GetProjectionMatrix());

		glActiveTexture(GL_TEXTURE1); 
		glBindTexture(GL_TEXTURE_2D, depthMap);

		//mainShader->setInt("diffuseTexture", 0);
		mainShader->setInt("shadowMap", 1);

		for (auto& object : sceneObjects) {
			object->Draw(*mainShader);
		}
	}
private:
	std::vector<Mesh*> sceneObjects;
	Shader* shader;
	unsigned int fbo;
	unsigned int rbo;
	unsigned int texture;
	unsigned int depthMap;
	glm::mat4 lightSpaceMatrix;
	Camera& camera;
	int width, height;
};