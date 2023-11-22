#include <iostream>

#include <Shader.h>

#include "Mesh.h"
#include "Texture.h"
#include "IncludeGL.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
    position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // create buffers/arrays
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
    // vertex color
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    std::cout << "Mesh destroyed\n";
}

void Mesh::Draw(Shader& shader) {
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
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

Mesh Mesh::GenerateCube() {
    std::vector<Vertex> vertices;

    // Front face (positive Z)
    vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) });

    // Back face (negative Z)
    vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) });

    // Left face (negative X)
    vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) });

    // Right face (positive X)
    vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) });

    // Top face (positive Y)
    vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) });

    // Bottom face (negative Y)
    vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back(Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back(Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) });

    // Define indices for each face (two triangles per face)
    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 1, 3, 2,
        // Back face
        4, 5, 6, 5, 7, 6,
        // Left face
        8, 9, 10, 9, 11, 10,
        // Right face
        12, 13, 14, 13, 15, 14,
        // Top face
        16, 17, 18, 17, 19, 18,
        // Bottom face
        20, 21, 22, 21, 23, 22
    };

    // Assuming you have a default textures vector or modify as needed
    std::vector<Texture> textures;

    return Mesh(vertices, indices, textures);
}

Mesh Mesh::GenerateQuad() {
    std::vector<Vertex> vertices;

    // Define each corner of the quad
    Vertex topLeft;
    topLeft.position = glm::vec3(-1.0f, 1.0f, 0.0f);
    topLeft.texCoords = glm::vec2(0.0f, 1.0f);
    // Set other attributes like normal, color, etc., if needed

    Vertex bottomLeft;
    bottomLeft.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    bottomLeft.texCoords = glm::vec2(0.0f, 0.0f);
    // ...

    Vertex bottomRight;
    bottomRight.position = glm::vec3(1.0f, -1.0f, 0.0f);
    bottomRight.texCoords = glm::vec2(1.0f, 0.0f);
    // ...

    Vertex topRight;
    topRight.position = glm::vec3(1.0f, 1.0f, 0.0f);
    topRight.texCoords = glm::vec2(1.0f, 1.0f);
    // ...

    // Add vertices to the vector
    vertices.push_back(topLeft);
    vertices.push_back(bottomLeft);
    vertices.push_back(bottomRight);
    vertices.push_back(topRight);

    // Indices remain the same
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    // Assuming you have a default textures vector or modify as needed
    std::vector<Texture> textures;

    return Mesh(vertices, indices, textures);
}