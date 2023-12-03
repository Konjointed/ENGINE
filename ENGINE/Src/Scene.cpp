#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Texture.h"

void InitScene(Scene* scene) {
	auto testObject = std::make_shared<GameObject>();
	testObject->type = STATICMESH;
	testObject->name = "Workspace";
	scene->sceneObjects.push_back(testObject);

	auto cubeModel = std::make_shared<Model>(Model::GenerateCube());
	auto cubeObject = std::make_unique<GameObject>(cubeModel);
	cubeObject->type = STATICMESH;
	cubeObject->name = "Cube";
	scene->sceneObjects.push_back(std::move(cubeObject));
	//testObject->AddChild(std::move(cubeObject));

	auto planeModel = std::make_shared<Model>(Model::GeneratePlane());
	auto planeObject = std::make_unique<GameObject>(planeModel);
	planeObject->type = STATICMESH;
	planeObject->name = "Plane";
	scene->sceneObjects.push_back(std::move(planeObject));
	//testObject->AddChild(std::move(planeObject));

	auto playerModel = std::make_shared<Model>("Resources/Models/Maria/Maria J J Ong.dae", false);
	auto playerObject = std::make_shared<GameObject>(playerModel);
	playerObject->type = SKINNEDMESH;
	playerObject->name = "Player";
	scene->sceneObjects.push_back(playerObject);

	scene->idleAnimation = new Animation("Resources/Animations/Idle.dae", playerModel.get());
	scene->animator = new Animator(scene->idleAnimation);

	auto cameraObject = std::make_shared<Camera>();
	cameraObject->type = NONE;
	cameraObject->name = "Camera";
	scene->sceneObjects.push_back(cameraObject);
	scene->camera = cameraObject;

	auto skyboxObject = std::make_shared<Skybox>();
	scene->skybox = skyboxObject;
}

void UpdateScene(Scene* scene, float deltaTime) {
	scene->camera->Update(deltaTime);
}