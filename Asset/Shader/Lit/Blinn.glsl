{_!Property_Start
{
    "RenderQueue": 2000,
    "_MainColor": [1, 1, 1, 1],
    "_MainTexture": [0, "White"],
    "_Shininess": 32
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
layout (location = 4) in vec2 texCoord;

out v2f
{
    vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
    vec2 uv;
    vec4 LightSpacePosition;
} f;

layout (std140) uniform TRANSFORM
{
    mat4 Matrix_P;
    mat4 Matrix_V;
    mat4 Matrix_M;
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

void main()
{
    f.WorldSpacePosition = Matrix_M * vec4(position, 1.0f);
    gl_Position = Matrix_P * Matrix_V * f.WorldSpacePosition;
    f.WorldSpaceNormal = transpose(inverse(mat3(Matrix_M))) * normal;
    f.uv = texCoord;
    f.LightSpacePosition = LightMatrix * f.WorldSpacePosition;
}
_!GLSL_VertexProgram_End
}
----------------------------------------------------------------------------
{_!GLSL_FragmentProgram_Start
#version 330 core

in v2f
{
    vec4 WorldSpacePosition;
    vec3 WorldSpaceNormal;
    vec2 uv;
    vec4 LightSpacePosition;
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

uniform float _Shininess;

uniform sampler2D _ShadowMap;

float ShadowCalculation(vec4 lightSpacePosition)
{
    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w; // ִ��͸�ӳ���
    if(projCoords.z > 1.0f) return 0.0f;
    projCoords = projCoords * 0.5f + 0.5f;
    //float closestDepth = texture(_ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //return currentDepth > closestDepth ? 1.0f : 0.0f; // ��ȣ�����������Ӱ������1
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    shadow /= 9.0;
    return shadow;
}

void main()
{
    vec3 normal = normalize(f.WorldSpaceNormal.xyz);
    vec3 lightDir = normalize(-WorldSpaceLightPos.xyz);
    vec3 viewDir = normalize(WorldSpaceCameraPos.xyz - f.WorldSpacePosition.xyz);
    
    vec4 texColor = texture(_MainTexture, f.uv);
    
    float lambert = max((dot(normal, lightDir)+1)*0.5f, 0.0f);
    float specular = pow(max(dot(normal, normalize(lightDir + viewDir)), 0.0f), _Shininess);
    //float shadow = 1.0f - ShadowCalculation(f.LightSpacePosition);
    float shadow = max(1.0f - ShadowCalculation(f.LightSpacePosition), 1.0f - ShadowStrength);
    
    //alpha*R(B)+(1-alpha)*R(A)
    vec3 color = (lambert + specular) 
    * shadow 
    * LightColor.rgb * texColor.rgb;
    
    //float alpha = texColor.a * _MainColor.a;
    //FragColor = vec4(vec3(1.0f - ShadowCalculation(f.LightSpacePosition)), 1.0f);
    FragColor = vec4(color, 1.0f);
    //FragColor = vec4(f.LightSpacePosition.xyz,1.0f);
}
_!GLSL_FragmentProgram_End
}