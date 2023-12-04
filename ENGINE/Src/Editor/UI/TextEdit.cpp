#include <fstream>
#include <filesystem>
#include <iostream>

#include "TextEdit.h"

TextEdit::TextEdit() {
	ImGuiIO& io = ImGui::GetIO();
	font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/FiraCode.ttf", 25.0f);
}

void TextEdit::Draw(LuaEnvironment& luaenv) {
    for (auto it = editors.begin(); it != editors.end(); ) {
        const std::string& editorPath = it->first;
        TextEditor& editor = *it->second;

        // Begin a new window with a menu bar for each editor
        bool isOpen = true;
        ImGui::Begin(editorPath.c_str(), &isOpen);

        ImGui::PushFont(font);
        editor.Render("TextEditor");
        ImGui::PopFont();

        // Menu bar with close button
        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("Close")) {
                isOpen = false;  // Mark as not open
            }
            ImGui::EndMenuBar();
        }

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

        if (!isOpen) {
            it = editors.erase(it);
        }
        else {
            ++it;
        }
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