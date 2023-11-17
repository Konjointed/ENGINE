#include "RenderBuffer.h"

#include <iostream>

#include <glad/glad.h>

RBO::RBO(int width, int height) {
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

RBO::~RBO() {
    glDeleteRenderbuffers(1, &rbo);
}

void RBO::Bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void RBO::Unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint RBO::GetID() const {
    return rbo;
}