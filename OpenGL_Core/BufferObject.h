#pragma once

#include <glad/glad.h>


namespace OpenGL_Core
{
    // 缓冲对象
    struct BufferObject
    {
    public:
        GLuint ID;
        /// <summary>
        /// 缓冲类型
        /// GL_ARRAY_BUFFER
        /// GL_ELEMENT_ARRAY_BUFFER
        /// GL_UNIFORM_BUFFER
        /// </summary>
        GLenum Type;

        /// <summary>
        /// 绑定缓冲
        /// </summary>
        inline void BindBuffer() { glBindBuffer(Type, ID); }
        /// <summary>
        /// 解绑缓冲
        /// </summary>
        inline void UnBindBuffer() { glBindBuffer(Type, 0); }
        /// <summary>
        /// 输入数据
        /// </summary>
        /// <param name="size">数据大小</param>
        /// <param name="data">数据指针 glm::value_ptr(data)</param>
        /// <param name="usage"></param>
        void BufferData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        /// <summary>
        /// 输入偏移数据
        /// </summary>
        /// <param name="offset">偏移量</param>
        /// <param name="size">数据大小</param>
        /// <param name="data">数据指针 glm::value_ptr(data)</param>
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
        /// <summary>
        /// 将offset和size表示的缓冲区对象缓冲区的范围绑定到由...指定的目标数组的索引索引处的绑定点
        /// </summary>
        /// <param name="index"></param>
        /// <param name="size"></param>
        /// <param name="offset"></param>
        inline void BindBufferRange(GLuint index, GLsizeiptr size, GLintptr offset = 0)
        {
            glBindBufferRange(this->Type, index, this->ID, offset, size);
        }

        BufferObject(GLenum bufferType);
        virtual ~BufferObject();
    };

}