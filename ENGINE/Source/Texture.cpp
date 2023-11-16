#include "Texture.h"

#include <stb_image.h>

TextureOLD::TextureOLD(const std::string& path) {
    // Load the image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // This line is added if your texture coordinates are flipped vertically.
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set the unpack alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, nrChannels == 4 ? 4 : 1); // Adjusted for the number of channels

    // Determine the format
    GLenum format = GL_RGB;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    // Upload the texture to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image memory
    stbi_image_free(data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureOLD::TextureOLD(const std::vector<std::vector<float>>& noiseMap) {
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

TextureOLD::TextureOLD(int width, int height) {
    // Generate the texture
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    // Set texture parameters for a depth texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Allocate the depth texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureOLD::TextureOLD(int width, int height, GLenum format, GLenum internalFormat, GLenum type) {
    // Generate the texture
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Allocate the texture (with no data)
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureOLD::~TextureOLD() {
    //std::cout << "Texture Destroyed\n";
    glDeleteTextures(1, &textureID);
}

void TextureOLD::Update(const std::vector<std::vector<float>>& noiseMap) {
    int width = noiseMap.size();
    int height = noiseMap[0].size();

    // Convert the noise map to an array of unsigned char (grayscale values)
    std::vector<unsigned char> pixels;
    for (const auto& row : noiseMap) {
        for (float value : row) {
            pixels.push_back(static_cast<unsigned char>(value * 255));
        }
    }

    // Bind the existing texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Update the texture with new data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureOLD::Bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void TextureOLD::Unbind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit); 
    glBindTexture(GL_TEXTURE_2D, 0);    
}

unsigned int TextureOLD::GetID() {
    return textureID;
}