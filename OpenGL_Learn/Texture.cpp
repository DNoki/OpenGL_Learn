#include "Texture.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION //通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了。
#include <stb_image.h>

using namespace std;


static std::vector<std::unique_ptr<Texture>> Textures;
static Texture * FindTexture(const string & name)
{
	for (auto& t : Textures)
	{
		if (t->Name == name)
		{
			return t.get();
		}
	}
	return nullptr;
}
static void AddTexture(unique_ptr<Texture> tex)
{
	if (FindTexture(tex->Name))
	{
		cout << "已存在同名Texture2D，添加失败。" << endl;
		return;
	}
	Textures.push_back(move(tex));
}


Texture::Texture(GLuint id, std::string name) : ID(id), Name(name) {}

Texture2D& Texture2D::LoadTexture2D(const std::string & path, GLenum wrapMode, GLenum scaleFilterMode)
{
	auto findResult = FindTexture(path);
	if (findResult)
	{
		return static_cast<Texture2D&>(*findResult);
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// 加载并生成纹理
	int texWidth, texHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);// 翻转纹理
	unsigned char *data = stbi_load(path.c_str(), &texWidth, &texHeight, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 4)
			format = GL_RGBA;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 1)
			format = GL_RED;

		glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);// 释放图像的内存
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
		throw exception("Failed to load texture");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleFilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleFilterMode);
	glBindTexture(GL_TEXTURE_2D, 0);

	//auto t = make_unique<Texture2D>(id, path);
	auto t = unique_ptr<Texture2D>(new Texture2D(id, path));
	auto result = t.get();
	AddTexture(move(t));
	//Scene::Textures2D.push_back(move(t));

	return *result;
}

unique_ptr<Texture2D> Texture2D::CreateTexture2D(unsigned int width, unsigned int height, GLenum format, GLenum wrapMode, GLenum scaleFilterMode)
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaleFilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaleFilterMode);
	glBindTexture(GL_TEXTURE_2D, 0);

	//return make_unique<Texture2D>(id, to_string(id));
	return unique_ptr<Texture2D>(new Texture2D(id, to_string(id)));
}

Texture2D::Texture2D(GLuint id, std::string name) :Texture(id, name) {}


TextureCube & TextureCube::LoadTextureCube(string name, vector<std::string> paths, GLenum wrapMode, GLenum scaleFilterMode)
{
	auto findResult = FindTexture(name);
	if (findResult)
	{
		return static_cast<TextureCube&>(*findResult);
	}

	if (paths.size() != 6)throw exception("给定的立方体贴图路径不是6个。");

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, nrChannels;
	for (size_t i = 0; i < paths.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);// 翻转纹理
		unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);// 释放图像的内存
		}
		else
		{
			std::cout << "Failed to load Cubemap texture" << std::endl;
			stbi_image_free(data);
			throw exception("Failed to load Cubemap texture");
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	auto t = unique_ptr<TextureCube>(new TextureCube(id, name));
	auto result = t.get();
	AddTexture(move(t));

	return *result;
}

TextureCube::TextureCube(GLuint id, std::string name) :Texture(id, name) {}