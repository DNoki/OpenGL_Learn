#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>

class Texture
{
public:
	GLuint ID;
	// ����������ֹ�ظ�����
	std::string Name;

	virtual GLenum GetTextureType() = 0;

protected:
	Texture(GLuint id, std::string name);
};

class Texture2D :public Texture
{
public:
	//@brief ���ļ���ȡ2D��ͼ
	//@param path		·��
	//@param wrapMode   �ظ�ģʽ
	//@param scaleFilterMode	����ģʽ
	//
	//@return ���ؼ��ص���ͼID
	static Texture2D& LoadTexture2D(const std::string & path, GLenum wrapMode = GL_REPEAT, GLenum scaleFilterMode = GL_LINEAR);
	//@brief ������2D��ͼ
	//@param width ���
	//@param height �߶�
	//@param format ��ʽ��GL_RGB��GL_RGBA
	//@param wrapMode �ظ�ģʽ
	//@param scaleFilterMode ����ģʽ
	//
	//@return ������ͼָ��
	static std::unique_ptr<Texture2D> CreateTexture2D(unsigned int width, unsigned int height, GLenum format, GLenum wrapMode = GL_REPEAT, GLenum scaleFilterMode = GL_LINEAR);

	inline GLenum GetTextureType() { return GL_TEXTURE_2D; }

	//// ��ͼ���·�� ���ڷ�ֹ�ظ�����
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
//// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//// ���ز���������
//int texWidth, texHeight, nrChannels;
//unsigned char *data = stbi_load("Resource/TestImage.png", &texWidth, &texHeight, &nrChannels, 0);
//if (data)
//{
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//	cout << texWidth << "  " << texHeight << "  " << nrChannels << endl;
//	glGenerateMipmap(GL_TEXTURE_2D);
//}
//else std::cout << "Failed to load texture" << std::endl;
//stbi_image_free(data);// �ͷ�ͼ����ڴ�
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, texture);
//enum TextureType
//{
//	Diffuse,
//	Specular,
//	Normal,
//};