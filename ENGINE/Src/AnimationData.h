#ifndef ANIMATION_DATA_H
#define ANIMATION_DATA_H

#include <glm/mat4x4.hpp>

struct BoneInfo {
	// id is index in finalBoneMatrices
	int id;
	//offset matrix transforms vertex from model space to bone space
	glm::mat4 offset;
};

#endif