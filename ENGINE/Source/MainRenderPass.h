#pragma once

#include "RenderPass.h"

class MainRenderPass : public RenderPass {
public:
	MainRenderPass(Shader* mainShader, GLuint depthMap, glm::mat4& lightSpaceMatrix, Camera& camera)
		: mainShader(mainShader), depthMap(depthMap), lightSpaceMatrix(lightSpaceMatrix), camera(camera) {}

	void SetSceneObjects(std::vector<Mesh*>& objects) override {
		sceneObjects = objects;
	}

	void Execute() override {
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
	}
private:
	std::vector<Mesh*> sceneObjects;
	Shader* mainShader;
	GLuint depthMap;
	glm::mat4 lightSpaceMatrix;
	Camera& camera;
};