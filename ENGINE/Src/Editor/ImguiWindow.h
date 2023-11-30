#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <string>

class ImguiWindow {
public:
	ImguiWindow();
	~ImguiWindow();

	virtual void Draw() = 0;
	virtual bool IsVisible() { return true; }
private:
	std::string title;
	bool visible = true;
};

#endif 