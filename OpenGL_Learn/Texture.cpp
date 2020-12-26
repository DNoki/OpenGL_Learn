#include "Texture.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION //ͨ������STB_IMAGE_IMPLEMENTATION��Ԥ���������޸�ͷ�ļ�������ֻ������صĺ�������Դ�룬�����ǽ����ͷ�ļ���Ϊһ�� .cpp �ļ��ˡ�
#include <stb_image.h>


namespace OpenGL_Learn
{
    Texture::~Texture()
    {
        auto id = this->ID;
        glDeleteTextures(1, &this->ID);
        cout << "LOG:: ����:: " << "ID=" << id << " ������Դ���ͷš� Name��" << this->Name << endl << endl;
    }

    unique_ptr<Texture2D> Texture2D::CreateTexture2D(const string& name, const string& path, bool isSRGB, WrapType wrapMode, ScaleFilterType scaleFilterMode, bool flipY)
    {
        unique_ptr<Texture2D> tex = nullptr;

        // ���ز���������
        int texWidth, texHeight, nrChannels;
        stbi_set_flip_vertically_on_load(flipY);// ��ת����
        unsigned char* data = stbi_load(path.c_str(), &texWidth, &texHeight, &nrChannels, 0);
        if (data)
        {
            GLenum internalformat;
            FormatType format;
            if (nrChannels == 4)
            {
                if (isSRGB)internalformat = GL_SRGB_ALPHA;
                else internalformat = GL_RGBA;
                format = FormatType::RGBA;
            }
            else if (nrChannels == 3)
            {
                if (isSRGB)internalformat = GL_SRGB;
                else internalformat = GL_RGB;
                format = FormatType::RGB;
            }
            else if (nrChannels == 1)
            {
                internalformat = GL_RED;
                format = FormatType::RED;
            }

            tex = Texture2D::CreateTexture2D(name, texWidth, texHeight, internalformat, format, TextureType::UNSIGNED_BYTE, data, wrapMode, scaleFilterMode);
            stbi_image_free(data);// �ͷ�ͼ����ڴ�
            cout << "LOG:: " << "����:: ID=" << tex->ID << " �Ѽ���2D��ͼ��" << path << endl << endl;
        }
        else
        {
            cout << "ERROR:: " << "Failed to load texture" << endl;
            stbi_image_free(data);
            throw exception("Failed to load texture");
        }
        return tex;
    }

    unique_ptr<Texture2D> Texture2D::CreateTexture2D(const string& name, GLsizei width, GLsizei height, GLenum internalformat, FormatType format, TextureType type, const void* pixels, WrapType wrapMode, ScaleFilterType scaleFilterMode)
    {
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(type), pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(scaleFilterMode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(scaleFilterMode));
        glBindTexture(GL_TEXTURE_2D, 0);

        auto tex = unique_ptr<Texture2D>(new Texture2D(id, name, width, height));

        return tex;
    }

    unique_ptr<TextureCube> TextureCube::CreateTextureCube(const string& name, List<string>& paths, bool isSRGB, WrapType wrapMode, ScaleFilterType scaleFilterMode, bool flipY)
    {
        if (paths.size() != 6) return nullptr;

        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        int width, height, nrChannels;


        for (unsigned int i = 0; i < 6; i++)
        {
            stbi_set_flip_vertically_on_load(flipY);// ��ת����
            auto data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);

            GLenum internalformat;
            FormatType format;
            if (nrChannels == 4)
            {
                if (isSRGB)internalformat = GL_SRGB_ALPHA;
                else internalformat = GL_RGBA;
                format = FormatType::RGBA;
            }
            else if (nrChannels == 3)
            {
                if (isSRGB)internalformat = GL_SRGB;
                else internalformat = GL_RGB;
                format = FormatType::RGB;
            }
            else //if (nrChannels == 1)
            {
                internalformat = GL_RED;
                format = FormatType::RED;
            }

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(TextureType::UNSIGNED_BYTE), data);

                //GenTexture2D(i, width, height, internalformat, format, TextureType::UNSIGNED_BYTE, data);
                cout << "LOG:: " << "Cube����:: ID=" << id << " �Ѽ���2D��ͼ��" << paths[i] << endl;
            }
            else
            {
                cout << "ERROR:: " << "Failed to load Cubemap texture" << endl;
                stbi_image_free(data);
                throw exception("Failed to load Cubemap texture");
            }
        }
        cout << endl;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<int>(scaleFilterMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<int>(scaleFilterMode));
        glBindTexture(GL_TEXTURE_2D, 0);

        auto tex = unique_ptr<TextureCube>(new TextureCube(id, name, width, height));

        return tex;
    }

    unique_ptr<TextureCube> TextureCube::CreateTextureCube(const string& name,
        GLsizei width, GLsizei height, 
        GLenum internalformat, FormatType format,
        TextureType type, const void* pixels, 
        WrapType wrapMode, ScaleFilterType scaleFilterMode)
    {
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        for (size_t i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(type), pixels);
            //GenTexture2D(i, width, height, internalformat, format, type, pixels);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<int>(wrapMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<int>(scaleFilterMode));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<int>(scaleFilterMode));
        glBindTexture(GL_TEXTURE_2D, 0);

        auto tex = unique_ptr<TextureCube>(new TextureCube(id, name, width, height));

        return tex;
    }

    //void TextureCube::GenTexture2D(UINT index, GLsizei width, GLsizei height, GLenum internalformat, FormatType format, TextureType type, const void* pixels)
    //{
    //    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
    //        0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(type), pixels);
    //}


    unique_ptr<Texture2DMultisample> Texture2DMultisample::CreateTexture2DMultisample(const string& name,int samples, GLsizei width, GLsizei height, GLenum internalformat)
    {
        GLuint id;

        // ��������
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        auto tex = unique_ptr<Texture2DMultisample>(new Texture2DMultisample(id, name, width, height));
        return tex;
    }

}