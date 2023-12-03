#pragma once

#include <vector>
#include <string>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>

#define MAX_BONE_INFLUENCE 4

class Shader;
class Texture;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 color;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	//bone indexes which will influence this vertex
	int boneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float weights[MAX_BONE_INFLUENCE];
};

/*
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};
*/

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();

	void Draw(Shader& shader, unsigned int shadowMapTexture = 0);
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
};
