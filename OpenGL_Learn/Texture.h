#pragma once

#include <string>
#include <memory>
#include <glad/glad.h>

#include "ResourceObject.h"

namespace OpenGL_Learn
{
    using namespace std;

    enum class FormatType
    {
        RED = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        BGR = GL_BGR,
        RGBA = GL_RGBA,
        BGRA = GL_BGRA,
        RED_INTEGER = GL_RED_INTEGER,
        RG_INTEGER = GL_RG_INTEGER,
        RGB_INTEGER = GL_RGB_INTEGER,
        BGR_INTEGER = GL_BGR_INTEGER,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        BGRA_INTEGER = GL_BGRA_INTEGER,
        STENCIL_INDEX = GL_STENCIL_INDEX,
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL,
    };
    enum class TextureType
    {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        BYTE = GL_BYTE,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        SHORT = GL_SHORT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        INT = GL_INT,
        HALF_FLOAT = GL_HALF_FLOAT,
        FLOAT = GL_FLOAT,
        UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
        UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
        UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
        UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
        UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
        UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
        UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
        UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
        UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
        UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
        UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
        UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
    };
    enum class WrapType
    {
        REPEAT = GL_REPEAT,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
    };
    enum class ScaleFilterType
    {
        NEAREST = GL_NEAREST, // �ڽ�����
        LINEAR = GL_LINEAR, // ���Թ���(�����Χ��ɫ)
    };


    class Texture : public ResourceObject
    {
    public:
        GLuint ID;

        // ��ȡ��ͼ����
        virtual GLenum GetTextureType() const = 0;
        // ��ȡ���
        inline int GetWidth() const { return _width; }
        // ��ȡ�߶�
        inline int GetHeight() const { return _height; }
        inline void BindTexture() const { glBindTexture(GetTextureType(), ID); }

        virtual ~Texture();

    protected:
        UINT _width;
        UINT _height;

        Texture(GLuint id, const string& name, UINT width, UINT height) :ResourceObject(name), ID(id), _width(width), _height(height) {}
    };



    class Texture2D final : public Texture
    {
    public:
        //@brief ���ļ���ȡ2D��ͼ
        //@param path		·��
        //@param isSRGB		�Ƿ�ʶ��ΪsRGB��ͼ
        //@param wrapMode   �ظ�ģʽ
        //@param scaleFilterMode	����ģʽ
        //@param flipY	�Ƿ�תY
        //
        //@return ���ؼ��ص���ͼID
        //static Texture2D& LoadTexture2D(const string& name, const string& path, GLenum wrapMode = GL_REPEAT, GLenum scaleFilterMode = GL_LINEAR);
        static unique_ptr<Texture2D> CreateTexture2D(const string& name,
            const string& path, bool isSRGB,
            WrapType wrapMode = WrapType::REPEAT,
            ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR,
            bool flipY = true);
        static unique_ptr<Texture2D> CreateTexture2D(const string& name,
            GLsizei width, GLsizei height,
            GLenum internalformat, FormatType format, TextureType type, const void* pixels,
            WrapType wrapMode = WrapType::REPEAT,
            ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR);

        inline GLenum GetTextureType() const { return GL_TEXTURE_2D; }

    private:

        Texture2D(GLuint id, const string& name, UINT width, UINT height) :Texture(id, name, width, height) {}
    };

    class Texture2DMultisample final : public Texture
    {
    public:
        static unique_ptr<Texture2DMultisample> CreateTexture2DMultisample(const string& name,
            int samples, GLsizei width, GLsizei height, GLenum internalformat);

        inline GLenum GetTextureType() const { return GL_TEXTURE_2D; }

    private:
        Texture2DMultisample(GLuint id, const string& name, UINT width, UINT height) :Texture(id, name, width, height) {}
    };


    class TextureCube final : public Texture
    {
    public:
        static unique_ptr<TextureCube> CreateTextureCube(const string& name,
            List<string>& paths, bool isSRGB,
            WrapType wrapMode = WrapType::CLAMP_TO_EDGE,
            ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR,
            bool flipY = true);
        static unique_ptr<TextureCube> CreateTextureCube(const string& name,
            GLsizei width, GLsizei height,
            GLenum internalformat, FormatType format, TextureType type, const void* pixels,
            WrapType wrapMode = WrapType::CLAMP_TO_EDGE,
            ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR);

        inline GLenum GetTextureType() const { return GL_TEXTURE_CUBE_MAP; }


    private:
        //static void GenTexture2D(UINT index, GLsizei width, GLsizei height, GLenum internalformat, FormatType format, TextureType type, const void* pixels);

        TextureCube(GLuint id, const string& name, UINT width, UINT height) :Texture(id, name, width, height) {}
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
    //	glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else cout << "Failed to load texture" << endl;
    //stbi_image_free(data);// �ͷ�ͼ����ڴ�
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //enum TextureType
    //{
    //	Diffuse,
    //	Specular,
    //	Normal,
    //};
}