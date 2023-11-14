#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/noise.hpp>

class Texture {
public:
    Texture(const std::string& path);
    Texture(const std::vector<std::vector<float>>& noiseMap);
    ~Texture();

    void Update(const std::vector<std::vector<float>>& noiseMap);
    void Bind() const;
    static void Unbind();

private:
    unsigned int textureID;
};