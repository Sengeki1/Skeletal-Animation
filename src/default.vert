#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUVs;
layout (location = 2) in vec3 aNormals;

out vec3 Normals;
out vec2 UVs;

uniform mat4 model;
uniform float scale;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * vec4(aPos * scale, 1.0);
	
	UVs = aUVs;
	Normals = aNormals;
}