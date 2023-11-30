#include "Viewport.h"
#include "../Editor.h"
#include "../../Window.h"

float Viewport::VIEWPORT_WIDTH = 1280.0f;
float Viewport::VIEWPORT_HEIGHT = 720.0f;

Viewport::Viewport() {}
Viewport::~Viewport() {}

void Viewport::Draw(unsigned int textureColorBuffer, Window& window) {
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	Viewport::VIEWPORT_WIDTH = size.x;
	Viewport::VIEWPORT_HEIGHT = size.y;
	//std::cout << size.x << " " << size.y << "\n";
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);

	ImGui::Image((void*)(intptr_t)textureColorBuffer, size, uv0, uv1);

	ImGui::End();
}