{_!Property_Start
{
    "RenderQueue": 2000,
    "_MainColor": [1, 1, 1, 1],
    "_MainTexture": [0, "White"]
}
_!Property_End
}
    "_SpecularColor": [1, 1, 1, 1],
    "_SpecularTexture": [1, "White"],
    "_Shininess": 32,
    "_NormalTexture": [2, "Normal"],
    "_NormalStrength": 1
----------------------------------------------------------------------------
{_!GLSL_VertexProgram_Start
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 color;
layout (location = 4) in vec2 texCoord;

out v2f
{
    vec4 worldSpacePosition;
    vec3 worldSpaceNormal;
    vec2 uv;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
};

void main()
{
    f.worldSpacePosition = Matrix_M * vec4(position, 1.0f);
    gl_Position = Matrix_P * Matrix_V * f.worldSpacePosition;
    f.worldSpaceNormal = transpose(inverse(mat3(Matrix_M))) * normal;
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
    vec4 worldSpacePosition;
    vec3 worldSpaceNormal;
    vec2 uv;
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
    // ����ռ����λ��
    vec4 WorldSpaceCameraPos;
    // x��������Ӱ���Ŀ�ȣ�y��������Ӱ���ĸ߶ȣ�zΪ��߱ȣ�w��������Ӱ��ʱΪ1.0��͸��ʱΪ0.0��
    vec4 OrthoParams;
};

#define POINT_LIGHTS_COUNT 4
layout (std140) uniform LIGHTING_DATA
{
    vec4 LightColor; // ������ɫ
    vec4 WorldSpaceLightPos; // ƽ�й⣺������ռ䷽��, 0���������⣺������ռ�λ��, 1����
    mat4 LightMatrix; // ���絽��ľ������ڲ���Cookie��˥������
    float ShadowStrength; // ��Ӱǿ�ȡ�
    // ǰ����Ⱦʹ��4�����Դ
    vec4[POINT_LIGHTS_COUNT] PointLight4Pos; // ���Դλ��
    vec4[POINT_LIGHTS_COUNT] PointLight4Color; // ���Դ��ɫ
    vec4[POINT_LIGHTS_COUNT] PointLight4Info; // ���Դ˥�� C=1.0, L=x, Q=y ˥��ǿ�� = 1.0 / (C + L * d + Q * d^2) �����Դ������z�����ԴԶ����w
};

layout (std140) uniform TIME
{
    // (RunTime, SinTime, CosTime, DeltaTime);
    vec4 Time;
};

uniform vec4 _MainColor;
uniform sampler2D _MainTexture;
//uniform samplerCube _CubeTexture;

void main()
{
    vec3 normal = normalize(f.worldSpaceNormal.xyz);
    vec3 lightDir = normalize(-WorldSpaceLightPos.xyz);
    
    vec3 color = LightColor.rgb * max((dot(normal, lightDir)+0)/1, 0.0f) * texture(_MainTexture, f.uv).rgb;
    //float value = ;
    //FragColor = vec4(value,value,value,1.0f);
    FragColor = vec4(color, 1.0f);
    
    //vec3 viewDir = normalize(WorldSpaceCameraPos.xyz - f.worldSpacePosition.xyz);
    //float value = 1.0f - dot(normal, viewDir); // ������Ч��
}
_!GLSL_FragmentProgram_End
}