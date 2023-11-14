#include "RenderBuffer.h"

#include <iostream>

#include <glad/glad.h>

RBO::RBO(int width, int height) {
    glGenRenderbuffers(1, &this->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);

    // Allocate storage for the renderbuffer
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // You can unbind the renderbuffer here as a good practice
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RBO::~RBO() {
    glDeleteRenderbuffers(1, &this->rbo);
}

unsigned int RBO::GetRBO() {
    return this->rbo;
}

void RBO::Bind() {
	glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
}

void RBO::UnBind() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}