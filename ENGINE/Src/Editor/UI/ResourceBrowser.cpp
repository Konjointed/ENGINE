#include <fstream>

#include "ResourceBrowser.h"
#include "../Editor.h"

ResourceBrowser::ResourceBrowser() {}

void ResourceBrowser::Draw(TextEdit& textEdit) {
    ImGui::Begin("Resource Browser");

    static bool createFilePopup = false;
    static char fileNameBuffer[128] = ""; // Buffer to hold the file name input
    static std::filesystem::path newFilePath; // Path where the new file will be created

    static bool deleteFilePopup = false;
    static std::filesystem::path fileToDelete;

    std::string path = "Resources";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry)) {
            if (ImGui::TreeNode(entry.path().filename().string().c_str())) {

                // Context menu for the directory
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Create New File")) {
                        createFilePopup = true;
                        newFilePath = entry.path(); // Set the path where the new file will be created
                        std::fill(std::begin(fileNameBuffer), std::end(fileNameBuffer), 0); // Clear the buffer
                    }
                    ImGui::EndPopup();
                }

                for (const auto& file : std::filesystem::directory_iterator(entry.path())) {
                    if (std::filesystem::is_regular_file(file)) {
                        // Create a selectable item for each file and check for selection
                        if (ImGui::Selectable(file.path().filename().string().c_str(), false)) {
                            // Check if the file extension is .lua
                            if (file.path().extension() == ".lua") {
                                textEdit.Open(file.path().string());
                            }
                        }

                        // Context menu for each file
                        if (ImGui::BeginPopupContextItem()) {
                            if (ImGui::MenuItem("Delete File")) {
                                deleteFilePopup = true;
                                fileToDelete = file.path();
                            }
                            ImGui::EndPopup();
                        }
                    }
                }
                ImGui::TreePop();
            }
        }
    }

    // Popup for creating a new file
    if (createFilePopup) {
        ImGui::OpenPopup("Create File");
        createFilePopup = false;
    }

    // Popup window to enter the file name
    if (ImGui::BeginPopupModal("Create File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("File Name", fileNameBuffer, sizeof(fileNameBuffer));
        if (ImGui::Button("Create")) {
            // Create the file with the given name
            std::ofstream outfile(newFilePath / fileNameBuffer);
            outfile.close();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Popup for deleting a file
    if (deleteFilePopup) {
        ImGui::OpenPopup("Delete File?");
    }

    // Confirmation popup for deleting the file
    if (ImGui::BeginPopupModal("Delete File?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete this file?");
        ImGui::Text("%s", fileToDelete.filename().string().c_str());
        if (ImGui::Button("Yes")) {
            std::filesystem::remove(fileToDelete); // Delete the file
            deleteFilePopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) {
            deleteFilePopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}
