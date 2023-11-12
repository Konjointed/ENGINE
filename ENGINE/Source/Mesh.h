#pragma once

#include <vector>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

class VAO;
class VBO;

class Mesh {
public:
	Mesh(const std::vector<float>& vertices);
	~Mesh();

	static Mesh GenerateCube();

private:
	VAO* vao;
	VBO* vbo;
};