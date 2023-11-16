#pragma once

#include <unordered_map>
#include <string>

struct Texture;

class ResourceManager {
public:
	static Texture* LoadTextureFromFile(const std::string path);
	static Texture* LoadCubemap(std::vector<std::string> faces);
	//static Texture* GetTexture(const std::string path);
	//static void UnloadTexture(const std::string path);
private:
	std::unordered_map<std::string, Texture*> textures;

	static int LoadTextureFromFileInternal(const std::string path);
	static int LoadCubemapInternal(std::vector<std::string> faces);
};