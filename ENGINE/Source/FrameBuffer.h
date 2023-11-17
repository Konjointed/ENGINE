#pragma once

#include <glad/glad.h>

class FBO {
public:
    unsigned int depthTextureID = 0;
    unsigned int colorTextureID = 0;

    FBO(int width, int height) : width(width), height(height) {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void AddDepthStencilRenderbuffer() {
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    void AddDepthTexture() {
        // Create depth texture
        glGenTextures(1, &depthTextureID);
        glBindTexture(GL_TEXTURE_2D, depthTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // Attach depth texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    void AddColorTexture(GLenum internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE) {
        // Create color texture
        glGenTextures(1, &colorTextureID);
        glBindTexture(GL_TEXTURE_2D, colorTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach color texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);
    }

    void BindTexture(unsigned int textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        if (depthTextureID != 0) {
            glBindTexture(GL_TEXTURE_2D, depthTextureID);
        }
        else if (colorTextureID != 0) {
            glBindTexture(GL_TEXTURE_2D, colorTextureID);
        }
    }

    void Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
    }

    void Finalize() {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~FBO() {
        glDeleteFramebuffers(1, &fbo);
    }

private:
    unsigned int fbo;
    unsigned int rbo;
    int width, height;
};
