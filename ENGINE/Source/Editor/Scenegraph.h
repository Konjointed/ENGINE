#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <string>
#include <vector>

#include "ImguiWindow.h"
#include "GUI.h"

class GameObject;

class SceneGraph : public ImguiWindow {
public:
	SceneGraph(GUI& gui);
	~SceneGraph();

	void Draw() override;
private:
	void DisplaySceneGraph(GameObject& node);
};

#endif 