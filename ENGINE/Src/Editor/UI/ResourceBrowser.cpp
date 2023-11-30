#include "ResourceBrowser.h"
#include "../Editor.h"

ResourceBrowser::ResourceBrowser() {}

void ResourceBrowser::Draw(TextEdit& textEdit) {
    ImGui::Begin("Resource Browswer");

    std::string path = "Resources";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry)) {
            if (ImGui::TreeNode(entry.path().filename().string().c_str())) {
                for (const auto& file : std::filesystem::directory_iterator(entry.path())) {
                    if (std::filesystem::is_regular_file(file)) {
                        // Create a selectable item for each file
                        if (ImGui::Selectable(file.path().filename().string().c_str(), false)) {
                            // Check if the file extension is .lua
                            if (file.path().extension() == ".lua") {
                                textEdit.Open(file.path().string());
                            }
                        }
                    }
                }
                ImGui::TreePop();
            }
        }
    }


    ImGui::End();
}