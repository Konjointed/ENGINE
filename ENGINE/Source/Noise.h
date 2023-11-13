#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include <limits>

#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/noise.hpp> // glm::perlin

class Noise {
public:
    static std::vector<std::vector<float>> GenerateNoiseMap(int mapWidth, int mapHeight, float scale, int seed, int octaves, float persistence, float lacunarity, glm::vec2 offset) {
        std::vector<std::vector<float>> noiseMap(mapWidth, std::vector<float>(mapHeight));

        std::mt19937 prng(seed);
        std::uniform_int_distribution<int> distribution(-100000, 100000);
        std::vector<glm::vec2> octaveOffsets(octaves);
        for (int i = 0; i < octaves; i++) {
            float offsetX = distribution(prng) + offset.x;
            float offsetY = distribution(prng) + offset.y;
            octaveOffsets[i] = glm::vec2(offsetX, offsetY);
        }

        if (scale <= 0) {
            scale = 0.0001f;
        }

        float maxNoiseHeight = std::numeric_limits<float>::lowest();
        float minNoiseHeight = std::numeric_limits<float>::max();

        float halfWidth = mapWidth / 2.0f;
        float halfHeight = mapHeight / 2.0f;

        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {

                float amplitude = 1;
                float frequency = 1;
                float noiseHeight = 0;

                for (int i = 0; i < octaves; i++) {
                    float sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
                    float sampleY = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;

                    // glm::perlin returns values in the range of [-1, 1]
                    float perlinValue = glm::perlin(glm::vec2(sampleX, sampleY)) * 2 - 1;
                    noiseHeight += perlinValue * amplitude;

                    amplitude *= persistence;
                    frequency *= lacunarity;
                }

                maxNoiseHeight = std::max(maxNoiseHeight, noiseHeight);
                minNoiseHeight = std::min(minNoiseHeight, noiseHeight);

                noiseMap[x][y] = noiseHeight;
            }
        }

        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                noiseMap[x][y] = InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x][y]);
            }
        }

        return noiseMap;
    }
private:
    static float InverseLerp(float a, float b, float value) {
        return (value - a) / (b - a);
    }
};