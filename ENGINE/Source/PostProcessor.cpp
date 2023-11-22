#include <vector>

#include "PostProcessor.h"
#include "IncludeGL.h"

PostProcessor::PostProcessor() {
    vertices = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f, // Top-left
        -1.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f,  1.0f, 0.0f, // Bottom-right
         1.0f,  1.0f,  1.0f, 1.0f  // Top-right
    };

    indices = {
        0, 1, 2, // First triangle (top-left, bottom-left, bottom-right)
        0, 2, 3  // Second triangle (top-left, bottom-right, top-right)
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLsizei  stride = 4 * sizeof(float);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

PostProcessor::~PostProcessor() {

}

void PostProcessor::Draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}