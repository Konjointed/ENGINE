#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include <glm/mat4x4.hpp>

class Skybox {
public:
	Skybox();
	~Skybox();

	void Draw(glm::mat4 view, glm::mat4 projection);
private:
	unsigned int vao;
	unsigned int vbo;

	std::vector<float> vertices;
};

#endif 