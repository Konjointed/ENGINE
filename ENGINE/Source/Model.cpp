#include <iostream>

#include <Shader.h>

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "AnimationData.h"
#include "AssimpGLMHelpers.h"

// Starts the loading processs
Model::Model(const std::string& path, bool gamma = false) 
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

Model::~Model() {}

void Model::Draw(Shader& shader, unsigned int shadowMap) {
	for (unsigned int i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader, shadowMap);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Load textures for different types 
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

	// Check to see if any were loaded
	if (diffuseMaps.empty() && specularMaps.empty() && normalMaps.empty() && heightMaps.empty()) {
		std::cout << "No materials were found for this mesh.\n";
	}
	else {
		std::cout << "Materials were found for this mesh.\n";
	}


	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	ExtractBoneWeightForVertices(vertices, mesh, scene);

	return Mesh(vertices, indices, textures);
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex) {
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
		vertex.boneIDs[i] = -1;
		vertex.weights[i] = 0.0f;
	}
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		printf("Loading material texture: %s\n", path.C_Str());
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < this->textures.size(); j++)
		{
			if (std::strcmp(this->textures[j].path.data(), path.C_Str()) == 0)
			{
				textures.push_back(this->textures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.ID = Texture::FromFile(path.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = path.C_Str();
			textures.push_back(texture);
			this->textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}

	return textures;
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
	auto& boneInfoMap = this->boneInfoMap;
	int& boneCount = this->boneCounter;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (unsigned int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId < vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
		if (vertex.boneIDs[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}