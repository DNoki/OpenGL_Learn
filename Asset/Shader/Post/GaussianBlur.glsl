{_!Property_Start
{
    "_KernelRadius": 1,
    "_Variance": 1,
    "_Offset": [1, 0, 0, 0]
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
{_!GLSL_FragmentProgram_Start
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
uniform float _Variance;
uniform vec4 _Offset;

float GaussianFunction(vec2 pos)
{
    return exp(-(pos.x * pos.x + pos.y * pos.y) / _Variance);
}

// ¸ßË¹Ä£ºý
vec4 GaussianBlur()
{
    vec2 offset = 1.0 / textureSize(_MainTexture, 0) * _KernelRadius * _Offset.xy;
    const int offsetSize = 11;
    const vec2 offsets[offsetSize] = vec2[](
        vec2(-5, -5), vec2(-4, -4), vec2(-3, -3), vec2(-2, -2), vec2(-1, -1), vec2(0, 0), 
        vec2(1, 1), vec2(2, 2), vec2(3, 3), vec2(4, 4), vec2(5, 5)
    );
    
    vec3 col = vec3(0.0f);
    float weight = 0.0f;
    for(int i = 0; i < offsetSize; i++)
    {
        float k = GaussianFunction(offsets[i] * _Offset.xy);
        col += texture(_MainTexture, f.uv.st + (offsets[i] * offset)).rgb * k;
        weight += k;
    }
    return vec4(col / weight, 1.0f);
}

void main()
{
    FragColor = GaussianBlur();
    //FragColor = vec4(color, 1.0f);
}
_!GLSL_FragmentProgram_End
