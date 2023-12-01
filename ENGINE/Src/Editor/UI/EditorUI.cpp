#include "EditorUI.h"
#include "../Editor.h"
#include "../../Scene.h"

EditorUI::EditorUI() {}
EditorUI::~EditorUI() {}

void EditorUI::Draw(unsigned int textureColorBuffer, Window& window, LuaEnvironment& luaenv, Scene& scene) {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Test 1");
			ImGui::MenuItem("Test 2");
			ImGui::MenuItem("Test 3");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	console.Draw();
	scenegraph.Draw(scene);
	viewport.Draw(textureColorBuffer, window);
	properties.Draw(scenegraph.GetSelectedObject());
	textEdit.Draw(luaenv);
	resourceBrowser.Draw(textEdit);
}