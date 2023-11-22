#ifndef SCENE_H
#define SCENE_H

#include <glm/vec3.hpp>

#include "Camera.h"

class Scene {
public:
	static glm::vec3 lightPosition;
	static Camera* camera;
	static bool wireframe;
private:
};

#endif 