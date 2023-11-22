#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

class PostProcessor {
public:
	PostProcessor();
	~PostProcessor();

	void Draw();
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

#endif