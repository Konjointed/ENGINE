#pragma once

class FBO {
public:
    unsigned int textureColorbuffer;

    FBO(int width, int height);
    ~FBO();

    void Bind();
    void UnBind();
private:
    unsigned int fbo;
    unsigned int rbo;
};