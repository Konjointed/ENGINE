#pragma once

#include <vector>

class Mesh;

class RenderPass {
public:
	virtual ~RenderPass() {}
	virtual void Execute() = 0;
	virtual void SetSceneObjects(std::vector<Mesh*>& objects) {}
};
