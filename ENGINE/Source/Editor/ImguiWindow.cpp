#include "ImguiWindow.h"
#include "GUI.h"

ImguiWindow::ImguiWindow(GUI& gui): gui(gui) {
	gui.RegisterWindow(this);
}

ImguiWindow::~ImguiWindow() {
	gui.UnregisterWindow(this);
}