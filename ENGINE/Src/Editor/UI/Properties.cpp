#include <iostream>
#include <memory>

#include "Properties.h"
#include "../Editor.h"
#include "../../Scene.h"

Properties::Properties() {}
Properties::~Properties() {}

void Properties::CameraProperties(Camera* camera) {
    Transform& transform = camera->transform;
    glm::vec3 position = transform.GetLocalPosition();

    if (ImGui::InputFloat3("Camera Position", &position[0])) {
        transform.SetLocalPosition(position);
    }

    ImGui::DragFloat("Camera Speed", &camera->movementSpeed, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
    ImGui::DragFloat("Far Plane", &camera->farPlane, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
    ImGui::DragFloat("FOV", &camera->fov, 0.1f, -FLT_MAX, FLT_MAX, "%.3f");
}

void Properties::ObjectProperties(GameObject* object) {
    Transform& transform = object->transform;

    // Display and edit position
    glm::vec3 position = transform.GetLocalPosition();
    if (ImGui::DragFloat3("Position", &position[0])) {
        transform.SetLocalPosition(position);
    }

    // Display and edit rotation
    glm::vec3 rotation = transform.GetLocalRotation();
    if (ImGui::DragFloat3("Rotation", &rotation[0])) {
        transform.SetLocalRotation(rotation);
    }

    // Display and edit scale
    glm::vec3 scale = transform.GetLocalScale();
    if (ImGui::DragFloat3("Scale", &scale[0])) {
        transform.SetLocalScale(scale);
    }
}

void Properties::Draw(GameObject* selectedObject, Scene& scene) {
	ImGui::Begin("Properties");
	ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

	if (selectedObject) {
        char name[128];
        strncpy_s(name, selectedObject->name.c_str(), sizeof(name));
        if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
            selectedObject->name = name;
        }

        Camera* camera = dynamic_cast<Camera*>(selectedObject);

        if (camera) {
            CameraProperties(camera);
        }
        else {
            // I'M NOT PROUD OF THIS SOLUTION, BUT OTHER METHODS CONFLICTED WITH RENDERING :(
            if (selectedObject->name == "Workspace") {
                ImGui::Checkbox("Wireframe Mode", &scene.wireframe);
                //ImGui::DragFloat3("Light Position", &scene.lightPosition[0]);
                //ImGui::DragFloat("Near", &scene.nearPlane);
                //ImGui::DragFloat("Far", &scene.farPlane);
                //ImGui::DragFloat("Ortho Size", &scene.orthoSize);
                ImGui::DragFloat3("Light Direction", &scene.lightDirection[0]);
            }

            ObjectProperties(selectedObject);
        }
	}

	ImGui::End();
}