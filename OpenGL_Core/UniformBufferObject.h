#pragma once

#include <glad/glad.h>

#include "ResourceObject.h"
#include "BufferObject.h"


namespace OpenGL_Core
{

    /// <summary>
    /// Uniform缓冲对象
    /// 定义一系列在多个着色器中相同的全局Uniform变量。
    /// </summary>
    class UniformBufferObject final :public ResourceObject
    {
    public:
        /// <summary>
        /// 绑定链接索引
        /// </summary>
        GLuint BindedIndex;
        /// <summary>
        /// 缓冲对象
        /// </summary>
        BufferObject Buffer;

        /// <summary>
        /// 输入偏移数据
        /// </summary>
        /// <param name="offset">偏移量</param>
        /// <param name="size"></param>
        /// <param name="data">数据</param>
        inline void SetSubData(GLintptr offset, GLsizeiptr size, const void* data)
        {
            //cout << "给定数据大小："<< size <<"  计算结果大小："<< sizeof(data) << endl;
            Buffer.BufferSubData(offset, size, data);
        }

        /// <summary>
        /// 创建UniformBufferObject
        /// </summary>
        /// <param name="name"></param>
        /// <param name="size">分配内存大小</param>
        /// <param name="index">绑定链接索引（不同Uniform缓冲对象在同一着色器中需要指定不同索引）</param>
        UniformBufferObject(const string& name, GLsizeiptr size, GLuint index) :ResourceObject(name), BindedIndex(index), Buffer(GL_UNIFORM_BUFFER)
        {
            this->Buffer.BindBuffer();
            this->Buffer.BufferData(size, NULL);
            this->Buffer.BindBufferRange(index, size);
        }
    };
}