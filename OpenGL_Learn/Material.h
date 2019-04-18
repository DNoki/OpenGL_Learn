#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"

struct BindedTexture
{
public:
	// �󶨵�����Ԫ
	unsigned int Unit;
	// ��ȡ����ͼ
	Texture* Tex;

	void UseTexture();

	BindedTexture(unsigned int unit, Texture* texture);
};

class Material
{
public:
	//@brief ��2D��ͼ
	//@param texture Ҫ�󶨵���ͼ
	//@param name ��Shader�������Ĳ���������
	//@param unit �󶨵�����Ԫ
	void BindTexture(Texture* texture, const std::string &name, unsigned int unit);

	void UseMaterial();

	inline Shader* GetShader() { return _shader; }

	Material(Shader& shader);
	Material(const std::string & shaderName);

private:
	// ����ʹ�õ�Shader
	Shader* _shader;
	// �󶨵���ͼ
	std::vector<BindedTexture> textures;
};


