#pragma once

#include <glad/glad.h>


namespace OpenGL_Learn
{
    // �������
    struct BufferObject
    {
    public:
        GLuint ID;
        // ��������
        // GL_ARRAY_BUFFER
        // GL_ELEMENT_ARRAY_BUFFER
        // GL_UNIFORM_BUFFER
        GLenum Type;

        // �󶨻���
        inline void BindBuffer() { glBindBuffer(Type, ID); }
        // ��󻺳�
        inline void UnBindBuffer() { glBindBuffer(Type, 0); }
        // ��������
        // size ���ݴ�С
        // data ����ָ�� glm::value_ptr(data)
        void BufferData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
        // ����ƫ������
        // offset ƫ����
        // size ���ݴ�С
        // data ����ָ�� glm::value_ptr(data)
        void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
        // ��offset��size��ʾ�Ļ��������󻺳����ķ�Χ�󶨵���...ָ����Ŀ������������������İ󶨵�
        inline void BindBufferRange(GLuint index, GLsizeiptr size, GLintptr offset = 0)
        {
            glBindBufferRange(this->Type, index, this->ID, offset, size);
        }

        BufferObject(GLenum bufferType);
        virtual ~BufferObject();
    };

}