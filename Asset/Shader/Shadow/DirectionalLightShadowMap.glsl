{_!Property_Start
{
}
_!Property_End
}
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
//layout (location = 4) in vec2 texCoord;

out v2f
{
    //vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
    //vec2 uv;
    vec4 LightSpacePosition;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

layout (std140) uniform LIGHTING_DATA
{
    vec4 LightColor; // 光照颜色
    vec4 WorldSpaceLightPos; // 平行光：（世界空间方向, 0）。其他光：（世界空间位置, 1）。
    mat4 LightMatrix; // 世界到光的矩阵。用于采样Cookie和衰减纹理。
    float ShadowStrength; // 阴影强度。
    // 前向渲染使用4个点光源
    vec4 PointLight4Pos[4]; // 点光源位置
    vec4 PointLight4Color[4]; // 点光源颜色
    vec2 PointLight4Atten[4]; // 点光源衰减 C=1.0, L=x, Q=y 衰减强度 = 1.0 / (C + L * d + Q * d^2) ，点光源阴影强度，点光源远裁面w
};

uniform float _NormalBias;

void main()
{
    f.WorldSpaceNormal = transpose(inverse(mat3(Matrix_M))) * normal;
    vec4 worldPos = Matrix_M * vec4(position, 1.0f);
    worldPos.xyz = worldPos.xyz - normalize(f.WorldSpaceNormal) * _NormalBias;
    gl_Position = LightMatrix * worldPos;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in v2f
{
    //vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
    //vec2 uv;
    vec4 LightSpacePosition;
} f;

layout (std140) uniform LIGHTING_DATA
{
    vec4 LightColor; // 光照颜色
    vec4 WorldSpaceLightPos; // 平行光：（世界空间方向, 0）。其他光：（世界空间位置, 1）。
    mat4 LightMatrix; // 世界到光的矩阵。用于采样Cookie和衰减纹理。
    float ShadowStrength; // 阴影强度。
    // 前向渲染使用4个点光源
    vec4 PointLight4Pos[4]; // 点光源位置
    vec4 PointLight4Color[4]; // 点光源颜色
    vec2 PointLight4Atten[4]; // 点光源衰减 C=1.0, L=x, Q=y 衰减强度 = 1.0 / (C + L * d + Q * d^2)
};

uniform float _Bias; // 基准值偏移

void main()
{
    vec3 normal = normalize(f.WorldSpaceNormal);
    vec3 lightDir = normalize(WorldSpaceLightPos.xyz);
    
    // 根据法线方向调整偏移
    float bias = max(10.0f * _Bias * (1.0f - dot(normal, lightDir)), _Bias);
    gl_FragDepth = gl_FragCoord.z + bias;
}
_!GLSL_FragmentProgram_End
}

















