{_!Property_Start
{

}
_!Property_End
}
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
//layout (location = 4) in vec2 texCoord;

out v2g
{
    //vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
    //vec2 uv;
} g;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

uniform float _NormalBias;

void main()
{
    g.WorldSpaceNormal = normalize(transpose(inverse(mat3(Matrix_M))) * normal);
    vec4 worldPos = Matrix_M * vec4(position, 1.0f);
    worldPos.xyz = worldPos.xyz - g.WorldSpaceNormal * _NormalBias;
    gl_Position = worldPos;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_GeometryProgram_Start
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

// 内建变量
//in gl_Vertex
//{
//    vec4  gl_Position;
//    float gl_PointSize;
//    float gl_ClipDistance[];
//} gl_in[];

in v2g
{
    //vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
    //vec2 uv;
} g[];

out g2f
{
    vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
} f;

uniform mat4 _ShadowMatrices[6];

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            f.WorldSpacePosition = gl_in[i].gl_Position;
            f.WorldSpaceNormal = g[i].WorldSpaceNormal;
            gl_Position = _ShadowMatrices[face] * f.WorldSpacePosition;
            EmitVertex();
        }    
        EndPrimitive();
    }
}

_!GLSL_GeometryProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in g2f
{
    vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
} f;

uniform vec4 _PointLightPosition;
uniform float _FarPlane;
uniform float _Bias; // 基准值偏移

void main()
{
    vec3 normal = normalize(f.WorldSpaceNormal);
    vec3 lightDir = _PointLightPosition.xyz - f.WorldSpacePosition.xyz;    
    float lightDistance = length(lightDir);
    lightDir = normalize(lightDir);
    
    // 根据法线方向调整偏移
    float bias = max(10.0f * _Bias * (1.0f - dot(normal, lightDir)), _Bias);
    
    // map to [0;1] range by dividing by far_plane
    // write this as modified depth
    gl_FragDepth = (lightDistance + bias) / _FarPlane;
}
_!GLSL_FragmentProgram_End
}

















