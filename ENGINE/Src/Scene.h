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

	glm::vec3 lightPosition = { 500.0f, 5000.0f, 500.0f };
	float nearPlane = 1.0f;
	float farPlane = 10000.0f;
	float orthoSize = 2200.0f;

	bool wireframe = false;
};

void InitScene(Scene* scene);

void UpdateScene(Scene* scene, float deltaTime);

#endif 