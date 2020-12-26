#include "Shader.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

#include "json.hpp"

#include "SceneManager.h"

#include "Math.h"
#include "GameSystem.h"
#include "UniformManager.h"


namespace OpenGL_Learn
{
    string ReadFile(const string& path)
    {
        string text;
        ifstream vShaderFile;
        // ��֤ifstream��������׳��쳣��
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try
        {
            vShaderFile.open(path);// ���ļ�
            stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();// ��ȡ�ļ��Ļ������ݵ���������
            vShaderFile.close();// �ر��ļ�������
            text = vShaderStream.str();// ת����������string
        }
        catch (ifstream::failure e)
        {
            cout << "ERROR::��ȡ�ļ�ʧ�ܡ�" << path << endl;
            throw;
        }

        cout << "LOG:: ��ɫ��:: " << "�Ѷ�ȡ�ļ�" << path << endl;
        return text;
    }

    void ShaderState::SetState(GLenum name, bool enable)
    {
        if (enable) glEnable(name);
        else glDisable(name);
    }

    map<int, unsigned int> Shader::Counter = map<int, unsigned int>();
    unique_ptr<ShaderState> Shader::realShaderState = nullptr;
    map<int, map<int, int>> Shader::StaticIntUniforms = map<int, map<int, int>>();
    map<int, map<int, float>> Shader::StaticFloatUniforms = map<int, map<int, float>>();
    map<int, map<int, Vector4>> Shader::StaticVector4Uniforms = map<int, map<int, Vector4>>();
    map<int, map<int, Matrix4x4>> Shader::StaticMatrix4x4Uniforms = map<int, map<int, Matrix4x4>>();

    // ʹ��ָ����Ⱦ״̬
    void Shader::UseState(const ShaderState& state)
    {
        // ��Ȳ���
        if (realShaderState->DepthTest != state.DepthTest)
        {
            realShaderState->DepthTest = state.DepthTest;
            realShaderState->DepthTestMode = state.DepthTestMode;
            realShaderState->DepthMask = state.DepthMask;

            ShaderState::SetState(GL_DEPTH_TEST, realShaderState->DepthTest);
            glDepthFunc((GLenum)realShaderState->DepthTestMode);
            glDepthMask(realShaderState->DepthMask);
        }
        else
        {
            if (realShaderState->DepthTest &&
                (realShaderState->DepthTestMode != state.DepthTestMode || realShaderState->DepthMask != state.DepthMask))
            {
                realShaderState->DepthTestMode = state.DepthTestMode;
                realShaderState->DepthMask = state.DepthMask;
                glDepthFunc((GLenum)realShaderState->DepthTestMode);
                glDepthMask(realShaderState->DepthMask);
            }
        }
        // ģ�����
        if (realShaderState->Stencil != state.Stencil)
        {
            realShaderState->Stencil = state.Stencil;
            realShaderState->StencilMask = state.StencilMask;
            realShaderState->StencilTestMode = state.StencilTestMode; realShaderState->StencilRef = state.StencilRef; realShaderState->StencilRefMask = state.StencilRefMask;
            realShaderState->StencilFail = state.StencilFail; realShaderState->StencilZfail = state.StencilZfail; realShaderState->StencilZpass = state.StencilZpass;

            ShaderState::SetState(GL_STENCIL_TEST, realShaderState->Stencil);
            glStencilMask(realShaderState->StencilMask); // д��ģ�建��������ֵ��ʹ��λ����ķ�ʽ�����Ƿ����д��ģ�建����
            glStencilFunc((GLenum)realShaderState->StencilTestMode, realShaderState->StencilRef, realShaderState->StencilRefMask); // ����ָ��ģ����Եĺ���������ָ����ʲô�����ͨ��ģ����ԡ�
            glStencilOp((GLenum)realShaderState->StencilFail, (GLenum)realShaderState->StencilZfail, (GLenum)realShaderState->StencilZpass);
        }
        else
        {
            if (realShaderState->Stencil &&
                (realShaderState->StencilMask != state.StencilMask || realShaderState->StencilTestMode != state.StencilTestMode ||
                    realShaderState->StencilRef != state.StencilRef || realShaderState->StencilRefMask != state.StencilRefMask ||
                    realShaderState->StencilFail != state.StencilFail || realShaderState->StencilZfail != state.StencilZfail || realShaderState->StencilZpass != state.StencilZpass))
            {
                realShaderState->StencilMask = state.StencilMask;
                realShaderState->StencilTestMode = state.StencilTestMode; realShaderState->StencilRef = state.StencilRef; realShaderState->StencilRefMask = state.StencilRefMask;
                realShaderState->StencilFail = state.StencilFail; realShaderState->StencilZfail = state.StencilZfail; realShaderState->StencilZpass = state.StencilZpass;
                glStencilMask(realShaderState->StencilMask);
                glStencilFunc((GLenum)realShaderState->StencilTestMode, realShaderState->StencilRef, realShaderState->StencilRefMask);
                glStencilOp((GLenum)realShaderState->StencilFail, (GLenum)realShaderState->StencilZfail, (GLenum)realShaderState->StencilZpass);
            }
        }
        // ���
        if (realShaderState->Blend != state.Blend)
        {
            realShaderState->Blend = state.Blend;
            realShaderState->BlendSfactor = state.BlendSfactor; realShaderState->BlendDfactor = state.BlendDfactor;
            realShaderState->BlendSfactorAlpha = state.BlendSfactorAlpha; realShaderState->BlendDfactorAlpha = state.BlendDfactorAlpha;

            ShaderState::SetState(GL_BLEND, realShaderState->Blend);
            glBlendFuncSeparate((GLenum)realShaderState->BlendSfactor, (GLenum)realShaderState->BlendDfactor,
                (GLenum)realShaderState->BlendSfactorAlpha, (GLenum)realShaderState->BlendDfactorAlpha);
        }
        else
        {
            if (realShaderState->Blend &&
                (realShaderState->BlendSfactor != state.BlendSfactor || realShaderState->BlendDfactor != state.BlendDfactor ||
                    realShaderState->BlendSfactorAlpha != state.BlendSfactorAlpha || realShaderState->BlendDfactorAlpha != state.BlendDfactorAlpha))
            {
                realShaderState->BlendSfactor = state.BlendSfactor; realShaderState->BlendDfactor = state.BlendDfactor; realShaderState->BlendSfactorAlpha = state.BlendSfactorAlpha; realShaderState->BlendDfactorAlpha = state.BlendDfactorAlpha;
                glBlendFuncSeparate((GLenum)realShaderState->BlendSfactor, (GLenum)realShaderState->BlendDfactor,
                    (GLenum)realShaderState->BlendSfactorAlpha, (GLenum)realShaderState->BlendDfactorAlpha);
            }
        }
        // ���޳�
        if (realShaderState->CullFace != state.CullFace)
        {
            realShaderState->CullFace = state.CullFace;
            realShaderState->CullFaceMode = state.CullFaceMode;
            ShaderState::SetState(GL_CULL_FACE, realShaderState->CullFace);
            glCullFace((GLenum)realShaderState->CullFaceMode);
        }
        else
        {
            if (realShaderState->CullFace && realShaderState->CullFaceMode != state.CullFaceMode)
            {
                realShaderState->CullFaceMode = state.CullFaceMode;
                glCullFace((GLenum)realShaderState->CullFaceMode);
            }
        }
        // ������
        if (realShaderState->FrontFace != state.FrontFace)
        {
            realShaderState->FrontFace = state.FrontFace;
            glFrontFace(realShaderState->FrontFace);
        }
    }

    int Shader::CheckMaxVertexAttribsCount()
    {
        GLint n;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
        return n;
    }


    void Shader::BindTexture(Texture& texture, const string& name, unsigned int unit)
    {
        if (!SetInt(name, unit)) // ָ������Ԫ
            return;
        if (_textures.count(unit) == 0)
            _textures.insert(make_pair(unit, &texture));
        else _textures[unit] = &texture;
    }

    void Shader::UseShader(bool willRender)
    {
        glUseProgram(this->ID);
        if (willRender)
        {
            UseState(this->State);

            GameSystem::PrintError("����Uniformǰ��");
            for (auto uniform : _intUniforms)
                // �Ƚ���ֵ�ͱ���ֵ�Ƿ����
                if (StaticIntUniforms[this->ID][uniform.first] != uniform.second)
                {
                    StaticIntUniforms[this->ID][uniform.first] = uniform.second;
                    glUniform1i(uniform.first, StaticIntUniforms[this->ID][uniform.first]);
                }
            for (auto uniform : _floatUniforms)
                if (!Math::Approximately(StaticFloatUniforms[this->ID][uniform.first], uniform.second))
                {
                    StaticFloatUniforms[this->ID][uniform.first] = uniform.second;
                    glUniform1f(uniform.first, StaticFloatUniforms[this->ID][uniform.first]);
                }
            for (auto uniform : _vector4Uniforms)
                if (StaticVector4Uniforms[this->ID][uniform.first] != uniform.second)
                {
                    StaticVector4Uniforms[this->ID][uniform.first] = uniform.second;
                    auto& value = StaticVector4Uniforms[this->ID][uniform.first];
                    glUniform4f(uniform.first, value.x, value.y, value.z, value.w);
                }
            for (auto uniform : _matrix4x4Uniforms)
                if (StaticMatrix4x4Uniforms[this->ID][uniform.first] != uniform.second)
                {
                    StaticMatrix4x4Uniforms[this->ID][uniform.first] = uniform.second;
                    glUniformMatrix4fv(uniform.first, 1, false, StaticMatrix4x4Uniforms[this->ID][uniform.first].GetPtr());
                }
            GameSystem::PrintError("����Uniform��");

            GameSystem::PrintError("����ͼǰ��");
            // ����󶨵���ͼ 
            for (auto tex : _textures)
            {
                glActiveTexture(GL_TEXTURE0 + tex.first);// ��������Ԫ
                tex.second->BindTexture(); // �󶨶�Ӧ��ͼ
            }
            GameSystem::PrintError("����ͼ��");
            //for (unsigned int i = 0; i < _textures.size(); i++)
            //    this->_textures[i].UseTexture();
        }
    }

    List<Tuple2<UniformType, string>> Shader::GetAllUniforms()
    {
        auto list = List<Tuple2<UniformType, string>>();
        size_t count;
        glGetProgramiv(this->ID, GL_ACTIVE_UNIFORMS, (int*)&count);

        const int bufSize = 256;
        int length, size;
        UniformType type;
        GLchar name[bufSize];
        for (size_t i = 0; i < count; i++)
        {
            glGetActiveUniform(this->ID, i, bufSize, &length, &size, (GLenum*)&type, name);
            if (name[0] != '_' || (
                type != UniformType::INT &&
                type != UniformType::FLOAT &&
                type != UniformType::FLOAT_VEC4 &&
                type != UniformType::SAMPLER_2D &&
                type != UniformType::SAMPLER_CUBE
                )) continue;
            list.push_back(Tuple2<UniformType, string>(type, name));
        }
        return list;
    }

    bool Shader::SetInt(const int& location, const int& value)
    {
        if (location < 0) return false;

        if (StaticIntUniforms.count(this->ID) == 0)
            StaticIntUniforms.insert(make_pair(this->ID, map<int, int>()));
        if (StaticIntUniforms[this->ID].count(location) == 0)
        {
            StaticIntUniforms[this->ID].insert(make_pair(location, value));
            this->UseShader();
            glUniform1i(location, value);
        }
        if (_intUniforms.count(location) == 0)
            _intUniforms.insert(make_pair(location, value));
        else _intUniforms[location] = value;
        return true;
    }
    bool Shader::SetFloat(const int& location, const float& value)
    {
        if (location < 0) return false;

        if (StaticFloatUniforms.count(this->ID) == 0)
            StaticFloatUniforms.insert(make_pair(this->ID, map<int, float>()));
        if (StaticFloatUniforms[this->ID].count(location) == 0)
        {
            StaticFloatUniforms[this->ID].insert(make_pair(location, value));
            this->UseShader();
            glUniform1f(location, value);
        }
        if (_floatUniforms.count(location) == 0)
            _floatUniforms.insert(make_pair(location, value));
        else _floatUniforms[location] = value;
        return true;
    }
    bool Shader::SetVector4(const int& location, const Vector4& value)
    {
        if (location < 0) return false;

        if (StaticVector4Uniforms.count(this->ID) == 0)
            StaticVector4Uniforms.insert(make_pair(this->ID, map<int, Vector4>()));
        if (StaticVector4Uniforms[this->ID].count(location) == 0)
        {
            StaticVector4Uniforms[this->ID].insert(make_pair(location, value));
            this->UseShader();
            glUniform4f(location, value.x, value.y, value.z, value.w);
        }
        if (_vector4Uniforms.count(location) == 0)
            _vector4Uniforms.insert(make_pair(location, value));
        else _vector4Uniforms[location] = value;
        return true;
    }
    bool Shader::SetMatrix4x4(const int& location, const Matrix4x4& value)
    {
        if (location < 0) return false;

        if (StaticMatrix4x4Uniforms.count(this->ID) == 0)
            StaticMatrix4x4Uniforms.insert(make_pair(this->ID, map<int, Matrix4x4>()));
        if (StaticMatrix4x4Uniforms[this->ID].count(location) == 0)
        {
            StaticMatrix4x4Uniforms[this->ID].insert(make_pair(location, value));
            this->UseShader();
            glUniformMatrix4fv(location, 1, false, value.GetPtr());
        }
        if (_matrix4x4Uniforms.count(location) == 0)
            _matrix4x4Uniforms.insert(make_pair(location, value));
        else _matrix4x4Uniforms[location] = value;
        return true;
    }

    Shader::Shader(const string& name) :ResourceObject(name),
        ID(GL_INVALID_INDEX), State(), RenderSequence(2000), _textures(),
        _intUniforms(), _floatUniforms(), _vector4Uniforms(), _matrix4x4Uniforms()
    {
        if (!realShaderState)
        {
            realShaderState = make_unique<ShaderState>();

            // ��Ȳ���
            ShaderState::SetState(GL_DEPTH_TEST, realShaderState->DepthTest);
            glDepthFunc((GLenum)realShaderState->DepthTestMode);
            glDepthMask(realShaderState->DepthMask);

            // ģ�����
            ShaderState::SetState(GL_STENCIL_TEST, realShaderState->Stencil);
            glStencilMask(realShaderState->StencilMask);
            glStencilFunc((GLenum)realShaderState->StencilTestMode, realShaderState->StencilRef, realShaderState->StencilRefMask);
            glStencilOp((GLenum)realShaderState->StencilFail, (GLenum)realShaderState->StencilZfail, (GLenum)realShaderState->StencilZpass);

            // ���
            ShaderState::SetState(GL_BLEND, realShaderState->Blend);
            glBlendFuncSeparate((GLenum)realShaderState->BlendSfactor, (GLenum)realShaderState->BlendDfactor,
                (GLenum)realShaderState->BlendSfactorAlpha, (GLenum)realShaderState->BlendDfactorAlpha);// �ֱ�ָ��RGB��alpha��������������

            // ���޳�
            ShaderState::SetState(GL_CULL_FACE, realShaderState->CullFace);
            glCullFace((GLenum)realShaderState->CullFaceMode);

            // ������
            glFrontFace(realShaderState->FrontFace);
        }
    }
    Shader::Shader(const Shader& shader) :Shader(shader.Name + " Copy")
    {
        ID = shader.ID;
        State = ShaderState(shader.State);
        RenderSequence = shader.RenderSequence;
        _textures = shader._textures;
        _intUniforms = shader._intUniforms;
        _floatUniforms = shader._floatUniforms;
        _vector4Uniforms = shader._vector4Uniforms;
        _matrix4x4Uniforms = shader._matrix4x4Uniforms;

        if (Counter.count(this->ID) == 0)
            Counter.insert(make_pair(this->ID, 0));
        Counter[this->ID]++;
    }
    Shader::Shader(const string& name, const string& vert, const string& frag, const string* geom) : Shader(name)
    {
        CompileShader(vert.c_str(), frag.c_str(), geom != nullptr ? geom->c_str() : nullptr);
    }
    Shader::Shader(const string& name, const char* vPath, const char* fPath, const char* gPath) : Shader(name)
    {
        string vertexCode, geometryCode, fragmentCode;

        vertexCode = ReadFile(vPath);
        if (gPath != nullptr)
            geometryCode = ReadFile(gPath);
        fragmentCode = ReadFile(fPath);

        CompileShader(vertexCode.c_str(), fragmentCode.c_str(), gPath != nullptr ? geometryCode.c_str() : nullptr);
    }
    Shader::Shader(const string& name, const char* path) : Shader(name)
    {
        string vertexCode, geometryCode, fragmentCode;
        bool isGeoReaded = false;

        string text = ReadFile(path);

        smatch match;
        regex_search(text, match, regex("_!GLSL_VertexProgram_Start[\\s\\S]+_!GLSL_VertexProgram_End"));
        if (match.size() > 0)
        {
            vertexCode = regex_replace(
                regex_replace(match[0].str(), regex("_!GLSL_VertexProgram_Start"), ""),
                regex("_!GLSL_VertexProgram_End"), "");
        }

        regex_search(text, match, regex("_!GLSL_GeometryProgram_Start[\\s\\S]+_!GLSL_GeometryProgram_End"));
        if (match.size() > 0)
        {
            isGeoReaded = true;
            geometryCode = regex_replace(regex_replace(match[0].str(), regex("_!GLSL_GeometryProgram_Start"), ""), regex("_!GLSL_GeometryProgram_End"), "");
        }

        regex_search(text, match, regex("_!GLSL_FragmentProgram_Start[\\s\\S]+_!GLSL_FragmentProgram_End"));
        if (match.size() > 0)
        {
            fragmentCode = regex_replace(regex_replace(match[0].str(), regex("_!GLSL_FragmentProgram_Start"), ""), regex("_!GLSL_FragmentProgram_End"), "");
        }

        CompileShader(vertexCode.c_str(), fragmentCode.c_str(), isGeoReaded ? geometryCode.c_str() : nullptr);

        regex_search(text, match, regex("_!Property_Start[\\s\\S]+_!Property_End"));
        if (match.size() > 0)
        {
            using json = nlohmann::json;
            auto propertyJson = json::parse(regex_replace(regex_replace(match[0].str(), regex("_!Property_Start"), ""), regex("_!Property_End"), ""));
            if (propertyJson.size() > 0)
            {
                if (propertyJson.count("RenderQueue") == 1)
                    this->RenderSequence = propertyJson.at("RenderQueue").get<unsigned int>();

                auto uniforms = GetAllUniforms();
                for (auto& uniform : uniforms)
                {
                    if (propertyJson.count(uniform.Item2) == 0) continue;
                    if (uniform.Item1 == UniformType::FLOAT)
                        this->SetFloat(uniform.Item2, propertyJson.at(uniform.Item2).get<float>());
                    else if (uniform.Item1 == UniformType::FLOAT_VEC4)
                    {
                        Vector4 value = Vector4::Zero;
                        for (size_t j = 0; j < 4; j++)
                            value[j] = propertyJson.at(uniform.Item2)[j].get<float>();
                        this->SetVector4(uniform.Item2, value);
                    }
                    else if (uniform.Item1 == UniformType::INT)
                        this->SetInt(uniform.Item2, propertyJson.at(uniform.Item2).get<int>());
                    else if (uniform.Item1 == UniformType::SAMPLER_2D)
                    {
                        Texture2D* tex = nullptr;
                        if (propertyJson.at(uniform.Item2)[1].get<string>() == "White")
                            tex = SceneManager::GetActiveScene().FindResourceObject<Texture2D>("Default White Texture");
                        else if (propertyJson.at(uniform.Item2)[1].get<string>() == "Gray")
                            tex = SceneManager::GetActiveScene().FindResourceObject<Texture2D>("Default Gray Texture");
                        else if (propertyJson.at(uniform.Item2)[1].get<string>() == "Black")
                            tex = SceneManager::GetActiveScene().FindResourceObject<Texture2D>("Default Black Texture");
                        else if (propertyJson.at(uniform.Item2)[1].get<string>() == "Normal")
                            tex = SceneManager::GetActiveScene().FindResourceObject<Texture2D>("Default Normal Texture");
                        if (tex != nullptr)
                            BindTexture(*tex, uniform.Item2, propertyJson.at(uniform.Item2)[0].get<int>());
                    }
                    else if (uniform.Item1 == UniformType::SAMPLER_CUBE)
                    {
                        TextureCube* tex = nullptr;
                        if (propertyJson.at(uniform.Item2)[1].get<string>() == "White")
                            tex = SceneManager::GetActiveScene().FindResourceObject<TextureCube>("Default White TextureCube");
                        if (tex != nullptr)
                            BindTexture(*tex, uniform.Item2, propertyJson.at(uniform.Item2)[0].get<int>());
                    }
                }
            }
        }
    }

    Shader::~Shader()
    {
        if (Counter.count(this->ID) == 0)
        {
            auto id = this->ID;
            glDeleteProgram(this->ID);
            cout << "LOG:: ��ɫ��:: " << "���ͷ�ID��" << id << endl << endl;
        }
        else
        {
            if (Counter[this->ID] == 0)
            {
                auto id = this->ID;
                glDeleteProgram(this->ID);
                cout << "LOG:: ��ɫ��:: " << "���ͷ�ID��" << id << endl << endl;
            }
            else
                Counter[this->ID]--;
        }
    }


    void Shader::CheckShaderCompile(GLuint shaderID, const string& type)
    {
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[512];
            glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
            cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << endl;
        }
    }

    void Shader::CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometryCode)
    {
        GLuint vert, geo, frag, id;

        // ���붥����ɫ��
        {
            vert = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vert, 1, &vertexCode, nullptr);
            glCompileShader(vert);
            CheckShaderCompile(vert, "����");
        }

        // ����Ƭ����ɫ��
        {
            frag = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(frag, 1, &fragmentCode, nullptr);
            glCompileShader(frag);
            CheckShaderCompile(frag, "Ƭ��");
        }

        if (geometryCode != nullptr)
        {
            // ���뼸����ɫ��
            {
                geo = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(geo, 1, &geometryCode, nullptr);
                glCompileShader(geo);
                CheckShaderCompile(geo, "����");
            }
        }

        // ��ɫ������
        {
            id = glCreateProgram();
            glAttachShader(id, vert);
            glAttachShader(id, frag);
            if (geometryCode != nullptr) glAttachShader(id, geo);
            glLinkProgram(id);
            // �������Ƿ�ɹ�
            GLint success;
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success)
            {
                GLchar infoLog[512];
                glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
                cout << "ERROR::PROGRAM::COMPILATION_FAILED" << endl
                    << infoLog << endl;
                throw exception("δ�ɹ�������ɫ����");
            }
        }

        // �ͷŶ���Ƭ�ζ���
        glDeleteShader(vert);
        glDeleteShader(frag);

        cout << "LOG:: ��ɫ��:: " << "�Ѱ���ɫ������ID��" << id << endl << endl;

        this->ID = id;
        // ��Uniform��
        UniformManager::UniformBlockBinding(this);

        // ��Ĭ��ShadowMap
        auto defaultWhiteTexture = SceneManager::GetActiveScene().FindResourceObject<Texture2D>("Default White Texture");
        auto defaultWhiteTextureCube = SceneManager::GetActiveScene().FindResourceObject<TextureCube>("Default White TextureCube");
        this->BindTexture(*defaultWhiteTexture, "_ShadowMap", 10);
        this->BindTexture(*defaultWhiteTextureCube, "_PointLight4ShadowMap[0]", 11);
        this->BindTexture(*defaultWhiteTextureCube, "_PointLight4ShadowMap[1]", 12);
        this->BindTexture(*defaultWhiteTextureCube, "_PointLight4ShadowMap[2]", 13);
        this->BindTexture(*defaultWhiteTextureCube, "_PointLight4ShadowMap[3]", 14);
    }
}
