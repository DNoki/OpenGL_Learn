#pragma once

#include <glad/glad.h>

#include "ResourceObject.h"
#include "BufferObject.h"


namespace OpenGL_Learn
{

    // Uniform缓冲对象 定义一系列在多个着色器中相同的全局Uniform变量。
    class UniformBufferObject final :public ResourceObject
    {
    public:
        // 绑定链接索引
        GLuint BindedIndex;
        // 缓冲对象
        BufferObject Buffer;

        // 输入偏移数据
        // @offset 偏移量
        // @data 数据
        inline void SetSubData(GLintptr offset, GLsizeiptr size, const void* data)
        {
            //cout << "给定数据大小："<< size <<"  计算结果大小："<< sizeof(data) << endl;
            Buffer.BufferSubData(offset, size, data);
        }


        // 创建UniformBufferObject
        // size 分配内存大小
        // index 绑定链接索引（不同Uniform缓冲对象在同一着色器中需要指定不同索引）
        UniformBufferObject(const string& name, GLsizeiptr size, GLuint index) :ResourceObject(name), BindedIndex(index), Buffer(GL_UNIFORM_BUFFER)
        {
            this->Buffer.BindBuffer();
            this->Buffer.BufferData(size, NULL);
            this->Buffer.BindBufferRange(index, size);
        }
    };
}