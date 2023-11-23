#ifndef SCENE_H
#define SCENE_H

#include <glm/vec3.hpp>

#include "Camera.h"

struct SceneElements {
	float nearPlane = -1000.0f, farPlane = 1000.0f;
	float orthoSize = 100.0f;
	glm::vec3 lightPosition;
	Camera* camera;
	bool wireframe;
};

#endif 