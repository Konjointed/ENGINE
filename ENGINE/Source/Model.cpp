#include <iostream>

#include "Model.h"

// Starts the loading processs
Model::Model(std::string const& path, bool gamma = false) 
	: gammaCorrection(gamma), position(0.0f), rotation(0.0f), scale(1.0f) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, 
											aiProcess_Triangulate | 
											aiProcess_GenSmoothNormals | 
											aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {

}