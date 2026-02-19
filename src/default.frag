#version 400

in vec3 Normals;
in vec2 UVs;

uniform sampler2D tex0;

out vec4 FragColor;

void main()
{
    FragColor = texture(tex0, UVs);
}