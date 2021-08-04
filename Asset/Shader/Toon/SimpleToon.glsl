{_!Property_Start
{
    "RenderQueue": 2000,
    "_MainColor": [1, 1, 1, 1],
    "_MainTexture": [0, "White"],
    "_NormalTexture": [2, "Normal"],
    "_NormalStrength": 1
}
_!Property_End
}
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
layout (location = 4) in vec2 texCoord;

out v2f
{
    vec4 worldPosition;
    vec3 worldNormal;
    vec2 uv;
    vec4 lightSpacePosition;
    vec4 localTangent;
    vec4 localBitangent;
    vec4 localNormal;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

#define POINT_LIGHTS_COUNT 4
layout (std140) uniform LIGHTING_DATA
{
    vec4 LightColor; // 光照颜色
    vec4 WorldSpaceLightPos; // 平行光：（世界空间方向, 0）。其他光：（世界空间位置, 1）。
    mat4 LightMatrix; // 世界到光的矩阵。用于采样Cookie和衰减纹理。
    float ShadowStrength; // 阴影强度。
    // 前向渲染使用4个点光源
    vec4[POINT_LIGHTS_COUNT] PointLight4Pos; // 点光源位置
    vec4[POINT_LIGHTS_COUNT] PointLight4Color; // 点光源颜色
    vec4[POINT_LIGHTS_COUNT] PointLight4Info; // 点光源衰减 C=1.0, L=x, Q=y 衰减强度 = 1.0 / (C + L * d + Q * d^2) ，点光源近裁面z，点光源远裁面w
};

void main()
{
    f.worldPosition = Matrix_M * vec4(position, 1.0f);
    gl_Position = Matrix_P * Matrix_V * f.worldPosition;
    f.worldNormal = transpose(inverse(mat3(Matrix_M))) * normal;
    f.uv = texCoord;
    f.lightSpacePosition = LightMatrix * f.worldPosition;
    
    f.localNormal = vec4(normal, 0.0f);
    f.localTangent = vec4(tangent, 0.0f);
    //f.localTangent = normalize(T - dot(T, N) * N);
    f.localBitangent = vec4(cross(tangent, normal), 0.0f);
    
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in v2f
{
    vec4 worldPosition;
    vec3 worldNormal;
    vec2 uv;
    vec4 lightSpacePosition;
    vec4 localTangent;
    vec4 localBitangent;
    vec4 localNormal;
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
    vec4 WorldSpaceCameraPos; // 世界空间相机位置
    vec4 OrthoParams; // x=正交摄影机的宽度，y=正交摄影机的高度，z=宽高比，w=正交摄影机时为1.0，透视时为0.0。
    vec4 ScreenParams; // x=相机目标纹理的宽度，y=相机目标纹理的高度，z=1.0/width，w=1.0/height。
};

#define POINT_LIGHTS_COUNT 4
layout (std140) uniform LIGHTING_DATA
{
    vec4 LightColor; // 光照颜色
    vec4 WorldSpaceLightPos; // 平行光：（世界空间方向, 0）。其他光：（世界空间位置, 1）。
    mat4 LightMatrix; // 世界到光的矩阵。用于采样Cookie和衰减纹理。
    float ShadowStrength; // 阴影强度。
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

uniform sampler2D _NormalTexture;
uniform float _NormalStrength;

uniform sampler2D _ShadowMap;
uniform samplerCube[POINT_LIGHTS_COUNT] _PointLight4ShadowMap;


// float DirectionalLightShadowCalculation(vec4 lightSpacePosition)
// {
//     vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w; // 执行透视除法
//     if(projCoords.z > 1.0f) return 0.0f;
//     projCoords = projCoords * 0.5f + 0.5f;
//     float currentDepth = projCoords.z;
    
//     float shadow = 0.0;
//     vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);
//     for(int x = -1; x <= 1; ++x)
//         for(int y = -1; y <= 1; ++y)
//         {
//             float pcfDepth = texture(_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
//             shadow += currentDepth > pcfDepth ? 0.0 : 1.0;
//         }
//     shadow /= 9.0;
//     return shadow; // 深度：该像素不在阴影处返回1
// }

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir, vec3 lightDir
    )
{
    float lambertK = 1.0 - smoothstep(0.395, 0.405, 1.0 - max((dot(normal, lightDir) + 1) * 0.5f, 0.0f)) * 0.5;
    vec3 lambert = vec3(lambertK, lambertK, lambertK);
    
    // 阴影
    float shadow = 0.0;
    vec3 projCoords = f.lightSpacePosition.xyz / f.lightSpacePosition.w; // 执行透视除法
    if(projCoords.z <= 1.0f) 
    {
        projCoords = projCoords * 0.5f + 0.5f;
        vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);
        for(int x = -1; x <= 1; ++x)
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += projCoords.z > pcfDepth ? 0.0 : 1.0;
            }
        shadow /= 9.0; // 深度：该像素不在阴影处返回1
        shadow = max(shadow, 1.0 - ShadowStrength);
    }
    else shadow = 1.0;

    return (lambert) * shadow;
}

void main()
{
    // mat3 TBN = mat3(
    //     normalize((Matrix_M * f.localTangent).xyz),
    //     normalize((Matrix_M * f.localBitangent).xyz),
    //     normalize((Matrix_M * f.localNormal).xyz));
    // vec3 normal = texture(_NormalTexture, f.uv).rgb * 2.0f - 1.0f;
    // normal.xy *= _NormalStrength;
    // normal = normalize(TBN * normal);
    
    vec3 normal = normalize(f.worldNormal.xyz);
    vec3 dirLightDir = normalize(-WorldSpaceLightPos.xyz);
    vec3 viewDir = normalize(WorldSpaceCameraPos.xyz - f.worldPosition.xyz);
    
    vec4 diffTexColor = texture(_MainTexture, f.uv) * _MainColor;
    vec3 color = CalcDirectionalLight(normal, viewDir, dirLightDir) * diffTexColor.rgb;
    
    FragColor = vec4(color, diffTexColor.a);
    //FragColor = vec4(f.worldPosition.xyz, 1.0);
    // FragColor = vec4(dirLightDir, 1.0);
    //FragColor = vec4(vec3(attenuation), diffTexColor.a);
    // FragColor = diffTexColor;
}
_!GLSL_FragmentProgram_End
}

















