#include "VertexArrays.h"

#include <iostream>

namespace OpenGL_Learn
{
    void VertexArrays::VertexAttribPointer(GLuint index, GLint size, GLsizei stride, const void* pointer, GLenum type, GLboolean normalized)
    {
        BindVertexArray();
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    VertexArrays::VertexArrays()
    {
        GLuint id;
        glGenVertexArrays(1, &id);
        this->ID = id;

        //cout << "LOG:: " << "顶点数组:: " << "ID=" << id << "已生成" << endl;
    }

    VertexArrays::~VertexArrays()
    {
        //auto id = this->ID;
        glDeleteVertexArrays(1, &this->ID);
        //cout << "LOG:: " << "顶点数组:: " << "ID=" << id << "已释放" << endl;
    }
}