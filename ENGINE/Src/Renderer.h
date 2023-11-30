#ifndef RENDERER_H
#define RENDERER_H

class Window;
class Animator;
class Skybox;
class GameObject;
class Camera;

class Renderer {
public:
	Renderer(Window& window);
	~Renderer();

	void Render(Window& window, Camera& camera, Animator& animator, Skybox& skybox, GameObject& scenegraph);
	void Resize();

	unsigned int GetTextureColorBuffer();
private:
	unsigned int framebuffer;
	unsigned int textureColorBuffer;
	unsigned int renderbuffer;
};

#endif 