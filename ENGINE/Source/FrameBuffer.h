#pragma once

#include "Texture.h"

class FBO {
public:
    FBO(int width, int height, bool depthMap = false);
    ~FBO();

    void Bind();
    void UnBind();
    TextureOLD& GetColorTexture();
    TextureOLD& GetDepthTexture();
private:
    unsigned int fbo;
    unsigned int rbo;
    TextureOLD* colorTexture;
    TextureOLD* depthTexture;
};