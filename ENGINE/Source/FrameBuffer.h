#pragma once

#include "Texture.h"

class FBO {
public:
    FBO(int width, int height, bool depthMap = false);
    ~FBO();

    void Bind();
    void UnBind();
    Texture& GetColorTexture();
    Texture& GetDepthTexture();
private:
    unsigned int fbo;
    unsigned int rbo;
    Texture* colorTexture;
    Texture* depthTexture;
};