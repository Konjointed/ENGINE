#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include "DrawableObject.h"

class Shader;

class Skybox : public DrawableObject {
public:
	Skybox();
	~Skybox();

	void Draw() override;
private:
	unsigned int vao;
	unsigned int vbo;

	std::vector<float> vertices;
	Shader* shader;
	unsigned int texture;
};

#endif 