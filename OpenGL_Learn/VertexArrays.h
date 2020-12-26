#pragma once

#include <glad/glad.h>


namespace OpenGL_Learn
{
    using namespace std;

    // ��������
    struct VertexArrays
    {
    public:
        GLuint ID;

        // �󶨶�������
        inline void BindVertexArray() { glBindVertexArray(ID); }
        // ��󶥵�����
        inline void UnBindVertexArray() { glBindVertexArray(0); }

        // ���Ӷ�������
        // @index �������Ե�λ��ֵ
        // @size �������ԵĴ�С
        // @stride ����
        // @pointer λ�������ڻ�������ʼλ�õ�ƫ����
        // @type ���ݵ�����
        // @normalized �Ƿ�ϣ�����ݱ���׼��
        void VertexAttribPointer(GLuint index, GLint size, GLsizei stride, const void* pointer, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE);

        VertexArrays();
        ~VertexArrays();
    };
}