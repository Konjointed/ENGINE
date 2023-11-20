#pragma once

#include <string>
#include <vector>

class Texture {
public:
	static unsigned int FromFile(std::string path, std::string directory, bool gamma = false);
	static unsigned int LoadCubemap(std::vector<std::string> faces);
private:
};
