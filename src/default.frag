#version 400

in vec3 Normals;
in vec3 FragPos;
in vec2 UVs;

uniform sampler2D tex0;

out vec4 FragColor;

void main()
{
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 lightPos = vec3(0.0, 1400.0, -200.0);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 normals = normalize(Normals);

    float ambientStrength = 0.3;
    vec4 ambient = ambientStrength * lightColor;

    float diff = max(dot(normals, lightDir), 0.0);
    
    FragColor = texture(tex0, UVs) * (diff + ambient);
}