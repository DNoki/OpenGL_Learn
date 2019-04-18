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
	// 绑定的纹理单元
	unsigned int Unit;
	// 读取的贴图
	Texture* Tex;

	void UseTexture();

	BindedTexture(unsigned int unit, Texture* texture);
};

class Material
{
public:
	//@brief 绑定2D贴图
	//@param texture 要绑定的贴图
	//@param name 在Shader中声明的采样器名称
	//@param unit 绑定的纹理单元
	void BindTexture(Texture* texture, const std::string &name, unsigned int unit);

	void UseMaterial();

	inline Shader* GetShader() { return _shader; }

	Material(Shader& shader);
	Material(const std::string & shaderName);

private:
	// 材质使用的Shader
	Shader* _shader;
	// 绑定的贴图
	std::vector<BindedTexture> textures;
};


