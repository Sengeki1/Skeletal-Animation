#pragma once
#ifndef COMMON_IMPLEMENTATION_H
#define COMMON_IMPLEMENTATION_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float factor);

glm::mat4 ai4x4ToMat4(const aiMatrix4x4& ai);


#endif // COMMON_IMPLEMENTATION_H