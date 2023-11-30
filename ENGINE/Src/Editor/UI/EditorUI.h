#ifndef EDITOR_UI_H
#define EDITOR_UI_H

#include "Console.h"
#include "SceneGraph.h"
#include "Viewport.h"
#include "Properties.h"
#include "TextEdit.h"
#include "ResourceBrowser.h"

class Window;

class EditorUI {
public:
	Console console;
	SceneGraph scenegraph;
	Viewport viewport;
	Properties properties;
	TextEdit textEdit;
	ResourceBrowser resourceBrowser;

	EditorUI();
	~EditorUI();

	void Draw(unsigned int textureColorBuffer, Window& window, LuaEnvironment& luaenv);
private:
};

#endif 