#include "Material.h"

#include <iostream>

using namespace std;

extern Shader* FindShader(const string& name);
extern void UseRendererState(const RendererState& state);

void BindedTexture::UseTexture()
{
	if (!(this->Tex))
	{
		cout << "��ͼָ��ָ��Ķ��󲻴��ڡ�" << endl;
		return;
	}
	glActiveTexture(GL_TEXTURE0 + this->Unit);// ��������Ԫ
	glBindTexture(Tex->GetTextureType(), Tex->ID);// �󶨶�Ӧ��ͼid
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
		cout << "Shaderָ��ָ��Ķ��󲻴��ڡ�" << endl;
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