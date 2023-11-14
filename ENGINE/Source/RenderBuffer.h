#pragma once

class RBO {
public:
	RBO(int width, int height);
	~RBO();

	unsigned int GetRBO();
	void Bind();
	void UnBind();
private:
	unsigned int rbo;
};