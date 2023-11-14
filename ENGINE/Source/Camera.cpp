#include "Camera.h"

#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
}

Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::ProcessKeyboard(float deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    float velocity = movementSpeed * deltaTime;
    if (state[SDL_SCANCODE_W]) {
        position += front * velocity;
    }
    if (state[SDL_SCANCODE_S]) {
        position -= front * velocity;
    }
    if (state[SDL_SCANCODE_A]) {
        position -= right * velocity;
    }
    if (state[SDL_SCANCODE_D]) {
        position += right * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch -= yoffset; // Subtracting to invert the y-axis movement

    // Constrain the pitch to prevent screen flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void Camera::Update(float deltaTime) {
    ProcessKeyboard(deltaTime);

    // Calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    // Re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::GetPosition() {
    return position;
}

glm::vec3 Camera::GetFront() {
    return front;
}