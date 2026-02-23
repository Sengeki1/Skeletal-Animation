#include "animation.h"

Animation::Animation(
	aiAnimation* animation, 
	aiNode* rootNode,
	const std::map<std::string, int>& bonesMapping,
	const std::map<std::string, glm::mat4>& bonesOffsetMatrix,
	const std::vector<glm::mat4>& finalBoneMatrix
) {
	this->animation = animation;
	this->rootNode = rootNode;
	this->bonesMapping = bonesMapping;
	this->bonesOffsetMatrix = bonesOffsetMatrix;
	this->finalBoneMatrix = finalBoneMatrix;
}

void Animation::update(aiNode* node, glm::mat4 parentTransform, const float& currentTime) {
	glm::mat4 localTransform = ai4x4ToMat4(node->mTransformation);
	auto name = node->mName;

	float tickPerSecond = (animation->mTicksPerSecond != 0) ? animation->mTicksPerSecond : 25.0f;
	float timeInTicks = currentTime * tickPerSecond;
	float currentAnimationTime = fmod(timeInTicks, animation->mDuration);

	for (unsigned int i = 0; i < animation->mNumChannels; i++) {
		auto& channel = animation->mChannels[i];

		if (channel->mNodeName == name) { // get animation channel of the current bone node
			localTransform = interpolateTransform(channel, currentAnimationTime);
			break;
		}
	}

	glm::mat4 globalTransform = parentTransform * localTransform;
	if (bonesMapping.count(name.C_Str())) {
		glm::mat4 globalInverse = glm::inverse(ai4x4ToMat4(rootNode->mTransformation));
		glm::mat4 offset = bonesOffsetMatrix[name.C_Str()];

		int index = bonesMapping[name.C_Str()];
		finalBoneMatrix[index] = globalInverse * globalTransform * offset;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		update(node->mChildren[i], globalTransform, currentTime);
	}
}

glm::mat4 Animation::interpolateTransform(aiNodeAnim* channel,const float& currentAnimationTime) {
	glm::vec3 position, scale;
	glm::mat4 rotation;

	auto vectorTransformation = [&](auto* keyframes, unsigned int totalKeyframes) {		
		// If before first keyframe
		if (currentAnimationTime <= (float)keyframes[0].mTime) {
			const aiVector3D& v = keyframes[0].mValue;
			return glm::vec3(v.x, v.y, v.z);
		}

		// If after last keyframe
		if (currentAnimationTime >= (float)keyframes[totalKeyframes - 1].mTime) {
			const aiVector3D& v = keyframes[totalKeyframes - 1].mValue;
			return glm::vec3(v.x, v.y, v.z);
		}
		
		for (unsigned int i = 0; i < totalKeyframes - 1; i++) {			
			if (currentAnimationTime >= (float)keyframes[i].mTime && currentAnimationTime < (float)keyframes[i + 1].mTime) {
				float previous_keyframe_time = (float)keyframes[i].mTime;
				float next_keyframe_time = (float)keyframes[i + 1].mTime;

				const aiVector3D& start = keyframes[i].mValue;
				const aiVector3D& end = keyframes[i + 1].mValue;

				glm::vec3 startPos = glm::vec3(start.x, start.y, start.z);
				glm::vec3 endPos = glm::vec3(end.x, end.y, end.z);

				float factor = (currentAnimationTime - previous_keyframe_time) / (next_keyframe_time - previous_keyframe_time);

				return lerp(startPos, endPos, factor);
			}
		}
	};

	auto transformationQuat = [&](auto* keyframes, unsigned int totalKeyframes) {
		if (currentAnimationTime <= (float)keyframes[0].mTime) {
			const aiQuaternion& v = keyframes[0].mValue;
			return glm::toMat4(glm::normalize(glm::quat(v.w, v.x, v.y, v.z)));
		}

		if (currentAnimationTime >= (float)keyframes[totalKeyframes - 1].mTime) {
			const aiQuaternion& v = keyframes[totalKeyframes - 1].mValue;
			return glm::toMat4(glm::normalize(glm::quat(v.w, v.x, v.y, v.z)));
		}

		for (unsigned int i = 0; i < totalKeyframes - 1; i++) {
			if (currentAnimationTime >= (float)keyframes[i].mTime && currentAnimationTime < (float)keyframes[i + 1].mTime) {
				float previous_keyframe_time = (float)keyframes[i].mTime;
				float next_keyframe_time = (float)keyframes[i + 1].mTime;

				const aiQuaternion& start = keyframes[i].mValue;
				const aiQuaternion& end = keyframes[i + 1].mValue;
				
				glm::quat quatStart = glm::normalize(glm::quat(start.w, start.x, start.y, start.z));
				glm::quat quatEnd = glm::normalize(glm::quat(end.w, end.x, end.y, end.z));

				float factor = (currentAnimationTime - previous_keyframe_time) / (next_keyframe_time - previous_keyframe_time);

				glm::quat interpolated = glm::slerp(quatStart, quatEnd, factor); // spherical interpolation (correct for rotation instead of mix)
				return glm::toMat4(interpolated);
			}
		}
	};
	
	position = channel->mNumPositionKeys > 0
		? vectorTransformation(channel->mPositionKeys, channel->mNumPositionKeys) : glm::vec3(0.0f);
	rotation = channel->mNumRotationKeys > 0 
		? transformationQuat(channel->mRotationKeys, channel->mNumRotationKeys) : glm::mat4(1.0f);
	scale = channel->mNumScalingKeys > 0
		? vectorTransformation(channel->mScalingKeys, channel->mNumScalingKeys) : glm::vec3(1.0f);

	glm::mat4 totalTransform = glm::mat4(1.0f);
	totalTransform = glm::translate(totalTransform, position);
	totalTransform *= rotation;
	totalTransform = glm::scale(totalTransform, scale);

	return totalTransform;
}
