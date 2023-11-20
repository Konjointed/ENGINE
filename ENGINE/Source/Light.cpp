#include "Light.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Light::Light() {}
Light::~Light() {}

glm::mat4 Light::GetSpaceMatrix() {
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::mat4 Light::GetProjectionMatrix() {
	return glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);
}

glm::mat4 Light::GetViewMatrix() {
	return glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
}