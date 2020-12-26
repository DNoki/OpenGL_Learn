#include "BufferObject.h"

#include <iostream>

namespace OpenGL_Learn
{

    using namespace std;

    void BufferObject::BufferData(GLsizeiptr size, const void* data, GLenum usage)
    {
        BindBuffer();
        glBufferData(this->Type, size, data, usage);
    }

    void BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
    {
        BindBuffer();
        glBufferSubData(this->Type, offset, size, data);
    }

    BufferObject::BufferObject(GLenum bufferType)
    {
        GLuint id;
        glGenBuffers(1, &id);
        this->ID = id;
        this->Type = bufferType;

        //cout << "LOG:: " << "�������:: " << "ID=" << id << "������" << endl;
    }

    BufferObject::~BufferObject()
    {
        //auto id = this->ID;
        glDeleteBuffers(1, &this->ID);
        //cout << "LOG:: " << "�������:: " << "ID=" << id << "���ͷ�" << endl;
    }

}