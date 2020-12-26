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

        //cout << "LOG:: " << "��������:: " << "ID=" << id << "������" << endl;
    }

    VertexArrays::~VertexArrays()
    {
        //auto id = this->ID;
        glDeleteVertexArrays(1, &this->ID);
        //cout << "LOG:: " << "��������:: " << "ID=" << id << "���ͷ�" << endl;
    }
}