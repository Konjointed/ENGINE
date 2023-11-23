#ifndef SCENE_H
#define SCENE_H

#include <glm/vec3.hpp>

#include "Camera.h"

struct SceneElements {
	glm::vec3 lightPosition;
	Camera* camera;
	bool wireframe;
};

#endif 