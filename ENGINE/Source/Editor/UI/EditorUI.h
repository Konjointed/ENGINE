#ifndef EDITOR_UI_H
#define EDITOR_UI_H

#include "Console.h"
#include "SceneGraph.h"
#include "Viewport.h"
#include "Properties.h"

class EditorUI {
public:
	Console console;
	SceneGraph scenegraph;
	Viewport viewport;
	Properties properties;

	EditorUI();
	~EditorUI();

	void Draw(unsigned int textureColorBuffer);
private:
};

#endif 