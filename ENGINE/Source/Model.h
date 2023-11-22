#ifndef MODEL_H 
#define MODEL_H 

#include <string>
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>

class Mesh;
struct BoneInfo;

class Model {
public:
	std::vector<unsigned int> textures;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(std::string const& path, bool gamma);
	~Model();

	void Draw();

	auto& GetBoneInfoMap();
	int& GetBoneCount();

	glm::mat4 GetModelMatrix() const;

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& rot);
	void SetScale(const glm::vec3& scl);
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::map<std::string, BoneInfo> boneInfoMap;
	int boneCounter = 0;

	void ProcessNode(aiNode* node, const aiScene* scene);
};

#endif 