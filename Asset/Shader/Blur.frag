#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture2D;

const float offset = 1.0 / 300.0;  

// 反相
vec4 Inversion(vec4 texColor)
{
    return vec4(vec3(1.0 - texColor), 1.0);
}

// 灰度
vec4 Grayscale(vec4 texColor)
{
    //float average = (texColor.r + texColor.g + texColor.b) / 3.0f;// 灰度
    float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;// 加权灰度

    return vec4(average, average, average, 1.0f); // 灰度
}

// 核效果
vec4 Kernel()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), vec2( 0.0f, offset), vec2( offset, offset), // 上
        vec2(-offset, 0.0f), vec2( 0.0f, 0.0f), vec2( offset, 0.0f),   // 中
        vec2(-offset, -offset), vec2( 0.0f, -offset), vec2( offset, -offset)  // 下
    );
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture2D, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0f);
}

// 模糊
vec4 Blur()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), vec2( 0.0f, offset), vec2( offset, offset), // 上
        vec2(-offset, 0.0f), vec2( 0.0f, 0.0f), vec2( offset, 0.0f),   // 中
        vec2(-offset, -offset), vec2( 0.0f, -offset), vec2( offset, -offset)  // 下
    );
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture2D, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0f);
}

// 边缘检测
vec4 EdgeDetection()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), vec2( 0.0f, offset), vec2( offset, offset), // 上
        vec2(-offset, 0.0f), vec2( 0.0f, 0.0f), vec2( offset, 0.0f),   // 中
        vec2(-offset, -offset), vec2( 0.0f, -offset), vec2( offset, -offset)  // 下
    );
    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture2D, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0f);
}

void main()
{
    vec4 texColor = texture(texture2D, TexCoord);
    //color = Inversion(texColor);
    color = texColor;
    //color = Blur();
    
    //vec3 c = EdgeDetection().xyz;
    //float value = (c.x + c.y + c.z) / 3.0f;
    //color = vec4(value, value, value, 1.0f);
    
    //color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //color = vec4(TexCoord, 0.0f, 1.0f);
    
}

















