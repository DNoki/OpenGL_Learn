#pragma once

#include <glad/glad.h>


namespace OpenGL_Learn
{
    using namespace std;

    // 顶点数组
    struct VertexArrays
    {
    public:
        GLuint ID;

        // 绑定顶点数组
        inline void BindVertexArray() { glBindVertexArray(ID); }
        // 解绑顶点数组
        inline void UnBindVertexArray() { glBindVertexArray(0); }

        // 链接顶点属性
        // @index 顶点属性的位置值
        // @size 顶点属性的大小
        // @stride 步长
        // @pointer 位置数据在缓冲中起始位置的偏移量
        // @type 数据的类型
        // @normalized 是否希望数据被标准化
        void VertexAttribPointer(GLuint index, GLint size, GLsizei stride, const void* pointer, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE);

        VertexArrays();
        ~VertexArrays();
    };
}