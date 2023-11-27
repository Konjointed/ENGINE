#include "Viewport.h"
#include "../Editor.h"

Viewport::Viewport() {}
Viewport::~Viewport() {}

void Viewport::Draw(unsigned int textureColorBuffer) {
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImVec2((float)1280, (float)720);
	ImVec2 uv0 = ImVec2(0.0f, 1.0f);
	ImVec2 uv1 = ImVec2(1.0f, 0.0f);

	ImGui::Image((void*)(intptr_t)textureColorBuffer, size, uv0, uv1);

	ImGui::End();
}