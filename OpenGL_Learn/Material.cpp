#include "Material.h"

#include <iostream>

using namespace std;

extern Shader* FindShader(const string& name);
extern void UseRendererState(const RendererState& state);

void BindedTexture::UseTexture()
{
	if (!(this->Tex))
	{
		cout << "贴图指针指向的对象不存在。" << endl;
		return;
	}
	glActiveTexture(GL_TEXTURE0 + this->Unit);// 激活纹理单元
	glBindTexture(Tex->GetTextureType(), Tex->ID);// 绑定对应贴图id
}

BindedTexture::BindedTexture(unsigned int unit, Texture* texture) :Unit(unit), Tex(texture) {}


void Material::BindTexture(Texture* texture, const std::string &name, unsigned int unit)
{
	this->_shader->UseShader();
	this->_shader->SetInt(name, unit);

	BindedTexture bindedTex(unit, texture);
	this->textures.push_back(bindedTex);
}

void Material::UseMaterial()
{
	if (!(this->_shader))
	{
		cout << "Shader指针指向的对象不存在。" << endl;
		return;
	}
	UseRendererState(this->_shader->State);
	this->_shader->UseShader();

	for (size_t i = 0; i < Material::textures.size(); i++)
	{
		this->textures[i].UseTexture();
	}
}

Material::Material(Shader& shader) :_shader(&shader) {}
Material::Material(const std::string & shaderName) : Material(*FindShader(shaderName)) {}