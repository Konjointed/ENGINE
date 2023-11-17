#pragma once

#include <vector>
#include <memory>

class RenderPipeline;

class Renderer {
public:
	Renderer();

	void Render();

	~Renderer();
private:
	std::vector<std::unique_ptr<RenderPipeline>> renderPipelines;
};