#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include <glm/mat4x4.hpp>

class Window;
class Shader;
class GameObject;
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

	unsigned int shadowMapFramebuffer;
	unsigned int shadowMapTexture;
};

#endif 