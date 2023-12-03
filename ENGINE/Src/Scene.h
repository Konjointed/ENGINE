#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Camera.h"
#include "Skybox.h"
#include "Animation.h"
#include "Animator.h"

class GameObject;

struct Scene {
	std::vector<std::shared_ptr<GameObject>> sceneObjects;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Skybox> skybox;

	Animation* idleAnimation;
	Animator* animator;

	glm::vec3 lightPosition = { 80.0f, 500.0f, -77.0f };
	float nearPlane = -1000.0f;
	float farPlane = 1000.0f;
	float orthoSize = 50.0f;

	bool wireframe = false;
};

void InitScene(Scene* scene);

void UpdateScene(Scene* scene, float deltaTime);

#endif 