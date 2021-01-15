#pragma once

#include <glad/glad.h>


namespace OpenGL_Core
{
    using namespace std;

    /// <summary>
    /// 顶点数组
    /// </summary>
    struct VertexArrays
    {
    public:
        GLuint ID;

        /// <summary>
        /// 绑定顶点数组
        /// </summary>
        inline void BindVertexArray() { glBindVertexArray(ID); }
        /// <summary>
        /// 解绑顶点数组
        /// </summary>
        inline void UnBindVertexArray() { glBindVertexArray(0); }

        /// <summary>
        /// 链接顶点属性
        /// </summary>
        /// <param name="index">顶点属性的位置值</param>
        /// <param name="size">顶点属性的大小</param>
        /// <param name="stride">步长</param>
        /// <param name="pointer">位置数据在缓冲中起始位置的偏移量</param>
        /// <param name="type">数据的类型</param>
        /// <param name="normalized">是否希望数据被标准化</param>
        void VertexAttribPointer(GLuint index, GLint size, GLsizei stride, const void* pointer, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE);

        VertexArrays();
        ~VertexArrays();
    };
}