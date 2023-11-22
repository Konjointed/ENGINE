#include "Animator.h"

#include <iostream>

#include "Animation.h"
#include "Bone.h"
#include "AnimationData.h"

Animator::Animator() {}

Animator::Animator(Animation* animation) {
	std::cout << "Animator Ctor\n";

	this->currentTime = 0.0;
	this->currentAnimation = animation;

	this->finalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		this->finalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt) {
	this->deltaTime = dt;
	if (this->currentAnimation)
	{
		this->currentTime += this->currentAnimation->GetTicksPerSecond() * dt;

		if (this->currentTime >= this->currentAnimation->GetDuration()) {
			//printf("Animation Ended\n");
			animationEnded = true;
		}

		this->currentTime = fmod(this->currentTime, this->currentAnimation->GetDuration());
		CalculateBoneTransform(&this->currentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation* pAnimation) {
	this->currentAnimation = pAnimation;
	this->currentTime = 0.0f;
	animationEnded = false;
}

void Animator::StopAnimation() {
	this->currentAnimation = nullptr;
	this->currentTime = 0.0f;
}

bool Animator::IsAnimationEnded() const {
	return animationEnded;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform) {
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* Bone = this->currentAnimation->FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(this->currentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = this->currentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		this->finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetFinalBoneMatrices() {
	return this->finalBoneMatrices;
}

Animator::~Animator() {
	std::cout << "Animator Dtor\n";
}