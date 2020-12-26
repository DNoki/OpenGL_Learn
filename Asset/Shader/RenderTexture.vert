#version 330 core
layout (location = 0) in vec3 position;
layout (location = 4) in vec2 uv;

out vec2 TexCoord;
//uniform mat4 Matrix_M;

mat4 Matrix_M = mat4(
                2.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 2.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
    TexCoord = uv;
}