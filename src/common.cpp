#include "common.h"

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float factor) {
	return a + factor * (b - a);
}

glm::mat4 ai4x4ToMat4(const aiMatrix4x4& ai) {
    return glm::mat4(
        ai.a1, ai.b1, ai.c1, ai.d1,
        ai.a2, ai.b2, ai.c2, ai.d2,
        ai.a3, ai.b3, ai.c3, ai.d3,
        ai.a4, ai.b4, ai.c4, ai.d4
    );
}

