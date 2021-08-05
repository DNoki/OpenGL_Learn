{_!Property_Start
{
    "RenderQueue": 2000,
    "_MainColor": [1, 1, 1, 1],
    "_MainTexture": [0, "White"]
}
_!Property_End
}
    "_SpecularColor": [1, 1, 1, 1],
    "_SpecularTexture": [1, "White"],
    "_Shininess": 32,
    "_NormalTexture": [2, "Normal"],
    "_NormalStrength": 1
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
layout (location = 4) in vec2 texCoord;

out v2f
{
    vec4 localSpacePosition;
    vec4 worldSpacePosition;
    vec3 worldSpaceNormal;
    vec2 uv;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    f.localSpacePosition = vec4(position, 1.0f);
    f.worldSpacePosition = Matrix_M * vec4(position, 1.0f);
    // gl_Position = Matrix_P * f.worldSpacePosition;
    gl_Position = Matrix_P * Matrix_V * f.worldSpacePosition;
    f.worldSpaceNormal = transpose(inverse(mat3(Matrix_M))) * normal;
    f.uv = texCoord;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{
_!GLSL_FragmentProgram_Start
#version 330 core

in v2f
{
    vec4 localSpacePosition;
    vec4 worldSpacePosition;
    vec3 worldSpaceNormal;
    vec2 uv;
} f;

out vec4 FragColor;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

layout (std140) uniform CAMERA_DATA
{
    // 世界空间相机位置
    vec4 WorldSpaceCameraPos;
    // x是正交摄影机的宽度，y是正交摄影机的高度，z为宽高比，w当正交摄影机时为1.0，透视时为0.0。
    vec4 OrthoParams;
};

#define POINT_LIGHTS_COUNT 4
layout (std140) uniform LIGHTING_DATA
{
    vec4 LightColor; // 光照颜色
    vec4 WorldSpaceLightPos; // 平行光：（世界空间方向, 0）。其他光：（世界空间位置, 1）。
    mat4 LightMatrix; // 世界到光的矩阵。用于采样Cookie和衰减纹理。
    float ShadowStrength; // 阴影强度。
    float Bias; // 基准值偏移
    // 前向渲染使用4个点光源
    vec4[POINT_LIGHTS_COUNT] PointLight4Pos; // 点光源位置
    vec4[POINT_LIGHTS_COUNT] PointLight4Color; // 点光源颜色
    vec4[POINT_LIGHTS_COUNT] PointLight4Info; // 点光源衰减 C=1.0, L=x, Q=y 衰减强度 = 1.0 / (C + L * d + Q * d^2) ，z=点光源阴影强度，w=点光源远裁面
};

layout (std140) uniform TIME
{
    // (RunTime, SinTime, CosTime, DeltaTime);
    vec4 Time;
};

uniform vec4 _MainColor;
uniform sampler2D _MainTexture;
//uniform samplerCube _CubeTexture;

void main()
{
    vec3 normal = normalize(f.worldSpaceNormal.xyz);
    vec3 lightDir = normalize(-WorldSpaceLightPos.xyz);
    
    vec3 color = LightColor.rgb * max((dot(normal, lightDir)+0)/1, 0.0f) * texture(_MainTexture, f.uv).rgb;
    //float value = ;
    //FragColor = vec4(value,value,value,1.0f);
    FragColor = vec4(f.localSpacePosition.xyz, 1.0f);
    
    //vec3 viewDir = normalize(WorldSpaceCameraPos.xyz - f.worldSpacePosition.xyz);
    //float value = 1.0f - dot(normal, viewDir); // 菲涅尔效果
}
_!GLSL_FragmentProgram_End
}