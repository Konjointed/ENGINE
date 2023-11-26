#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>

#include "Model.h"

class SceneElements;

//class Model;

class Transform {
public:
	void ComputeModelMatrix() {
		modelMatrix = GetLocalModelMatrix();
		isDirty = false;
	}

	void ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix) {
		modelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
		isDirty = false;
	}

	void SetLocalPosition(const glm::vec3& newPosition) {
		position = newPosition;
		isDirty = true;
	}

	void SetLocalRotation(const glm::vec3& newRotation) {
		eulerRotation = newRotation;
		isDirty = true;
	}

	void SetLocalScale(const glm::vec3& newScale) {
		scale = newScale;
		isDirty = true;
	}

	const glm::vec3& GetGlobalPosition() const { return modelMatrix[3]; }
	const glm::vec3& GetLocalPosition() const { return position; }
	const glm::vec3& GetLocalRotation() const { return eulerRotation; }
	const glm::vec3& GetLocalScale() const { return scale; }
	const glm::mat4& GetModelMatrix() const { return modelMatrix; }
	glm::vec3 GetRight() const { return modelMatrix[0]; }
	glm::vec3 GetUp() const { return modelMatrix[1]; }
	glm::vec3 GetBackward() const { return modelMatrix[2]; }
	glm::vec3 GetForward() const{ return -modelMatrix[2]; }
	glm::vec3 GetGlobalScale() const { return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) }; }
	bool IsDirty() const { return isDirty; }

protected:
	// Local space information
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 eulerRotation = { 0.0f, 0.0f, 0.0f }; // Degrees
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	// Global space information concatenate in matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	bool isDirty = true;

	glm::mat4 GetLocalModelMatrix() {
		const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		// Y * X * Z
		const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

		// translation * rotation * scale (also know as TRS matrix)
		return glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
	}
};

class  GameObject {
public:
	static SceneElements* scene;

	// Scene graph
	std::list<std::unique_ptr<GameObject>> children;
	GameObject* parent = nullptr;

	// Space information
	Transform transform;

	Model* model = nullptr;
	std::string name;

	GameObject() : name("Unnamed GameObject") {}
	GameObject(Model& model) : model(&model), name("Unnamed GameObject") {}

	template<typename... TArgs>
	void AddChild(TArgs&... args) {
		children.emplace_back(std::make_unique<GameObject>(args...));
		children.back()->parent = this;
	}

	// Add an existing gameobject
	// scenegraph.AddChild(std::move(playerObject));
	void AddChild(GameObject child) {
		children.emplace_back(std::make_unique<GameObject>(std::move(child)));
		children.back()->parent = this;
	}

	/*
	void SetName(const std::string& newName) {
		name = newName;
	}

	const std::string& GetName() const {
		return name;
	}
	*/

	void UpdateSelfAndChild() {
		if (transform.IsDirty()) {
			ForceUpdateSelfAndChild();
			return;
		}

		for (auto&& child : children)
		{
			child->UpdateSelfAndChild();
		}
	}

	void ForceUpdateSelfAndChild() {
		if (parent) {
			transform.ComputeModelMatrix(parent->transform.GetModelMatrix());
		}
		else {
			transform.ComputeModelMatrix();
		}

		for (auto&& child : children) {
			child->ForceUpdateSelfAndChild();
		}
	}

	void DrawSelfAndChild(Shader shader) {
		// If this GameObject has a model, draw it
		if (model != nullptr) {
			// Set the transformation matrix for this object
			shader.SetMatrix4("model", transform.GetModelMatrix());

			// Draw the model
			model->Draw(shader);
		}

		// Draw all children
		for (auto&& child : children) {
			child->DrawSelfAndChild(shader);
		}
	}
private:
};

#endif 