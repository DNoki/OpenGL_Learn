{_!Property_Start
{
    "RenderQueue": 2000,
    "_MainColor": [1, 1, 1, 1],
    "_MainTexture": [0, "White"]
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

out vec2 uv;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    gl_Position = Matrix_P * Matrix_V * Matrix_M * vec4(position, 1.0f);
    uv = texCoord;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in vec2 uv;

out vec4 FragColor;

uniform vec4 _MainColor;
uniform sampler2D _MainTexture;

void main()
{
    FragColor = _MainColor * texture(_MainTexture, uv);
}
_!GLSL_FragmentProgram_End
}