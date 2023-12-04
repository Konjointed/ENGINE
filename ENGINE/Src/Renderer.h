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
	Renderer(Window& window, Scene& scene);
	~Renderer();

	void Render(Window& window, Scene& scene);
	void Resize();

	glm::mat4 getLightSpaceMatrix(Scene& scene, const float nearPlane, const float farPlane);
	std::vector<glm::mat4> getLightSpaceMatrices(Scene& scene);
	unsigned int GetTextureColorBuffer();
private:
	unsigned int framebuffer;
	unsigned int textureColorBuffer;
	unsigned int renderbuffer;

	unsigned int lightFBO;
	unsigned int lightDepthMaps;
	unsigned int depthMapResolution = 4096;

	unsigned int matricesUBO;

	std::vector<float> shadowCascadeLevels;
};

#endif 