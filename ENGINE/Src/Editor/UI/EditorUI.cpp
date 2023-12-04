#include "EditorUI.h"
#include "../Editor.h"
#include "../../Scene.h"

EditorUI::EditorUI() {
	luaenv = new LuaEnvironment(console);
}

EditorUI::~EditorUI() {
	delete luaenv;
}

void EditorUI::Draw(unsigned int textureColorBuffer, Window& window, Scene& scene) {
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
	properties.Draw(scenegraph.GetSelectedObject(), scene);
	textEdit.Draw(*luaenv);
	resourceBrowser.Draw(textEdit);
}