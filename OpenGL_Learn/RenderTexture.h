#pragma once

#include <glad/glad.h>

#include "Texture.h"

namespace OpenGL_Learn
{
    enum class AttachmentType
    {
        COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
        DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT
    };

    class RenderTexture : public ResourceObject
    {
    public:
        // ʹ��Ĭ�ϴ��ڻ���
        static inline void UnBindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
        //@brief ������Ⱦ��ͼ
        //@param name ����
        //@param width ���
        //@param height �߶�
        //@param format ��ʽ��GL_RGB��GL_RGBA��GL_DEPTH_COMPONENT
        //@param wrapMode �ظ�ģʽ GL_CLAMP_TO_EDGE��GL_REPEAT
        //@param scaleFilterMode ����ģʽ GL_LINEAR��GL_NEAREST
        //@param attachment ��������ʽ GL_COLOR_ATTACHMENT0��GL_DEPTH_ATTACHMENT
        //@param isDepthAndStencilOn �Ƿ�����Ⱥ�ģ�� GL_DEPTH24_STENCIL8
        //@param rboFormat ������󸽼���ʽ
        //static unique_ptr<RenderTexture> CreateRenderTexture(const string& name,
        //    GLsizei width, GLsizei height,
        //    GLenum internalformat, FormatType format,
        //    TextureType type = TextureType::UNSIGNED_BYTE,
        //    WrapType wrapMode = WrapType::CLAMP_TO_EDGE,
        //    ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR,
        //    AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0,
        //    bool isDepthAndStencilOn = true,
        //    GLenum rboFormat = GL_DEPTH24_STENCIL8);
        static unique_ptr<RenderTexture> CreateRenderTexture(const string& name, GLsizei width, GLsizei height, int samples = 0)
        {
            auto rt = unique_ptr<RenderTexture>(new RenderTexture(name, width, height));
            rt->_samples = samples;
            // ����֡����
            glGenFramebuffers(1, &rt->_framebufferID);

            return rt;
        }

        Texture2D* AttachmentTexture2D(GLenum internalformat, FormatType format,
            TextureType type = TextureType::UNSIGNED_BYTE,
            WrapType wrapMode = WrapType::CLAMP_TO_EDGE,
            ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR,
            AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0);
        Texture2DMultisample* AttachmentMultisampleTexture(GLenum internalformat,
            AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0);
        TextureCube* AttachmentTextureCube(GLenum internalformat, FormatType format,
            TextureType type = TextureType::UNSIGNED_BYTE,
            WrapType wrapMode = WrapType::CLAMP_TO_EDGE,
            ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR,
            AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0);

        GLuint AttachmentRenderBuffer(GLenum internalformat = GL_DEPTH24_STENCIL8, AttachmentType attachmentType = AttachmentType::DEPTH_STENCIL_ATTACHMENT);
        bool CheckFramebufferSuccess();

        //inline virtual GLenum GetTextureType() const override { return GL_TEXTURE_2D; }
        // ����󶨵�֡����
        inline void BindFramebuffer() const { glBindFramebuffer(GL_FRAMEBUFFER, this->_framebufferID); }
        // λ�鴫�� ������ָ��֡����
        // @renderTexture ֡���壨Ĭ�Ͽ��������ڣ�
        // @mask ָ��Ҫ��ȡ�Ļ����� // GL_COLOR_BUFFER_BIT ��ɫ������ // GL_DEPTH_BUFFER_BIT ��Ȼ����� // GL_STENCIL_BUFFER_BIT ģ�建����
        // @filter ��������ʱ�Ĳ�ֵ���� GL_NEAREST  GL_LINEAR
        void Blit(RenderTexture& renderTexture, GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, ScaleFilterType filter = ScaleFilterType::NEAREST);
        inline Texture* GetTexture(UINT index)
        {
            if (index < _textureIDs.Count())
                return _textureIDs[index].get();
            else return nullptr;
        }
        inline GLuint GetFrameBufferID() const { return _framebufferID; }
        inline List<unique_ptr<Texture>>& GetTextures() { return _textureIDs; }
        inline GLuint GetRenderBufferObjectID() const { return _renderBufferObjectID; }

        ~RenderTexture() override;

    protected:
        GLuint _framebufferID;
        List<unique_ptr<Texture>> _textureIDs;
        GLuint _renderBufferObjectID;

        UINT _width;
        UINT _height;
        int _samples;

        RenderTexture(const string& name, UINT width, UINT height) :ResourceObject(name),
            _framebufferID(GL_INVALID_INDEX), _textureIDs(), _renderBufferObjectID(GL_INVALID_INDEX),
            _width(width), _height(height), _samples(0) {}
        //RenderTexture(GLuint id, const string& name, UINT width, UINT height) :Texture(id, name, width, height), _framebufferID(GL_INVALID_INDEX), _renderBufferObjectID(GL_INVALID_INDEX) {}
    };

    //class MultisampleRenderTexture final : public RenderTexture
    //{
    //public:
    //    //@brief �����������Ⱦ��ͼ
    //    //@param name ����
    //    //@param samples ������
    //    //@param width ���
    //    //@param height �߶�
    //    //@param format ��ʽ��GL_RGB��GL_RGBA
    //    //@param attachment ��������ʽ GL_COLOR_ATTACHMENT0��GL_DEPTH_ATTACHMENT
    //    //@param isDepthAndStencilOn �Ƿ�����Ⱥ�ģ�� GL_DEPTH24_STENCIL8
    //    //@param rboFormat ������󸽼���ʽ
    //    static unique_ptr<MultisampleRenderTexture> CreateRenderTexture(const string& name,
    //        int samples, GLsizei width, GLsizei height, GLenum internalformat,
    //        AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0,
    //        bool isDepthAndStencilOn = true,
    //        GLenum rboFormat = GL_DEPTH24_STENCIL8);

    //    //inline GLenum GetTextureType() const override { return GL_TEXTURE_2D_MULTISAMPLE; }

    //    // ��ȡ������
    //    inline int GetSamples() const { return _samples; }

    //private:
    //    int _samples;

    //    MultisampleRenderTexture(const string& name, UINT width, UINT height) : RenderTexture(name, width, height), _samples(4) {}
    //    //MultisampleRenderTexture(GLuint id, const string& name, UINT width, UINT height) : RenderTexture(id, name, width, height), _samples(4) {}
    //};

    //class CubeRenderTexture final : public RenderTexture
    //{
    //public:
    //    static unique_ptr<CubeRenderTexture> CreateRenderTexture(const string& name,
    //        GLsizei width, GLsizei height,
    //        GLenum internalformat, FormatType format,
    //        TextureType type = TextureType::UNSIGNED_BYTE,
    //        WrapType wrapMode = WrapType::CLAMP_TO_EDGE,
    //        ScaleFilterType scaleFilterMode = ScaleFilterType::LINEAR,
    //        AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0);

    //    //inline GLenum GetTextureType() const override { return GL_TEXTURE_CUBE_MAP; }

    //private:

    //    CubeRenderTexture(const string& name, UINT width, UINT height) :RenderTexture(name, width, height) {}
    //    //CubeRenderTexture(GLuint id, const string& name, UINT width, UINT height) :RenderTexture(id, name, width, height) {}
    //};
}
