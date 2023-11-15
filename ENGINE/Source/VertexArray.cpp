#include "VertexArray.h"

#include <iostream>

#include <glad/glad.h>

VAO::VAO() {
    glGenVertexArrays(1, &this->vao);
}

VAO::~VAO() {
    std::cout << "VAO destroyed\n";
    glDeleteVertexArrays(1, &this->vao);
}

void VAO::Bind() const {
    glBindVertexArray(this->vao);
}

void VAO::Unbind() const {
    glBindVertexArray(0);
}