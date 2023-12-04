#ifndef EDITOR_UI_H
#define EDITOR_UI_H

#include "Console.h"
#include "SceneGraph.h"
#include "Viewport.h"
#include "Properties.h"
#include "TextEdit.h"
#include "ResourceBrowser.h"
#include "../../LuaEnvironment.h"

class Window;
class LuaEnvironment;
struct Scene;

class EditorUI {
public:
	Console console;
	SceneGraph scenegraph;
	Viewport viewport;
	Properties properties;
	TextEdit textEdit;
	ResourceBrowser resourceBrowser;

	LuaEnvironment* luaenv = nullptr; // Note: move this back to application class

	EditorUI();
	~EditorUI();

	void Draw(unsigned int textureColorBuffer, Window& window, Scene& scene);
private:
};

#endif 