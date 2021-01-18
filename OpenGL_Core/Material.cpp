#include "pch.h"

#include "Material.h"

namespace OpenGL_Core
{
    bool  Material::UseShaderPass(unsigned int index)
    {
        if (index < GetPassCount())
        {
            Shaders[index]->UseShader(true);
            return true;
        }
        else return false;
    }

    Material::Material(const string& name, Shader* shader) :Material(name, { shader }) {}
    Material::Material(const string& name, initializer_list<Shader*> shaders) : ResourceObject(name), Shaders()
    {
        for (auto shader : shaders)
            AddShaderPass(shader);
    }
    Shader* Material::operator[](unsigned int i) { return Shaders[i]; }
}