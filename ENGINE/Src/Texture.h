#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>

class Texture {
public:
	unsigned int id;
	std::string type;
	std::string path;

	static Texture FromFile(std::string path, bool gamma);
	static Texture LoadCubemap(std::vector<std::string> faces);
};

#endif