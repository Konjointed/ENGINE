#include "Properties.h"
#include "../Editor.h"

Properties::Properties() {}
Properties::~Properties() {}

void Properties::Draw() {
	ImGui::Begin("Properties");
	ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

	ImGui::End();
}