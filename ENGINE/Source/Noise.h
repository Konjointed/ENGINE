#pragma once

#include <vector>
#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/noise.hpp> // glm::perlin

class Noise {
public:
    static std::vector<std::vector<float>> GenerateNoiseMap(int mapWidth, int mapHeight, float scale) {
        std::vector<std::vector<float>> noiseMap(mapWidth, std::vector<float>(mapHeight));

        if (scale <= 0) {
            scale = 0.0001f;
        }

        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                float sampleX = x / scale;
                float sampleY = y / scale;

                // glm::perlin returns values in the range of [-1, 1], so we need to normalize it to [0, 1]
                float perlinValue = glm::perlin(glm::vec2(sampleX, sampleY)) * 0.5f + 0.5f;
                noiseMap[x][y] = perlinValue;
            }
        }

        return noiseMap;
    }
};