{_!Property_Start
{
    "RenderQueue": 4500,
    "_MainColor": [1, 1, 1, 1],
    "_MainTexture": [0, "White"]
}
_!Property_End
}
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
// layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
// layout (location = 4) in vec2 texCoord;

out v2f
{
    vec4 worldPosition;
    vec3 worldNormal;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    f.worldPosition = Matrix_M * vec4(position, 1.0f);
    gl_Position = Matrix_P * Matrix_V * f.worldPosition;
    f.worldNormal = transpose(inverse(mat3(Matrix_M))) * normal;
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

layout (std140) uniform TIME
{
    // (RunTime, SinTime, CosTime, DeltaTime);
    vec4 Time;
};

uniform vec4 _MainColor;
uniform sampler2D _MainTexture;

void main()
{
    vec3 normal = normalize(f.worldNormal);
    vec3 viewDir = normalize((WorldSpaceCameraPos - f.worldPosition).xyz);

    vec3 color = vec3(1.0f - max(dot(normal, viewDir), 0.0f));
    
    FragColor = vec4(color, 1.0f);
    //FragColor = vec4(f.worldPosition.xyz, diffTexColor.a);
    //FragColor = vec4(normal, diffTexColor.a);
    //FragColor = vec4(vec3(attenuation), diffTexColor.a);
    //FragColor = LightColor;
}
_!GLSL_FragmentProgram_End
}