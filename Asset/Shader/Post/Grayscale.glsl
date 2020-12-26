{_!Property_Start
{
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

// 灰度
vec4 Grayscale(vec4 texColor)
{
    //float average = (texColor.r + texColor.g + texColor.b) / 3.0f;// 平均灰度
    //float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;// 加权灰度
    float average = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));// 加权灰度

    return vec4(vec3(average), 1.0f); // 灰度
}

void main()
{
    FragColor = Grayscale(texture(_MainTexture, f.uv));
    //FragColor = vec4(color, 1.0f);
}
_!GLSL_FragmentProgram_End
}

















