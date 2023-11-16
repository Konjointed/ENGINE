#pragma once

#include <vector>
#include <string>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>

class VAO;
class VBO;
class EBO;
class Shader;

enum TEXTURE_TYPE {
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT
};

struct Texture {
	int ID;
	TEXTURE_TYPE type;
	std::string path;
};

class Mesh {
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, int layout);
	~Mesh();

	static Mesh GenerateCube();
	static Mesh GeneratePlane();
	static Mesh GenerateQuad();
	static Mesh GenerateArrow();

	void Draw(Shader& shader);

	void AddTexture(Texture* texture);

	const glm::mat4& GetModelMatrix() const;
	glm::vec3 GetPosition();

	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);
private:
	VAO* vao;
	VBO* vbo;
	EBO* ebo;
	unsigned int indexCount;

	std::vector<Texture> textures;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};