#include "Editor.h"
//#include "ImguiWindow.h"
#include "../Window.h"
#include "../Scene.h"

Editor::Editor(Window& window) : window(window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window.GetWindow(), window.GetContext());
	ImGui_ImplOpenGL3_Init();

	editorUI = new EditorUI;
	luaenv = new LuaEnvironment;
}

Editor::~Editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete editorUI;
	delete luaenv;
}

void Editor::Draw(unsigned int textureColorBuffer, Scene& scene) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	editorUI->Draw(textureColorBuffer, window, *luaenv, scene);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}

/*
void GUI::DrawSceneGraph(GameObject& node) {
	// Start a tree node
	if (ImGui::TreeNode(node.name.c_str())) {
		// Display the node's properties
		ImGui::Text("Position: %f, %f, %f", node.transform.GetLocalPosition().x,
			node.transform.GetLocalPosition().y,
			node.transform.GetLocalPosition().z);
		// ... other properties like rotation, scale, etc.

		// Recursively display children
		for (auto&& child : node.children) {
			DrawSceneGraph(*child);
		}

		// End the tree node
		ImGui::TreePop();
	}
}

void GUI::DrawSceneControls() {
	ImGui::DragFloat("Near Plane", &scene->nearPlane, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
	ImGui::DragFloat("Far Plane", &scene->farPlane, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
	ImGui::DragFloat("Ortho Size", &scene->orthoSize, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
	ImGui::DragFloat3("Light Position", &scene->lightPosition[0], 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
	ImGui::InputFloat3("Camera Position", &(scene->camera->GetPosition()[0]));
	ImGui::DragFloat("Camera Speed", &scene->camera->movementSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
	ImGui::Checkbox("Wireframe Mode", &scene->wireframe);
}
*/