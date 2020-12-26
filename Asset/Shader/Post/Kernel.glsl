{
_!Property_Start
{
    "_KernelRadius": 1
}
_!Property_End
}
----------------------------------------------------------------------------
{
_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
layout (location = 4) in vec2 texCoord;

out v2f
{
    //vec4 WorldSpacePosition;
    //vec3 WorldSpaceNormal;
    vec2 uv;
    //vec4 LightSpacePosition;
} f;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
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
    //vec4 WorldSpacePosition;
    //vec3 WorldSpaceNormal;
    vec2 uv;
    //vec4 LightSpacePosition;
} f;

out vec4 FragColor;

//uniform vec4 _MainColor;
uniform sampler2D _MainTexture;
//uniform sampler2D _ShadowMap;
uniform float _KernelRadius;

// 核效果
vec4 Kernel()
{
    vec2 offset = 1.0 / textureSize(_MainTexture, 0) * _KernelRadius;
    vec2 offsets[9] = vec2[](
        vec2(-offset.x, offset.y), vec2( 0.0f, offset.y), vec2( offset.x, offset.y), // 上
        vec2(-offset.x, 0.0f), vec2( 0.0f, 0.0f), vec2( offset.x, 0.0f),   // 中
        vec2(-offset.x, -offset.y), vec2( 0.0f, -offset.y), vec2( offset.x, -offset.y)  // 下
    );
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(_MainTexture, f.uv + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0f);
}

void main()
{
    FragColor = Kernel();
}
_!GLSL_FragmentProgram_End
}