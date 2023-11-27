#include "Scenegraph.h"
#include "../../GameObject.h"
#include "../../SceneElements.h"
#include "../Editor.h"

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void SceneGraph::Draw() {
	if (ImGui::Begin("Scene Graph")) {
		ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

		if (ImGui::TreeNode("Camera")) {
			ImGui::InputFloat3("Camera Position", &(GameObject::scene->camera->GetPosition()[0]));
			ImGui::DragFloat("Camera Speed", &GameObject::scene->camera->movementSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
			ImGui::TreePop();
		}

		DisplaySceneGraph(GameObject::scene->scenegraph);

		// Right-click on blank space
		if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered()) {
			ImGui::OpenPopup("SceneGraphContextMenu");
		}

		// Context menu
		if (ImGui::BeginPopup("SceneGraphContextMenu")) {
			if (ImGui::MenuItem("Add Camera")) {
				// Functionality to add a camera node
			}
			if (ImGui::MenuItem("Add Model")) {
				// Functionality to add a model node
			}
			// Add other menu items as needed

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
		selectedObject = &object;
	}

	if (node_open) {
		// Recursively display children
		for (auto&& child : object.children) {
			DisplaySceneGraph(*child);
		}
		ImGui::TreePop();
	}
}