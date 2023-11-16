#include "FrameBuffer.h"

#include <iostream>

#include <glad/glad.h>

FBO::FBO(int width, int height, bool depthMap) {
    // Generate framebuffer
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    if (depthMap) {
        // Create depth texture for shadow mapping using Texture class
        this->depthTexture = new TextureOLD(width, height); // Assuming Texture class handles depth textures
        glBindTexture(GL_TEXTURE_2D, this->depthTexture->GetID());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture->GetID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    else {
        // Create color texture using Texture class
        this->colorTexture = new TextureOLD(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
        glBindTexture(GL_TEXTURE_2D, this->colorTexture->GetID());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTexture->GetID(), 0);

        // Generate render buffer for depth and stencil
        glGenRenderbuffers(1, &this->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
    }

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    // Unbind framebuffer to prevent accidental changes
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO() {
	glDeleteFramebuffers(1, &this->fbo);
    glDeleteRenderbuffers(1, &this->rbo);
}

void FBO::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void FBO::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureOLD& FBO::GetColorTexture() { return *colorTexture; }

TextureOLD& FBO::GetDepthTexture() { return *depthTexture; }