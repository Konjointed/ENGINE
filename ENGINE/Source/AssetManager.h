#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <map>

class Shader;
class Texture;
class Model;

class AssetManager {
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture> textures;
	static std::map<std::string, Model> models;

	static void Clear();

	static Shader LoadShader(const std::string vsPath, const std::string fsPath, const std::string name);
	static Shader GetShader(const std::string name);

	static Texture LoadTexture(const std::string path, const std::string name);
	static Texture GetTexture(const std::string name);

	static Model LoadModel(const std::string path, const std::string name);
	static Model GetModel(const std::string name);
private:
	AssetManager() {}
};

#endif 