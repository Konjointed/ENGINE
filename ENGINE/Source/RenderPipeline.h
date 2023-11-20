#pragma once

#include <vector>

#include "RenderPass.h"

class RenderPipeline {
public:
	void AddPass(RenderPass* pass) {
		passes.push_back(pass);
	}

	void Execute() {
		for (auto& pass : passes) {
			pass->Execute();
		}
	}
private:
	std::vector<RenderPass*> passes;
};