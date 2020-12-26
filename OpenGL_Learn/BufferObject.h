#pragma once

#include <glad/glad.h>


namespace OpenGL_Learn
{
    // 缓冲对象
    struct BufferObject
    {
    public:
        GLuint ID;
        // 缓冲类型
        // GL_ARRAY_BUFFER
        // GL_ELEMENT_ARRAY_BUFFER
        // GL_UNIFORM_BUFFER
        GLenum Type;

        // 绑定缓冲
        inline void BindBuffer() { glBindBuffer(Type, ID); }
        // 解绑缓冲
        inline void UnBindBuffer() { glBindBuffer(Type, 0); }
        // 输入数据
        // size 数据大小
        // data 数据指针 glm::value_ptr(data)
        void BufferData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        // 输入偏移数据
        // offset 偏移量
        // size 数据大小
        // data 数据指针 glm::value_ptr(data)
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
        // 将offset和size表示的缓冲区对象缓冲区的范围绑定到由...指定的目标数组的索引索引处的绑定点
        inline void BindBufferRange(GLuint index, GLsizeiptr size, GLintptr offset = 0)
        {
            glBindBufferRange(this->Type, index, this->ID, offset, size);
        }

        BufferObject(GLenum bufferType);
        virtual ~BufferObject();
    };

}