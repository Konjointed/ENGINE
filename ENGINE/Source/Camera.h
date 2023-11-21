#pragma once

#include <SDL2/SDL_events.h>
#include <glm/ext/matrix_transform.hpp>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = 0.0f);
	~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void Update(float deltaTime);
	glm::vec3& GetPosition();
	glm::vec3 GetFront();

	float movementSpeed = 30.0f;
private:
	float fov = 45.0f;
	float aspectRatio = static_cast<float>(1280) / 720;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	//float movementSpeed = 30.0f;
	float mouseSensitivity = 0.1f;
	bool firstMouse = true;
	float lastX, lastY;
};
