#ifndef GUI_H
#define GUI_H 

#include <vector>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include "UI/EditorUI.h"

class Window;

class Editor {
public:
	EditorUI editorUI;

	Editor(Window& window);
	~Editor();

	void Draw(unsigned int textureColorBuffer);
private:
};

#endif 
