#version 330 core

in v2f
{
    vec3 PositionWorldSpace;
    vec3 NormalWorldSpace;
    vec2 TexCoord;
} f;

out vec4 FragColor;

void main()
{
	FragColor = vec4(f.PositionWorldSpace, 1.0f);
}