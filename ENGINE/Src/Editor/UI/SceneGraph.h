#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <string>
#include <vector>

class GameObject;
struct Scene;

class SceneGraph {
public:
	SceneGraph();
	~SceneGraph();

	void Draw(Scene& scene);
	GameObject* GetSelectedObject();
private:
	char searchQuery[128] = "";

	GameObject* selectedObject = nullptr;

	void DisplaySceneGraph(GameObject& object);
};

#endif 