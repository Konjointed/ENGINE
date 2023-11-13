#pragma once

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/noise.hpp>

class Texture {
public:
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

    Texture(const std::vector<std::vector<float>>& noiseMap) {
        int width = noiseMap.size();
        int height = noiseMap[0].size();

        // Convert the noise map to an array of unsigned char (grayscale values)
        std::vector<unsigned char> pixels;
        for (const auto& row : noiseMap) {
            for (float value : row) {
                pixels.push_back(static_cast<unsigned char>(value * 255));
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

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