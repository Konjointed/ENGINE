#pragma once

#include <glad/glad.h>

class RBO {
public:
    RBO(int width, int height);
    ~RBO();

    void Bind();
    void Unbind();
    GLuint GetID() const;

private:
    GLuint rbo;
};