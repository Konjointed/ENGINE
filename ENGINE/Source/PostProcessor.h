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

	std::vector<float> vertices;
};

#endif