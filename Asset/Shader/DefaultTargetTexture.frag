#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture2D;

void main()
{
    color = texture(texture2D, TexCoord);
}












