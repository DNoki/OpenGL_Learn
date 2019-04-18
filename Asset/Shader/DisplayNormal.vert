#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;

out v2g
{
    vec3 NormalWorldSpace;
    vec3 TangentWorldSpace;
    vec3 BitangentWorldSpace;
    mat4 Matrix_PV;
} g;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    mat3 normalTransform = mat3(transpose(inverse(Matrix_M)));
    g.NormalWorldSpace = normalize(vec3(normalTransform * normal));
    g.TangentWorldSpace = normalize(vec3(normalTransform * tangent));
    g.BitangentWorldSpace = normalize(vec3(normalTransform * cross(normal, tangent)));

    g.Matrix_PV = Matrix_P * Matrix_V;
    
    gl_Position = Matrix_M * vec4(position, 1.0f);
}















