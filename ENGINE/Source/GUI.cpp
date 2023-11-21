#include "GUI.h"

#include "Window.h"

GUI::GUI(Window& window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window.GetWindow(), window.GetContext());
	ImGui_ImplOpenGL3_Init();
}

GUI::~GUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	static bool showDemoWindow = false;
	//static bool showSceneControls = false;
	static bool showConsole = false;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("ImGui Demo", NULL, &showDemoWindow);
			//ImGui::MenuItem("Scene Controls", NULL, &showSceneControls);
			ImGui::MenuItem("Console", NULL, &showConsole);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (showDemoWindow) {
		ImGui::ShowDemoWindow();
	}

	if (showConsole) {
		ImGui::Begin("Console", &showConsole);
		DrawConsole();
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::AddLog(const std::string& log) {
	output.push_back(log);
}

void GUI::DrawConsole() {
	if (ImGui::Button("Clear")) {
		output.clear();
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Error")) {
		AddLog("[Error] error message");
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Warning")) {
		AddLog("[Warning]  warning message");
	}

	// Console output
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (const auto& line : output) {
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
	ImGui::EndChild();

	// Input text field
	if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf),
		ImGuiInputTextFlags_EnterReturnsTrue)) {
		std::string inputStr = inputBuf;

		ExecuteCommand(inputStr);

		inputBuf[0] = '\0';
	}
}

void GUI::ExecuteCommand(const std::string& command) {
	AddLog("Executing command: " + command);
}