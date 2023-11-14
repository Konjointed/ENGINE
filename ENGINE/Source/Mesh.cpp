#include "Mesh.h"

#include <glad/glad.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) :
    indexCount(indices.size()),
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f), 
    scale(1.0f, 1.0f, 1.0f) 
{
    // Create and bind the VAO
    vao = new VAO();
    vao->Bind();

    // Create the VBO with the vertex data
    vbo = new VBO(vertices);

    // Create the EBO and upload the index data
    ebo = new EBO(indices.data(), indices.size() * sizeof(unsigned int));

    // Assuming the stride (gap between vertices) is 8 floats (3 for position, 2 for texture, 3 for normal)
    GLsizei stride = 8 * sizeof(float);

    // Set the vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    vao->Unbind(); // Unbind the VAO to prevent accidental modifications.
}

Mesh::~Mesh() {
    delete vbo;
    delete ebo;
    delete vao;
}

Mesh Mesh::GenerateCube() {
    std::vector<float> vertices = {
        // Positions        // Texture Coords // Normals

        // Front face (positive Z)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

        // Back face (negative Z)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

        // Left face (negative X)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

        // Right face (positive X)
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,

        // Top face (positive Y)
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

        // Bottom face (negative Y)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 1, 3, 2,

        // Back face
        4, 6, 5, 6, 7, 5,

        // Left face
        8, 10, 9, 10, 11, 9,

        // Right face
        12, 13, 14, 13, 15, 14,

        // Top face
        16, 17, 18, 17, 19, 18,

        // Bottom face
        20, 22, 21, 22, 23, 21
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::GeneratePlane() {
    std::vector<float> vertices = {
        // Positions         // Texture Coords // Normals
        -0.5f, 0.0f, -0.5f,   0.0f, 0.0f,    0.0f, 1.0f, 0.0f, // 0. left back
         0.5f, 0.0f, -0.5f,   1.0f, 0.0f,    0.0f, 1.0f, 0.0f, // 1. right back
        -0.5f, 0.0f,  0.5f,   0.0f, 1.0f,    0.0f, 1.0f, 0.0f, // 2. left front
         0.5f, 0.0f,  0.5f,   1.0f, 1.0f,    0.0f, 1.0f, 0.0f  // 3. right front
    };

    std::vector<unsigned int> indices = {
        // Two triangles forming the plane
        0, 1, 2, // First triangle
        1, 3, 2  // Second triangle
    };

    return Mesh(vertices, indices);
}

const glm::mat4& Mesh::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}

void Mesh::Draw() {
    vao->Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}

glm::vec3 Mesh::GetPosition() {
    return position;
}

void Mesh::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Mesh::SetRotation(const glm::vec3& newRotation) {
    rotation = newRotation;
}

void Mesh::SetScale(const glm::vec3& newScale) {
    scale = newScale;
}
