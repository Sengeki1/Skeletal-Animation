#version 400

in vec3 Normals;
in vec2 UVs;

uniform sampler2D tex0;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}