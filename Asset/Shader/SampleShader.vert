#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 color;
layout (location = 4) in vec2 texCoord;

out v2f
{
    vec3 PositionWorldSpace;
    vec3 NormalWorldSpace;
    vec2 TexCoord;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};
layout (std140) uniform CAMERA
{
    vec4 CameraWorldSpace;
};
layout (std140) uniform LIGHT
{
    vec4 LightWorldSpace;
    vec4 LightDirWorldSpace;
};

void main()
{
    f.PositionWorldSpace = (Matrix_M * vec4(position, 1.0f)).xyz;
    //f.NormalWorldSpace = mat3(Matrix_M) * normal;
    f.NormalWorldSpace = mat3(transpose(inverse(Matrix_M))) * normal;// 法线变换 参考 Unity Shader 4.7章
    f.TexCoord = texCoord;
    
    gl_Position = Matrix_P * Matrix_V * vec4(f.PositionWorldSpace, 1.0f);
    //f.PositionWorldSpace = position;
}















