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

	Animation* idleAnimation;
	Animator* animator;
};

void InitScene(Scene* scene);

void UpdateScene(Scene* scene, float deltaTime);

#endif 