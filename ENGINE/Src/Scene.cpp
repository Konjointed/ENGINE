#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"

void InitScene(Scene* scene) {
	auto playerModel = std::make_shared<Model>("Resources/Models/Maria/Maria J J Ong.dae", false);
	auto playerObject = std::make_shared<GameObject>(playerModel);
	scene->sceneObjects.push_back(playerObject);

	scene->idleAnimation = new Animation("Resources/Animations/Idle.dae", playerModel.get());
	scene->animator = new Animator(scene->idleAnimation);

	auto cameraObject = std::make_shared<Camera>();
	scene->sceneObjects.push_back(cameraObject);
	scene->camera = cameraObject;
}

void UpdateScene(Scene* scene, float deltaTime) {
	scene->camera->Update(deltaTime);
}