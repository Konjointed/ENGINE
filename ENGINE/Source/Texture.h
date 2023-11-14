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
    void Bind(unsigned int unit = 0) const;
    static void Unbind(unsigned int unit = 0);

private:
    unsigned int textureID;
};