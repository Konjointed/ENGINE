#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include "GUI.h"

class ImguiWindow {
public:
	ImguiWindow(GUI& gui);
	~ImguiWindow();

	virtual void Draw() = 0;
	virtual bool IsVisible() { return true; }
private:
	GUI& gui;

	std::string title;
	bool visible = true;
};

#endif 