#pragma once

#include <vector>

class VBO {
public:
	//VBO();
	VBO(const std::vector<float>& vertices);
	~VBO();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int vbo;
};
