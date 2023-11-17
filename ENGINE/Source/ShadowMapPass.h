#pragma once

class ShadowMapPass {
public:
	ShadowMapPass() {}

	void Execute() {}

	unsigned int GetDepthTexture() {}
private:
	FBO shadowMapFBO;
};