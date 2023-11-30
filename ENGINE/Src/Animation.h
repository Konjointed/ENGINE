#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/ext/matrix_float4x4.hpp>

class Model;
class Bone;
struct BoneInfo;

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation {
private:
	float duration;
	float ticksPerSecond;
	AssimpNodeData rootNode;
	std::vector<Bone> bones;
	std::map<std::string, BoneInfo> boneInfoMap;

	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
public:
	Animation() = default;
	Animation(const std::string& animationPath, Model* model);

	Bone* FindBone(const std::string& name);

	inline float GetTicksPerSecond() { return this->ticksPerSecond; }
	inline float GetDuration() { return this->duration; }
	inline const AssimpNodeData& GetRootNode() { return this->rootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return this->boneInfoMap; }

	~Animation();
};

#endif 