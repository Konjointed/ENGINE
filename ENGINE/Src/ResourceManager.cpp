#include <iostream>
#include <sstream>
#include <fstream>

#include <Shader.h>

#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"

// Instantiate static variables
std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Model> ResourceManager::models;

// SHADERS
Shader ResourceManager::LoadShader(std::string vsPath, std::string fsPath, std::string gsPath, std::string name) {
    shaders[name] = LoadShaderFromFile(vsPath, fsPath, gsPath);
	return shaders[name];
}
Shader ResourceManager::GetShader(std::string name) {
	return shaders[name];
}

// TEXTURES
Texture ResourceManager::LoadTexture(std::string path, std::string name) {
	return textures[name];
}
Texture ResourceManager::LoadCubemap(std::vector<std::string> faces, std::string name) {
	textures[name] = Texture::LoadCubemap(faces);
	return textures[name];
}
Texture ResourceManager::GetTexture(std::string name) {
	return textures[name];
}

// MODELS
Model ResourceManager::LoadModel(std::string path, std::string name) {
    models[name] = Model(path, false);
    return models[name];
}
Model ResourceManager::GetModel(std::string name) {
    return models[name];
}

Shader ResourceManager::LoadShaderFromFile(std::string vsPath, std::string fsPath, std::string gsPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vsPath);
        std::ifstream fragmentShaderFile(fsPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gsPath != "")
        {
            std::ifstream geometryShaderFile(gsPath);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gsPath != "" ? gShaderCode : nullptr);
    return shader;
}