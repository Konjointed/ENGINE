#ifndef GUI_H
#define GUI_H 

#include <vector>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

class Window;
class ImguiWindow;

class GUI {
public:
	GUI(Window& window);
	~GUI();

	void DrawWindows();
	void RegisterWindow(ImguiWindow* window);
	void UnregisterWindow(ImguiWindow* window);
private:
	ImGuiIO* io = nullptr;
	std::vector<ImguiWindow*> imguiWindows;
};

#endif 
