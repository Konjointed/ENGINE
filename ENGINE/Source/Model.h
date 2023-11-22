#ifndef MODEL_H 
#define MODEL_H 

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>

class Texture;
class Mesh;
class Shader;
struct Vertex;
struct BoneInfo;

class Model {
public:
	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(const std::string& path, bool gamma);
	~Model();

	void Draw(Shader& shader);

	auto& GetBoneInfoMap() { return boneInfoMap; }
	int& GetBoneCount() { return boneCounter; }
	/*
	glm::mat4 GetModelMatrix() const;

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& rot);
	void SetScale(const glm::vec3& scl);
	*/
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::map<std::string, BoneInfo> boneInfoMap;
	int boneCounter = 0;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneDataToDefault(Vertex& vertex);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
};

#endif 