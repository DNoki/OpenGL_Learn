#pragma once

#include <string>
#include <initializer_list>
#include <glad/glad.h>

#include "Shader.h"

namespace OpenGL_Core
{
    using namespace std;

    /// <summary>
    /// 材质（包含多个着色器Pass）
    /// </summary>
    class Material final : public ResourceObject
    {
    public:
        /// <summary>
        /// 材质使用的Shader
        /// </summary>
        List<Shader*> Shaders;

        inline unsigned int GetPassCount() { return (unsigned int)Shaders.size(); }
        inline Shader* GetMainShader() { return GetPassCount() > 0 ? Shaders[0] : nullptr; }
        /// <summary>
        /// 添加着色器通道
        /// </summary>
        /// <param name="shader"></param>
        inline void AddShaderPass(Shader* shader) { if (shader) Shaders.push_back(shader); }
        bool UseShaderPass(unsigned int index);
        //// 设置整形
        //inline void SetInt(const unsigned int& index, const string& name, const int& value)
        //{
        //    if (index < GetPassCount())
        //        this->Shaders[index]->SetInt(name, value);
        //}
        //// 设置Float
        //inline void SetFloat(const unsigned int& index, const string& name, const float& value)
        //{
        //    if (index < GetPassCount())
        //        this->Shaders[index]->SetFloat(name, value);
        //}
        //// 设置Vector4
        //inline void SetVector4(const unsigned int& index, const string& name, const Vector4& value)
        //{
        //    if (index < GetPassCount())
        //        this->Shaders[index]->SetVector4(name, value);
        //}
        //// 设置Matrix4x4
        //inline void SetMatrix4x4(const unsigned int& index, const string& name, const Matrix4x4& value)// const成员函数：防止成员函数修改类对象的内容
        //{
        //    if (index < GetPassCount())
        //        this->Shaders[index]->SetMatrix4x4(name, value);
        //}

        Material(const string& name, Shader* shader);
        Material(const string& name, initializer_list<Shader*> shaders);

        Shader* operator[](unsigned int i);

    private:

    };



}

