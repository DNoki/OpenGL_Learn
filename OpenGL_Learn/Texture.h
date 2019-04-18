#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>

class Texture
{
public:
	GLuint ID;
	// 名称用来防止重复加载
	std::string Name;

	virtual GLenum GetTextureType() = 0;

protected:
	Texture(GLuint id, std::string name);
};

class Texture2D :public Texture
{
public:
	//@brief 从文件读取2D贴图
	//@param path		路径
	//@param wrapMode   重复模式
	//@param scaleFilterMode	缩放模式
	//
	//@return 返回加载的贴图ID
	static Texture2D& LoadTexture2D(const std::string & path, GLenum wrapMode = GL_REPEAT, GLenum scaleFilterMode = GL_LINEAR);
	//@brief 创建空2D贴图
	//@param width 宽度
	//@param height 高度
	//@param format 格式：GL_RGB、GL_RGBA
	//@param wrapMode 重复模式
	//@param scaleFilterMode 缩放模式
	//
	//@return 返回贴图指针
	static std::unique_ptr<Texture2D> CreateTexture2D(unsigned int width, unsigned int height, GLenum format, GLenum wrapMode = GL_REPEAT, GLenum scaleFilterMode = GL_LINEAR);

	inline GLenum GetTextureType() { return GL_TEXTURE_2D; }

	//// 贴图存放路径 用于防止重复加载
	//std::string Path;
	//const GLenum TextureType;

private:
	Texture2D(GLuint id, std::string name);
};

class TextureCube :public Texture
{
public:
	static TextureCube& LoadTextureCube(std::string name, std::vector<std::string> paths, GLenum wrapMode = GL_CLAMP_TO_EDGE, GLenum scaleFilterMode = GL_LINEAR);
	
	inline GLenum GetTextureType() { return GL_TEXTURE_CUBE_MAP; }


private:
	TextureCube(GLuint id, std::string name);
};


//GLuint texture;
//glGenTextures(1, &texture);
//glBindTexture(GL_TEXTURE_2D, texture);
//// 为当前绑定的纹理对象设置环绕、过滤方式
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// 加载并生成纹理
//int texWidth, texHeight, nrChannels;
//unsigned char *data = stbi_load("Resource/TestImage.png", &texWidth, &texHeight, &nrChannels, 0);
//if (data)
//{
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//	cout << texWidth << "  " << texHeight << "  " << nrChannels << endl;
//	glGenerateMipmap(GL_TEXTURE_2D);
//}
//else std::cout << "Failed to load texture" << std::endl;
//stbi_image_free(data);// 释放图像的内存
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, texture);
//enum TextureType
//{
//	Diffuse,
//	Specular,
//	Normal,
//};