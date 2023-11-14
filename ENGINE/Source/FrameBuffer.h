#pragma once

class FBO {
public:
    unsigned int texture;

    FBO(int width, int height, bool depthMap = false);
    ~FBO();

    void Bind();
    void UnBind();
private:
    unsigned int fbo;
    unsigned int rbo;
};