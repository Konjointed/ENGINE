#pragma once

#include <string>
#include <vector>

class Texture {
public:
	unsigned int ID;
	std::string type;
	std::string path;

	static unsigned int FromFile(std::string path, std::string directory, bool gamma = false);
	static unsigned int LoadCubemap(std::vector<std::string> faces);
	// GenerateTexture2D(w, h)
	// GenerateTexture3D(w, h, d)
private:
};
