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
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // Normal attribute
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    vao->Unbind(); // Unbind the VAO to prevent accidental modifications.
}

Mesh::~Mesh() {
    delete vbo;
    delete ebo;
    delete vao;
}

Mesh Mesh::GenerateCube() {
    std::vector<float> vertices = {
        // Positions         
        -0.5f, -0.5f, -0.5f, // 0. left bottom back
         0.5f, -0.5f, -0.5f, // 1. right bottom back
        -0.5f,  0.5f, -0.5f, // 2. left top back
         0.5f,  0.5f, -0.5f, // 3. right top back
        -0.5f, -0.5f,  0.5f, // 4. left bottom front
         0.5f, -0.5f,  0.5f, // 5. right bottom front
        -0.5f,  0.5f,  0.5f, // 6. left top front
         0.5f,  0.5f,  0.5f  // 7. right top front
    };

    std::vector<unsigned int> indices = {
        // Front face
        4, 5, 6,
        5, 7, 6,

        // Back face
        0, 2, 1,
        1, 2, 3,

        // Left face
        0, 4, 2,
        2, 4, 6,

        // Right face
        1, 3, 5,
        3, 7, 5,

        // Top face
        2, 6, 3,
        3, 6, 7,

        // Bottom face
        0, 1, 4,
        1, 5, 4
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
