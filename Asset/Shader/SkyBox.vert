#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoord;

layout (std140) uniform Transform
{
	mat4 Matrix_P;
	mat4 Matrix_V;
	mat4 Matrix_M;
};

void main()
{
    gl_Position = (Matrix_P * Matrix_V * vec4(position, 1.0)).xyww; 
    TexCoord = position;
}