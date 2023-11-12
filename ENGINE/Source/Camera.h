#pragma once

#include <glm/ext/matrix_transform.hpp>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = 0.0f);
	~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void Update();
private:
	float fov = 45.0f;
	float aspectRatio = static_cast<float>(1280) / 720;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
};
