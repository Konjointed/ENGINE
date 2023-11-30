#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <vector>

#include <glm/ext/matrix_float4x4.hpp>

class Animation;
struct AssimpNodeData;

class Animator
{
private:
	std::vector<glm::mat4> finalBoneMatrices;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;
	bool animationEnded = false;
public:
	Animator();
	Animator(Animation* animation);

	void UpdateAnimation(float dt);
	void PlayAnimation(Animation* pAnimation);
	void StopAnimation();
	bool IsAnimationEnded() const;
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	std::vector<glm::mat4> GetFinalBoneMatrices();

	~Animator();
};

#endif