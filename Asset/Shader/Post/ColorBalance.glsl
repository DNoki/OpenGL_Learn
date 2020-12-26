{_!Property_Start
{
    "_Exposure": 1,
    "_Gamma": 2.2
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
//uniform samplerCube _CubeTexture;
//uniform sampler2D _ShadowMap;

uniform float _Exposure; // 曝光度
uniform float _Gamma; // 伽马值

void main()
{
    vec4 texColor = texture(_MainTexture, f.uv);
    
    // Reinhard色调映射
    //texColor.rgb = texColor.rgb / (texColor.rgb + vec3(1.0f));
    
    // Exposure曝光度色调映射
    texColor.rgb = vec3(1.0f) - exp(-texColor.rgb * _Exposure);
    
    // Gamma校正
    texColor.rgb = pow(texColor.rgb, vec3(1.0f / _Gamma));
    FragColor = texColor;
    //FragColor = vec4(color, 1.0f);
}
_!GLSL_FragmentProgram_End
}

















