{_!Property_Start
{
    "RenderQueue": 4000,
    "_Magnitude": 0.05
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
//layout (location = 4) in vec2 texCoord;

out v2g
{
    vec3 WorldSpaceTangent;
    vec3 WorldSpaceBitangent;
    vec3 WorldSpaceNormal;
    mat4 Matrix_PV;
} g;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    mat3 normalTransform = mat3(transpose(inverse(Matrix_M)));
    g.WorldSpaceNormal = normalize(vec3(normalTransform * normal));
    g.WorldSpaceTangent = normalize(vec3(normalTransform * tangent));
    g.WorldSpaceBitangent = normalize(vec3(normalTransform * cross(normal, tangent)));

    g.Matrix_PV = Matrix_P * Matrix_V;
    gl_Position = Matrix_M * vec4(position, 1.0f);
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_GeometryProgram_Start
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 18) out;

// 内建变量
//in gl_Vertex
//{
//    vec4  gl_Position;
//    float gl_PointSize;
//    float gl_ClipDistance[];
//} gl_in[];

in v2g
{
    vec3 WorldSpaceTangent;
    vec3 WorldSpaceBitangent;
    vec3 WorldSpaceNormal;
    mat4 Matrix_PV;
} g[];

out g2f
{
    vec4 Color;
} f;

uniform float _Magnitude;

void GenerateLine(int index, vec4 pos1, vec4 pos2, vec4 color)
{
    f.Color = color;
    gl_Position = g[index].Matrix_PV * pos1;
    EmitVertex();
    gl_Position = g[index].Matrix_PV * pos2;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    for (int i = 0; i < 3; i++)
    {
        GenerateLine(i, gl_in[i].gl_Position, gl_in[i].gl_Position + vec4(g[i].WorldSpaceTangent * _Magnitude, 0.0), vec4(1.0f, 0.0f, 0.0f, 0.0f));
        GenerateLine(i, gl_in[i].gl_Position, gl_in[i].gl_Position + vec4(g[i].WorldSpaceBitangent * _Magnitude, 0.0), vec4(0.0f, 1.0f, 0.0f, 0.0f));
        GenerateLine(i, gl_in[i].gl_Position, gl_in[i].gl_Position + vec4(g[i].WorldSpaceNormal * _Magnitude, 0.0), vec4(0.0f, 0.0f, 1.0f, 0.0f));
    }
}
_!GLSL_GeometryProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in g2f
{
    vec4 Color;
} f;

out vec4 FragColor;

void main()
{
    FragColor = f.Color;
}
_!GLSL_FragmentProgram_End
}