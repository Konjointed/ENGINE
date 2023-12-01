#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

class Window;
struct Scene;

class Renderer {
public:
	Renderer(Window& window);
	~Renderer();

	void Render(Window& window, Scene& scene);
	void Resize();

	unsigned int GetTextureColorBuffer();
private:
	unsigned int framebuffer;
	unsigned int textureColorBuffer;
	unsigned int renderbuffer;
};

#endif 