#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Animation.h"
#include "AssimpGLMHelpers.h"
#include "Bone.h"
#include "Model.h"
#include "AnimationData.h"

Animation::Animation(const std::string& animationPath, Model* model) {
	std::cout << "Animation Ctor\n";

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	this->duration = animation->mDuration;
	this->ticksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHierarchyData(this->rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model) {
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.GetBoneInfoMap(); //getting m_BoneInfoMap from Model class
	int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		this->bones.push_back(Bone(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id, channel));
	}

	this->boneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) {
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

Bone* Animation::FindBone(const std::string& name) {
	auto iter = std::find_if(this->bones.begin(), this->bones.end(),
		[&](const Bone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iter == this->bones.end()) return nullptr;
	else return &(*iter);
}

Animation::~Animation() {
	std::cout << "Animation Dtor\n";
}