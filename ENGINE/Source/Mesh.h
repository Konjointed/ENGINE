#pragma once

#include <vector>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>

class VAO;
class VBO;
class EBO;

class Mesh {
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	static Mesh GenerateCube();
	static Mesh GeneratePlane();

	void Draw();

	const glm::mat4& GetModelMatrix() const;

	//void SetPosition(const glm::vec3& position);
	//void SetRotation(const glm::vec3& rotation);
	//void SetScale(const glm::vec3& scale);
private:
	VAO* vao;
	VBO* vbo;
	EBO* ebo;
	unsigned int indexCount;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};