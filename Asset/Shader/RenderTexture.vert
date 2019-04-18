#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 4) in vec2 texCoord;

out vec2 TexCoord;
uniform mat4 Matrix_M;

void main()
{
    gl_Position = Matrix_M * vec4(position.x, position.y, position.z, 1.0); 
    TexCoord = texCoord;
}