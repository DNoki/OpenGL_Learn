#include "pch.h"

#include "BufferObject.h"

namespace OpenGL_Core
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

        //cout << "LOG:: " << "缓冲对象:: " << "ID=" << id << "已生成" << endl;
    }

    BufferObject::~BufferObject()
    {
        //auto id = this->ID;
        glDeleteBuffers(1, &this->ID);
        //cout << "LOG:: " << "缓冲对象:: " << "ID=" << id << "已释放" << endl;
    }

}