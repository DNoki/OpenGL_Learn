#pragma once

#include <glad/glad.h>

#include "ResourceObject.h"
#include "BufferObject.h"


namespace OpenGL_Learn
{

    // Uniform������� ����һϵ���ڶ����ɫ������ͬ��ȫ��Uniform������
    class UniformBufferObject final :public ResourceObject
    {
    public:
        // ����������
        GLuint BindedIndex;
        // �������
        BufferObject Buffer;

        // ����ƫ������
        // @offset ƫ����
        // @data ����
        inline void SetSubData(GLintptr offset, GLsizeiptr size, const void* data)
        {
            //cout << "�������ݴ�С��"<< size <<"  ��������С��"<< sizeof(data) << endl;
            Buffer.BufferSubData(offset, size, data);
        }


        // ����UniformBufferObject
        // size �����ڴ��С
        // index ��������������ͬUniform���������ͬһ��ɫ������Ҫָ����ͬ������
        UniformBufferObject(const string& name, GLsizeiptr size, GLuint index) :ResourceObject(name), BindedIndex(index), Buffer(GL_UNIFORM_BUFFER)
        {
            this->Buffer.BindBuffer();
            this->Buffer.BufferData(size, NULL);
            this->Buffer.BindBufferRange(index, size);
        }
    };
}