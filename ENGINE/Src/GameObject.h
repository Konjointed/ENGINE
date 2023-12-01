#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include <memory>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>

#include "Model.h"

//class Model;

enum ClassID {
	CLASS_ID_GAMEOBJECT,
	CLASS_ID_CAMERA,    
};

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

class  GameObject : public std::enable_shared_from_this<GameObject> {
public:
	// Scene graph
	std::list<std::unique_ptr<GameObject>> children;
	GameObject* parent = nullptr;

	// Space information
	Transform transform;

	ClassID classID;
	std::string name;
	std::shared_ptr<Model> model = nullptr;

	GameObject() : classID(CLASS_ID_GAMEOBJECT), name("Unnamed GameObject") { 
		std::cout << "Constructing GameObject\n"; 
	}

	GameObject(std::shared_ptr<Model> model)
		: model(model), classID(CLASS_ID_GAMEOBJECT), name("Unnamed GameObject") {
		std::cout << "Constructing GameObject\n";
	}

	template<typename... TArgs>
	void AddChild(TArgs&... args) {
		children.emplace_back(std::make_unique<GameObject>(args...));
		children.back()->parent = this;
	}

	virtual void UpdateSelfAndChild() {
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

	virtual void DrawSelfAndChild(Shader shader) {
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
};

#endif 