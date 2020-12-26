{_!Property_Start
{
    "RenderQueue": 2333
}
_!Property_End
}
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
//layout (location = 4) in vec2 texCoord;

out v2f
{
    vec4 WorldSpacePosition;
    //vec3 WorldSpaceNormal;
    //vec2 uv;
    //vec4 LightSpacePosition;
} f;

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

void main()
{
    float orthoSize = 1.0f;
    if(bool(OrthoParams.w))
        orthoSize = max(orthoSize, max(OrthoParams.x, OrthoParams.y));

    f.WorldSpacePosition = vec4(position, 1.0f);
    gl_Position = (Matrix_P * mat4(mat3(Matrix_V)/*去除相机位置变换*/) * f.WorldSpacePosition).xyww;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in v2f
{
    vec4 WorldSpacePosition;
    //vec3 WorldSpaceNormal;
    //vec2 uv;
    //vec4 LightSpacePosition;
} f;

out vec4 FragColor;

layout (std140) uniform TIME
{
    // (RunTime, SinTime, CosTime, DeltaTime);
    vec4 Time;
};

//uniform vec4 _MainColor;
//uniform sampler2D _MainTexture;
uniform samplerCube _CubeTexture;

void main()
{
    FragColor = texture(_CubeTexture, f.WorldSpacePosition.xyz);
}
_!GLSL_FragmentProgram_End
}