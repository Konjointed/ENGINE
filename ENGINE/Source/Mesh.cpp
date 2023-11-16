#include "Mesh.h"

#include <iostream>

#include <glad/glad.h>

#include <Shader.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, int layout) :
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

    // Determine stride (gap between vertices)
    GLsizei stride;
    switch (layout) {
    case 0: // Position and Texture
        stride = 4 * sizeof(float); // 2 for position, 2 for texture
        // Set attribute pointers for position and texture
        break;
    case 1: // Position, Texture, and Normals
        stride = 8 * sizeof(float); // 3 for position, 2 for texture, 3 for normal
        // Set attribute pointers for position, texture, and normal
        break;
    }

    // Set the vertex attribute pointers
    if (layout == 0) {
        // Set up attribute pointers for position and texture
        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (layout == 1) {
        // Set up attribute pointers for position, texture, and normal
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    vao->Unbind(); // Unbind the VAO to prevent accidental modifications.
}

Mesh::~Mesh() {
    std::cout << "Mesh destroyed\n";
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

    return Mesh(vertices, indices, 1);
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

    return Mesh(vertices, indices, 1);
}

Mesh Mesh::GenerateQuad() {
    std::vector<float> vertices = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f, // Top-left
        -1.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f,  1.0f, 0.0f, // Bottom-right
         1.0f,  1.0f,  1.0f, 1.0f  // Top-right
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle (top-left, bottom-left, bottom-right)
        0, 2, 3  // Second triangle (top-left, bottom-right, top-right)
    };

    return Mesh(vertices, indices, 0);
}

Mesh Mesh::GenerateArrow() {
    // Arrow shaft vertices
    std::vector<float> vertices = {
        // Positions         // Texture Coords // Normals
        // Shaft (line) - not actually used in lighting, but you can use this if needed
        0.0f, 0.0f, 0.0f,     0.0f, 0.0f,     0.0f, 0.0f, 1.0f, // Shaft start
        0.0f, 0.0f, 1.0f,     1.0f, 0.0f,     0.0f, 0.0f, 1.0f, // Shaft end

        // Arrowhead (pyramid)
        0.0f, 0.0f, 1.0f,     0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // Top of the pyramid, and end of the shaft
        -0.1f, 0.1f, 0.8f,    0.0f, 1.0f,     -1.0f, 1.0f, 0.0f, // Bottom left of the pyramid
        0.1f, 0.1f, 0.8f,     1.0f, 1.0f,     1.0f, 1.0f, 0.0f,  // Bottom right of the pyramid
        -0.1f, -0.1f, 0.8f,   0.0f, 1.0f,     -1.0f, -1.0f, 0.0f,// Top left of the pyramid
        0.1f, -0.1f, 0.8f,    1.0f, 1.0f,     1.0f, -1.0f, 0.0f  // Top right of the pyramid
    };

    std::vector<unsigned int> indices = {
        // Line indices (shaft)
        0, 1, // This will only be visible if you draw GL_LINES

        // Pyramid indices (arrowhead)
        2, 3, 4, // Front face
        2, 4, 5, // Right face
        2, 5, 6, // Back face
        2, 6, 3  // Left face
    };

    return Mesh(vertices, indices, 1);
}

void Mesh::Draw(Shader& shader) {
    glm::mat4 model = GetModelMatrix();
    shader.setMat4("model", model);

    // Bind Textures
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit
        shader.setInt("diffuseTexture", 0);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID); // Bind the texture
    }

    // Draw Mesh
    vao->Bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    vao->Unbind();
}

void Mesh::AddTexture(Texture* texture) {
    textures.push_back(*texture);
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
