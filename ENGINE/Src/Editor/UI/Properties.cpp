#include <iostream>
#include <memory>

#include "Properties.h"
#include "../Editor.h"

Properties::Properties() {}
Properties::~Properties() {}

void Properties::CameraProperties(Camera* camera) {
    Transform& transform = camera->transform;
    glm::vec3 position = transform.GetLocalPosition();

    ImGui::InputFloat3("Camera Position", &(position[0]));
    ImGui::DragFloat("Camera Speed", &camera->movementSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
}

void Properties::ObjectProperties(GameObject* object) {
    Transform& transform = object->transform;

    // Display and edit position
    glm::vec3 position = transform.GetLocalPosition();
    if (ImGui::InputFloat3("Position", &position[0])) {
        transform.SetLocalPosition(position);
    }

    // Display and edit rotation
    glm::vec3 rotation = transform.GetLocalRotation();
    if (ImGui::InputFloat3("Rotation", &rotation[0])) {
        transform.SetLocalRotation(rotation);
    }

    // Display and edit scale
    glm::vec3 scale = transform.GetLocalScale();
    if (ImGui::InputFloat3("Scale", &scale[0])) {
        transform.SetLocalScale(scale);
    }
}

void Properties::Draw(GameObject* selectedObject) {
	ImGui::Begin("Properties");
	ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

	if (selectedObject) {
        char name[128];
        strncpy_s(name, selectedObject->name.c_str(), sizeof(name));
        if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
            selectedObject->name = name;
        }

        if (selectedObject->classID == CLASS_ID_CAMERA) {
            Camera* camera = static_cast<Camera*>(selectedObject);
            CameraProperties(camera);
        }

        if (selectedObject->classID == CLASS_ID_GAMEOBJECT) {
            ObjectProperties(selectedObject);
        }
	}

	ImGui::End();
}