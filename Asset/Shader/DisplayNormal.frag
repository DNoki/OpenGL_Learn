#version 330 core

in g2f
{
    vec4 Color;
} f;

out vec4 FragColor;

void main()
{
    FragColor = f.Color;
}












