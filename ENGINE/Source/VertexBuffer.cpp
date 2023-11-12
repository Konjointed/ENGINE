#include "VertexBuffer.h"

#include <glad/glad.h>

//VBO::VBO() : vbo(0) {}

VBO::VBO(const std::vector<float>& vertices) : vbo(0) {
    glGenBuffers(1, &this->vbo); // Generate buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo); // Bind the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); // Upload data to the buffer
}

VBO::~VBO() {
    glDeleteBuffers(1, &this->vbo);
}

void VBO::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
}

void VBO::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}