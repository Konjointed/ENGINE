#include "Camera.h"

#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera(glm::vec3 up, float yaw, float pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    GameObject()
{
    std::cout << "Constructing Camera\n";
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
}

Camera::~Camera() {
    std::cout << "Destructing Camera\n";
}

void Camera::UpdateSelfAndChild() {
    GameObject::UpdateSelfAndChild(); 
}

void Camera::DrawSelfAndChild(Shader shader, unsigned int shadowMapTexture) {
    GameObject::DrawSelfAndChild(shader, 0); 
}

void Camera::ProcessKeyboard(float deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    float velocity = movementSpeed * deltaTime;
    if (state[SDL_SCANCODE_W]) {
        glm::vec3 position = transform.GetLocalPosition();
        position += front * velocity;
        transform.SetLocalPosition(position);
    }
    if (state[SDL_SCANCODE_S]) {
        glm::vec3 position = transform.GetLocalPosition();
        position -= front * velocity;
        transform.SetLocalPosition(position);
    }
    if (state[SDL_SCANCODE_A]) {
        glm::vec3 position = transform.GetLocalPosition();
        position -= right * velocity;
        transform.SetLocalPosition(position);
    }
    if (state[SDL_SCANCODE_D]) {
        glm::vec3 position = transform.GetLocalPosition();
        position += right * velocity;
        transform.SetLocalPosition(position);
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

    aspectRatio = Viewport::VIEWPORT_WIDTH / Viewport::VIEWPORT_HEIGHT;

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

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(transform.GetLocalPosition(), transform.GetLocalPosition() + front, up);
}

glm::mat4 Camera::GetProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::GetFront() {
    return front;
}