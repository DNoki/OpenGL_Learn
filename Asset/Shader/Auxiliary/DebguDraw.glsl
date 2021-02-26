{_!Property_Start
{
    "RenderQueue": 4000
}
_!Property_End
}
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
// layout (location = 1) in vec3 normal;
// layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 color;
//layout (location = 4) in vec2 texCoord;

out v2g
{
    vec3 Color;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    gl_Position = Matrix_P * Matrix_V * vec4(position, 1.0f);
    f.Color = color;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------

{_!GLSL_FragmentProgram_Start
#version 330 core

in v2g
{
    vec3 Color;
} f;

out vec4 FragColor;

void main()
{
    FragColor = vec4(f.Color, 1.0f);
}
_!GLSL_FragmentProgram_End
}