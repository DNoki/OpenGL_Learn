#version 330 core

// 接收的输入是经过插值计算的
in v2f
{
    vec3 PositionWorldSpace;
    vec3 NormalWorldSpace;
    vec2 TexCoord;
} f;

out vec4 FragColor;

layout (std140) uniform CAMERA
{
    vec4 CameraWorldSpace;
};

uniform vec4 uniformColor;
uniform sampler2D texture2D;
uniform samplerCube skybox;
uniform float time;

void main()
{
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //FragColor = vec4(f.NormalWorldSpace, 1.0f);
    //FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0f);
    FragColor = texture(texture2D, f.TexCoord) * time;
    return;

    // float ratio = 1.00 / 1.52;
    
    vec3 worldViewDir = normalize(f.PositionWorldSpace - vec3(CameraWorldSpace));
    vec3 worldRefle = reflect(worldViewDir, normalize(f.NormalWorldSpace));// 反射
    // vec3 worldRefra = refract(worldViewDir, normalize(f.NormalWorldSpace), ratio);// 折射
    vec4 texColor = vec4(texture(skybox, worldRefle).rgb, 1.0f)
    //vec4(vec3(gl_FragCoord.z), 1.0);
    *texture(texture2D, f.TexCoord); 
    // vec4(f.TexCoord, 0.0f, 1.0f);
    // vec4(f.NormalWorldSpace, 1.0f);
    // vec4(1.0f, 0.5f, 0.2f, 1.0f);
    
    //gl_FragCoord.xyz // gl_FragCoord的xy是视口空间坐标，原点为左下角，z分量等于对应片段的深度值
    //if(texColor.a < 0.1f) discard;// 将会丢弃该片段
    
    if(gl_FrontFacing)
        FragColor = texColor;
    else FragColor = texColor * vec4(0.5f, 0.5f, 0.5f, 1.0f);
}












