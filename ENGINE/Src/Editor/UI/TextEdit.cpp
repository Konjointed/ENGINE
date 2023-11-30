#include <fstream>
#include <filesystem>
#include <iostream>

#include "TextEdit.h"

TextEdit::TextEdit() {
	ImGuiIO& io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/FiraCode.ttf", 25.0f);
}

void TextEdit::Draw(LuaEnvironment& luaenv) {
    for (auto& pair : editors) {
        const std::string& editorPath = pair.first;
        TextEditor& editor = *pair.second;

        // Begin a new window for each editor
        ImGui::Begin(editorPath.c_str());

        ImGui::PushFont(font);
        editor.Render("TextEditor");
        ImGui::PopFont();

        // Context menu for each editor
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup("TextEditorPopup");
        }

        if (ImGui::BeginPopup("TextEditorPopup")) {
            if (ImGui::MenuItem("Run Lua Script")) {
                luaenv.DoFile(editorPath.c_str());
            }
            ImGui::EndPopup();
        }

        // Saving for each editor
        if (editor.IsTextChanged()) {
            std::string currentText = editor.GetText();
            std::ofstream out(editorPath);
            out << currentText;
            out.close();
        }

        ImGui::End();
    }
}

void TextEdit::Open(std::string path) {
	auto editor = std::make_unique<TextEditor>();
	editor->SetShowWhitespaces(false);

	//auto lang = TextEditor::LanguageDefinition::CPlusPlus();
	//auto lang = TextEditor::LanguageDefinition::GLSL();
	auto lang = TextEditor::LanguageDefinition::Lua();
	editor->SetLanguageDefinition(lang);

    std::ifstream file(path);
    if (file) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        editor->SetText(content);
    }

	editors[path] = std::move(editor);
}