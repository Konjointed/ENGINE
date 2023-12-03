#pragma once

#include <SDL2/SDL_events.h>
#include <glm/ext/matrix_transform.hpp>

#include "GameObject.h"
#include "Editor/UI/Viewport.h"

class Camera : public GameObject {
public:
	float movementSpeed = 1.0f;

	Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = 0.0f);
	~Camera();

	void UpdateSelfAndChild() override;
	void DrawSelfAndChild(Shader shader, unsigned int shadowMapTexture) override;

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void Update(float deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetFront();
private:
	float fov = 45.0f;
	float aspectRatio = (float)Viewport::VIEWPORT_WIDTH / Viewport::VIEWPORT_HEIGHT;
	float nearPlane = 0.1f;
	float farPlane = 10000.0f;

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
