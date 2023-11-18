#pragma once

class RenderPass {
public:
	virtual ~RenderPass() {}
	virtual void Execute() = 0;
	virtual void SetSceneObjects(std::vector<Mesh*>& objects) {}
};
