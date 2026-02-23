#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in vec4 aWeights;
layout (location = 4) in ivec4 aBoneIDs;

out vec3 Normals;
out vec3 FragPos;

out vec2 UVs;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 bones[72];

void main() {
	mat4 skinMatrix = aWeights.x * bones[aBoneIDs.x]
					+ aWeights.y * bones[aBoneIDs.y]
					+ aWeights.z * bones[aBoneIDs.z]
					+ aWeights.w * bones[aBoneIDs.w];


	vec4 skinnedPosition = skinMatrix * vec4(aPos, 1.0);

	gl_Position = projection * model * skinnedPosition;
	FragPos = vec3(skinnedPosition);
	
	UVs = aUVs;
	Normals = aNormals;
}