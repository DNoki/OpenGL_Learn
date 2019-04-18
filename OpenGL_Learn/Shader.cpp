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
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vPath);// 打开文件
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();// 读取文件的缓冲内容到数据流中
		vShaderFile.close();// 关闭文件处理器
		vertexCode = vShaderStream.str();// 转换数据流到string

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
		cout << "读取" << vPath << endl << "读取" << fPath << endl;
		if (gPath != NULL) cout << "读取" << gPath << endl;
		cout << "已绑定着色器程序ID：" << ID << endl << endl;
	}

	// 为所有着色器绑定统一的变换，Uniform缓冲对象
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

	// 编译顶点着色器
	{
		vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, &vertexCode, NULL);
		glCompileShader(vert);
		CheckShaderCompile(vert, "顶点");
	}

	// 编译片段着色器
	{
		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &fragmentCode, NULL);
		glCompileShader(frag);
		CheckShaderCompile(frag, "片段");
	}

	if (geometryCode != NULL)
	{
		// 编译几何着色器
		{
			geo = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geo, 1, &geometryCode, NULL);
			glCompileShader(geo);
			CheckShaderCompile(geo, "几何");
		}
	}

	// 着色器程序
	{
		id = glCreateProgram();
		glAttachShader(id, vert);
		glAttachShader(id, frag);
		if (geometryCode != NULL) glAttachShader(id, geo);
		glLinkProgram(id);
		// 检测编译是否成功
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

	// 释放顶点片段对象
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
