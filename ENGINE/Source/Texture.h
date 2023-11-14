#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/noise.hpp>

class Texture {
public:
    Texture() {}
    Texture(const std::string& path);
    Texture(const std::vector<std::vector<float>>& noiseMap);
    Texture(int width, int height);
    Texture(int width, int height, GLenum format, GLenum internalFormat, GLenum type);
    ~Texture();

    void Update(const std::vector<std::vector<float>>& noiseMap);
    void Bind(unsigned int unit = 0) const;
    static void Unbind(unsigned int unit = 0);
    unsigned int GetID();
private:
    unsigned int textureID;
};