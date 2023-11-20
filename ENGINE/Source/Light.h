#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

class Light {
public:
	Light();
	~Light();

	glm::mat4 GetSpaceMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
private:
	glm::vec3 lightPosition = { 80.0f, 500.0f, -77.0f };
	glm::vec3 lightDirection = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
	float nearPlane = -1000.0f;
	float farPlane = 1000.0f;
	float orthoSize = 100.0f;
};