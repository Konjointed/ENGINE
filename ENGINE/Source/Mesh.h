#pragma once

#include <vector>
#include <string>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>

class Mesh {
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, int layout);
	~Mesh();

	void Draw();

	const glm::mat4& GetModelMatrix() const;
	glm::vec3 GetPosition();

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

	static Mesh GenerateCube();
	static Mesh GeneratePlane();
	static Mesh GenerateQuad();
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int indexCount;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};
