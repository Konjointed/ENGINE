#pragma once

#include <vector>
#include <memory>

class RenderPipeline;

class Renderer {
public:
	Renderer();

	void Render();
	void SetPipeline(RenderPipeline* pipeline);

	~Renderer();
private:
	//std::vector<std::unique_ptr<RenderPipeline>> renderPipelines;
	RenderPipeline* activePipeline;
};