#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>


// ��Ⱦ״̬
struct RendererState
{
public:
	static void SetState(GLenum name, GLboolean enable);

	// ��Ȳ���
	GLboolean DepthTest;
	GLboolean DepthMask;
	GLenum DepthTestMode;
	// ���
	GLboolean Blend;
	GLenum Sfactor;
	GLenum Dfactor;
	GLenum SfactorAlpha;
	GLenum DfactorAlpha;
	// ���޳�
	GLboolean CullFace;
	GLenum CullFaceMode;
	// ������
	GLenum FrontFace;

	RendererState();
};

/*
���һ���ṹ�壬�������ճ�������ģ��������ݣ�������Ⱦʱ����
���һ�����Ըı���ɫ�������ķ��������翪���ر���Ȳ���
*/
class Shader
{
public:
	std::string ShaderName;
	// ��ɫ��ID
	GLuint ID;
	RendererState State;

	// ��ѯ�Կ�֧�ֿɴ��ݶ��ٶ������ԣ�һ��Ϊ16��
	static int CheckMaxVertexAttribsCount();

	// ʹ����ɫ��
	void UseShader();

	inline void SetInt(const std::string &name, const int &value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	inline void SetMatrix4(const std::string &name, const glm::mat4 &mat) const// const��Ա��������ֹ��Ա�����޸�����������
	{
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_TRUE, &mat[0][0]);
	}
	//@brief ��Uniform��
	//@param name Shader�е�Uniform������
	//@param id �󶨵�ID
	inline void SetUniformBlockBinding(const std::string& name, int id)
	{
		glUniformBlockBinding(this->ID, glGetUniformBlockIndex(this->ID, name.c_str()), id);
	}


	Shader();
	Shader(const std::string& name, const char * vPath, const char * fPath, const char* gPath = NULL);

private:
	void CheckShaderCompile(GLuint shaderID, const std::string& type);
	GLuint CompileShader(const char * vertexCode, const char * fragmentCode, const char * geometry = NULL);
};

struct UniformBufferObject
{
public:
	GLuint ID;
	GLuint BindedID;

	// ���»�������
	// offset ƫ����
	// size ���ݴ�С
	// data ����ָ�� glm::value_ptr(data)
	void SetSubBufferData(GLintptr offset, GLsizeiptr size, const void *data);

	UniformBufferObject() = default;
	// ����UniformBufferObject
	// size �����ڴ��С
	// index ����������
	UniformBufferObject(GLsizeiptr size, GLuint index);
};