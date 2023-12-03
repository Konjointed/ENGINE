#include <iostream>

#include "Scenegraph.h"
#include "../Editor.h"
#include "../../GameObject.h"
#include "../../Scene.h"

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void SceneGraph::Draw(Scene& scene) {
	if (ImGui::Begin("Scene Graph")) {
		ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

		for (auto& object : scene.sceneObjects) {
			DisplaySceneGraph(*object);
		}

		// Right-click on blank space
		if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
			ImGui::OpenPopup("SceneGraphContextMenu");
		}

		// Context menu
		if (ImGui::BeginPopup("SceneGraphContextMenu")) {
			if (ImGui::MenuItem("Add Object")) {
				// Queue command to add object
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void SceneGraph::DisplaySceneGraph(GameObject& object) {
	// Start a tree node with a selectable label
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (selectedObject == &object) {
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}
	bool node_open = ImGui::TreeNodeEx(object.name.c_str(), node_flags);
	if (ImGui::IsItemClicked()) {
		std::cout << "setting selected object\n";
		selectedObject = &object;
	}

	if (node_open) {
		/*
		if (object.children.empty()) {
			std::cout << "HAS NO CHILDREN\n";
		}
		*/

		// Recursively display children
		for (auto&& child : object.children) { 
			DisplaySceneGraph(*child);
		}
		ImGui::TreePop();
	}
}

GameObject* SceneGraph::GetSelectedObject() {
	return selectedObject;
}