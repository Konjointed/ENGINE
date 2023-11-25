#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
	Renderer();
	~Renderer();

	void Render();
private:
	void Resize();
	void ResetRenderState();
};

#endif 