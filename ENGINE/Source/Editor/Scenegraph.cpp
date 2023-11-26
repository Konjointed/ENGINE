#include "Scenegraph.h"
#include "../GameObject.h"
#include "../SceneElements.h"

SceneGraph::SceneGraph(GUI& gui) : ImguiWindow(gui) {}

SceneGraph::~SceneGraph() {}

void SceneGraph::Draw() {
	if (ImGui::Begin("Scene Graph")) {
		DisplaySceneGraph(GameObject::scene->scenegraph);
	}
	ImGui::End();
}

void SceneGraph::DisplaySceneGraph(GameObject& node) {
	// Start a tree node
	if (ImGui::TreeNode(node.name.c_str())) {
		// Display the node's properties
		ImGui::Text("Position: %f, %f, %f", node.transform.GetLocalPosition().x,
			node.transform.GetLocalPosition().y,
			node.transform.GetLocalPosition().z);
		// ... other properties like rotation, scale, etc.

		// Recursively display children
		for (auto&& child : node.children) {
			DisplaySceneGraph(*child);
		}

		// End the tree node
		ImGui::TreePop();
	}
}