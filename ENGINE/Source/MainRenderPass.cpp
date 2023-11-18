#include "MainRenderPass.h"

#include <glad/glad.h>
#include <Shader.h>

#include "Camera.h"
#include "Mesh.h"

MainRenderPass::MainRenderPass(Shader* mainShader, unsigned int depthMap, glm::mat4& lightSpaceMatrix, Camera& camera)
	: mainShader(mainShader), depthMap(depthMap), lightSpaceMatrix(lightSpaceMatrix), camera(camera),
	width(1280), height(720)
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// create a color attachment texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainRenderPass::SetSceneObjects(std::vector<Mesh*>& objects) {
	sceneObjects = objects;
}

unsigned int MainRenderPass::GetColorTexture() {
	return texture;
}

void MainRenderPass::Execute() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainShader->use();
	mainShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	mainShader->setMat4("view", camera.GetViewMatrix());
	mainShader->setMat4("projection", camera.GetProjectionMatrix());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//mainShader->setInt("diffuseTexture", 0);
	mainShader->setInt("shadowMap", 1);

	for (auto& object : sceneObjects) {
		object->Draw(*mainShader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}