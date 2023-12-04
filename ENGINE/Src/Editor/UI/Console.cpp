#include "Console.h"
#include "../Editor.h"

Console::Console() {}
Console::~Console() {}

void Console::Draw() {
	ImGui::Begin("Console");
	ImGui::InputText("Filter", filterBuf, IM_ARRAYSIZE(filterBuf));

	if (ImGui::Button("Clear")) {
		output.clear();
	}
	/*
	ImGui::SameLine();
	if (ImGui::Button("Add Error")) {
		AddLog("[Error] error message");
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Warning")) {
		AddLog("[Warning]  warning message");
	}
	*/

	// Console output
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (const auto& line : output) {
		if (IsLineVisible(line)) {
			ImVec4 color;
			if (line.find("[Error]") != std::string::npos) {
				color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Red color for errors
			}
			else if (line.find("[Warning]") != std::string::npos) {
				color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); // Yellow color for warnings
			}
			else {
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White color for regular messages
			}
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(line.c_str());
			ImGui::PopStyleColor();
		}
	}
	ImGui::EndChild();

	// Input text field
	if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf),
		ImGuiInputTextFlags_EnterReturnsTrue)) {
		std::string inputStr = inputBuf;

		ExecuteCommand(inputStr);

		inputBuf[0] = '\0';
	}
	ImGui::End();
}

void Console::AddLog(const std::string& log) {
	output.push_back(log);
}

void Console::ExecuteCommand(const std::string& command) {
	AddLog("Executing command: " + command);
}

bool Console::IsLineVisible(const std::string& line) {
	if (filterBuf[0] == '\0') {
		return true;
	}
	return line.find(filterBuf) != std::string::npos;
}
