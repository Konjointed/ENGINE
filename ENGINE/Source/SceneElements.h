#ifndef SCENE_H
#define SCENE_H

#include <glm/vec3.hpp>

#include "Camera.h"

struct SceneElements {
	float nearPlane = -300.0f;
	float farPlane = 300.0f;
	float orthoSize = 1.0f;
	glm::vec3 lightPosition;
	Camera* camera;
	bool wireframe;
};

#endif 