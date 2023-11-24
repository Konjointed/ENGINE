#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <glm/vec3.hpp>

class Meshes;
class Camera;
class DrawableObject;

/*
struct SceneElements {
	float nearPlane = -300.0f, farPlane = 300.0f;
	float orthoSize = 1.0f;
	glm::vec3 lightPosition;
	Camera* camera;
	bool wireframe;
};
*/

class Scene {
public:
	std::vector<DrawableObject> objects;

	float nearPlane = -300.0f;
	float farPlane = 300.0f;
	float orthoSize = 1.0f;
	glm::vec3 lightPosition = { 0.0f, 0.0f, 0.0f };

	Camera* camera = nullptr;

	bool wireframe = false;
};

void AddInstance() {}

#endif 