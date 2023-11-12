#include "VertexArray.h"

#include <glad/glad.h>

VAO::VAO() {
    glGenVertexArrays(1, &this->vao);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &this->vao);
}

void VAO::Bind() const {
    glBindVertexArray(this->vao);
}

void VAO::Unbind() const {
    glBindVertexArray(0);
}