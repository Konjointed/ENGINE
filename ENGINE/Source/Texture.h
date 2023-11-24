#pragma once

#include <string>
#include <vector>

class Texture {
public:
	unsigned int ID;
	std::string type;
	std::string path;

	static Texture FromFile(std::string path, bool gamma = false);
	static Texture LoadCubemap(std::vector<std::string> faces);
	// GenerateTexture2D(w, h)
	// GenerateTexture3D(w, h, d)
private:
};
