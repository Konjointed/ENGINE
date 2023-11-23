#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

class Shader;

class Skybox {
public:
	Skybox();
	~Skybox();

	void Draw();
private:
	unsigned int vao;
	unsigned int vbo;

	std::vector<float> vertices;
	Shader* shader;
	unsigned int texture;
};

#endif 