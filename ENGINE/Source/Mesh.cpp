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
    GLsizei stride = 5 * sizeof(float);

    // Set the vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
        // Positions        // Texture Coords

        // Front face (4, 5, 6, 7)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4. left bottom front
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5. right bottom front
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 6. left top front
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 7. right top front

         // Back face (0, 1, 2, 3)
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0. left bottom back
          0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1. right bottom back
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 2. left top back
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 3. right top back

          // Left face (8, 9, 10, 11)
          -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 8. left bottom back
          -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 9. left bottom front
          -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 10. left top back
          -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 11. left top front

          // Right face (12, 13, 14, 15)
           0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 12. right bottom back
           0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 13. right bottom front
           0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 14. right top back
           0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 15. right top front

           // Top face (16, 17, 18, 19)
           -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 16. left top back
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 17. right top back
           -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 18. left top front
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 19. right top front

            // Bottom face (20, 21, 22, 23)
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 20. left bottom back
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 21. right bottom back
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 22. left bottom front
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 23. right bottom front
    };

    std::vector<unsigned int> indices = {
        // Front face
        4, 5, 6,
        5, 7, 6,

        // Back face
        8, 10, 9,
        9, 10, 11,

        // Left face
        12, 14, 13,
        13, 14, 15,

        // Right face
        16, 18, 17,
        17, 18, 19,

        // Top face
        20, 22, 21,
        21, 22, 23,

        // Bottom face
        0, 1, 2,
        1, 3, 2
    };

    return Mesh(vertices, indices);
}

Mesh Mesh::GeneratePlane() {
    std::vector<float> vertices = {
        // Positions         // Texture Coords
        -0.5f, 0.0f, -0.5f,   0.0f, 0.0f, // 0. left back
         0.5f, 0.0f, -0.5f,   1.0f, 0.0f, // 1. right back
        -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, // 2. left front
         0.5f, 0.0f,  0.5f,   1.0f, 1.0f  // 3. right front
    };

    std::vector<unsigned int> indices = {
        // Two triangles forming the plane
        0, 1, 2, // First triangle
        1, 3, 2  // Second triangle
    };

    return Mesh(vertices, indices);
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
