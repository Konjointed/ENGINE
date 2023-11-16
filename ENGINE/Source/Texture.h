#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/noise.hpp>

class TextureOLD {
public:
    TextureOLD() {}
    TextureOLD(const std::string& path);
    TextureOLD(const std::vector<std::vector<float>>& noiseMap);
    TextureOLD(int width, int height);
    TextureOLD(int width, int height, GLenum format, GLenum internalFormat, GLenum type);
    ~TextureOLD();

    void Update(const std::vector<std::vector<float>>& noiseMap);
    void Bind(unsigned int unit = 0) const;
    static void Unbind(unsigned int unit = 0);
    unsigned int GetID();
private:
    unsigned int textureID;
};