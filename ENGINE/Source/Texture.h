#pragma once

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/noise.hpp>

class Texture {
public:
    // Constructor
    Texture(const std::string& path) {
        // Load the image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cerr << "Failed to load texture" << std::endl;
            return;
        }

        // Generate and bind the texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture to the GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free the image memory
        stbi_image_free(data);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture(int width, int height) {
        // Generate Perlin noise data
        std::vector<unsigned char> data(width * height * 3); // 3 for RGB
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                float noise = glm::perlin(glm::vec2(x, y) * 0.1f) * 0.5f + 0.5f; // Adjust scale and bias
                unsigned char value = static_cast<unsigned char>(noise * 255.0f);
                data[(y * width + x) * 3 + 0] = value; // R
                data[(y * width + x) * 3 + 1] = value; // G
                data[(y * width + x) * 3 + 2] = value; // B
            }
        }

        // Generate and bind the texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture to the GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Destructor
    ~Texture() {
        glDeleteTextures(1, &textureID);
    }

    // Bind the texture
    void Bind() const {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    // Unbind the texture
    static void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    unsigned int textureID;
};