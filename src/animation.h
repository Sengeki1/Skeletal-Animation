#pragma once
#ifndef ANIMATION_IMPLEMENTATION_H
#define ANIMATION_IMPLEMENTATION_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "common.h"

class Animation {
	public:
		Animation(
			aiAnimation* animation, 
			aiNode* rootNode,
			const std::map<std::string, int>& bonesMapping,
			const std::map<std::string, glm::mat4>& bonesOffsetMatrix,
			const std::vector<glm::mat4>& finalBoneMatrix
		);
		void update(
			aiNode* node, 
			glm::mat4 parentTransform, 
			const float& currentTime
		);

		std::map<std::string, int> bonesMapping;
		std::map<std::string, glm::mat4> bonesOffsetMatrix;
		std::vector<glm::mat4> finalBoneMatrix;
	private:
		glm::mat4 interpolateTransform(
			aiNodeAnim* channel, 
			const float& currentTime
		);
		
		aiAnimation* animation;
		aiNode* rootNode;
};

# endif // ANIMATION_IMPLEMENTATION_H