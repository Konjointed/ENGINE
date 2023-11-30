#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <string>
#include <unordered_map>
#include <memory>

#include <ImGuiColorTextEdit/TextEditor.h>

#include "../../LuaEnvironment.h"

class TextEdit {
public:
	TextEdit();
	void Draw(LuaEnvironment& luaenv);
	void Open(std::string path);
private:
	std::unordered_map<std::string, std::unique_ptr<TextEditor>> editors;
	ImFont* font;
};

#endif 