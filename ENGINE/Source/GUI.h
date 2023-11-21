#ifndef GUI_H
#define GUI_H 

#include <vector>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

class Window;

class GUI {
public:
	GUI(Window& window);
	~GUI();

	void Draw();

	void AddLog(const std::string& log);
private:
	void DrawSceneControls();

	void DrawConsole();
	void ExecuteCommand(const std::string& command);
	bool IsLineVisible(const std::string& line);

	char inputBuf[256] = "";
	char filterBuf[256] = "";
	std::vector<std::string> output;
};

#endif 
