#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int Shader::CheckMaxVertexAttribsCount()
{
	GLint n;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
	//std::cout << n << std::endl;
	return n;
}

void Shader::UseShader()
{
	glUseProgram(this->ID);
}

Shader::Shader() {}
Shader::Shader(const string& name, const char * vPath, const char * fPath, const char* gPath) :Shader()
{	
	string vertexCode;
	string fragmentCode;
	string geometryCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream gShaderFile;
	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vPath);// ���ļ�
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();// ��ȡ�ļ��Ļ������ݵ���������
		vShaderFile.close();// �ر��ļ�������
		vertexCode = vShaderStream.str();// ת����������string

		fShaderFile.open(fPath);
		std::stringstream fShaderStream;
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		fragmentCode = fShaderStream.str();

		if (gPath != NULL)
		{
			gShaderFile.open(gPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	this->ID = CompileShader(vertexCode.c_str(), fragmentCode.c_str(), gPath != NULL ? geometryCode.c_str() : NULL);
	{
		cout << "��ȡ" << vPath << endl << "��ȡ" << fPath << endl;
		if (gPath != NULL) cout << "��ȡ" << gPath << endl;
		cout << "�Ѱ���ɫ������ID��" << ID << endl << endl;
	}

	// Ϊ������ɫ����ͳһ�ı任��Uniform�������
	glUniformBlockBinding(this->ID, glGetUniformBlockIndex(this->ID, "Transform"), 0);

	this->ShaderName = name;
}

void Shader::CheckShaderCompile(GLuint shaderID, const string& type)
{
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

GLuint Shader::CompileShader(const char * vertexCode, const char * fragmentCode, const char * geometryCode)
{
	GLuint vert, geo, frag, id;

	// ���붥����ɫ��
	{
		vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, &vertexCode, NULL);
		glCompileShader(vert);
		CheckShaderCompile(vert, "����");
	}

	// ����Ƭ����ɫ��
	{
		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &fragmentCode, NULL);
		glCompileShader(frag);
		CheckShaderCompile(frag, "Ƭ��");
	}

	if (geometryCode != NULL)
	{
		// ���뼸����ɫ��
		{
			geo = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geo, 1, &geometryCode, NULL);
			glCompileShader(geo);
			CheckShaderCompile(geo, "����");
		}
	}

	// ��ɫ������
	{
		id = glCreateProgram();
		glAttachShader(id, vert);
		glAttachShader(id, frag);
		if (geometryCode != NULL) glAttachShader(id, geo);
		glLinkProgram(id);
		// �������Ƿ�ɹ�
		GLint success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
			throw;
		}
	}

	// �ͷŶ���Ƭ�ζ���
	glDeleteShader(vert);
	glDeleteShader(frag);

	return id;
}

void UniformBufferObject::SetSubBufferData(GLintptr offset, GLsizeiptr size, const void * data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->ID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferObject::UniformBufferObject(GLsizeiptr size, GLuint index)
{
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, index, id, 0, size);
	this->ID = id;
	this->BindedID = index;
}

void RendererState::SetState(GLenum name, GLboolean enable)
{
	if (enable)
	{
		glEnable(name);
	}
	else
	{
		glDisable(name);
	}
}

RendererState::RendererState() :
	DepthTest(GL_FALSE), DepthMask(GL_TRUE), DepthTestMode(GL_LESS),
	Blend(GL_FALSE), Sfactor(GL_SRC_ALPHA), Dfactor(GL_ONE_MINUS_SRC_ALPHA), SfactorAlpha(GL_SRC_ALPHA), DfactorAlpha(GL_ONE_MINUS_SRC_ALPHA),
	CullFace(GL_FALSE), CullFaceMode(GL_BACK),
	FrontFace(GL_CCW)
{
}
