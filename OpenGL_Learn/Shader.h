#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>


// 渲染状态
struct RendererState
{
public:
	static void SetState(GLenum name, GLboolean enable);

	// 深度测试
	GLboolean DepthTest;
	GLboolean DepthMask;
	GLenum DepthTestMode;
	// 混合
	GLboolean Blend;
	GLenum Sfactor;
	GLenum Dfactor;
	GLenum SfactorAlpha;
	GLenum DfactorAlpha;
	// 面剔除
	GLboolean CullFace;
	GLenum CullFaceMode;
	// 正向面
	GLenum FrontFace;

	RendererState();
};

/*
设计一个结构体，用来接收程序输入的：材质数据，并在渲染时调用
设计一个可以改变着色器参数的方法，比如开启关闭深度测试
*/
class Shader
{
public:
	std::string ShaderName;
	// 着色器ID
	GLuint ID;
	RendererState State;

	// 查询显卡支持可传递多少顶点属性，一般为16个
	static int CheckMaxVertexAttribsCount();

	// 使用着色器
	void UseShader();

	inline void SetInt(const std::string &name, const int &value) const
	{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}
	inline void SetMatrix4(const std::string &name, const glm::mat4 &mat) const// const成员函数：防止成员函数修改类对象的内容
	{
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_TRUE, &mat[0][0]);
	}
	//@brief 绑定Uniform块
	//@param name Shader中的Uniform块名称
	//@param id 绑定的ID
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

	// 更新缓冲内容
	// offset 偏移量
	// size 数据大小
	// data 数据指针 glm::value_ptr(data)
	void SetSubBufferData(GLintptr offset, GLsizeiptr size, const void *data);

	UniformBufferObject() = default;
	// 创建UniformBufferObject
	// size 分配内存大小
	// index 绑定链接索引
	UniformBufferObject(GLsizeiptr size, GLuint index);
};