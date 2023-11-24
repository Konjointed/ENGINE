#include "AssetManager.h"

#include <Shader.h>
#include "Texture.h"
#include "Model.h"

std::map<std::string, Texture> AssetManager::textures;
std::map<std::string, Shader> AssetManager::shaders;

void AssetManager::Clear() {

}

Shader AssetManager::LoadShader(const std::string vsPath, const std::string fsPath, const std::string name) {
	shaders[name] = Shader(vsPath.c_str(), fsPath.c_str());
	return shaders[name];
}

Shader AssetManager::GetShader(const std::string name) {
	return shaders[name];
}

Texture AssetManager::LoadTexture(const std::string path, const std::string name) {
	textures[name] = Texture::FromFile(path, false);
	return textures[name];
}

Texture AssetManager::GetTexture(const std::string name) {
	return textures[name];
}

Model AssetManager::LoadModel(const std::string path, const std::string name) {
	models[name] = Model(path, false);
	return models[name];
}

Model AssetManager::GetModel(const std::string name) {
	return models[name];
}