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
        // 使用默认窗口缓冲
        static inline void UnBindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
        //@brief 创建渲染贴图
        //@param name 名称
        //@param width 宽度
        //@param height 高度
        //@param format 格式：GL_RGB、GL_RGBA、GL_DEPTH_COMPONENT
        //@param wrapMode 重复模式 GL_CLAMP_TO_EDGE、GL_REPEAT
        //@param scaleFilterMode 缩放模式 GL_LINEAR、GL_NEAREST
        //@param attachment 纹理附件格式 GL_COLOR_ATTACHMENT0、GL_DEPTH_ATTACHMENT
        //@param isDepthAndStencilOn 是否开启深度和模板 GL_DEPTH24_STENCIL8
        //@param rboFormat 缓冲对象附件格式
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
            // 生成帧缓冲
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
        // 激活绑定的帧缓冲
        inline void BindFramebuffer() const { glBindFramebuffer(GL_FRAMEBUFFER, this->_framebufferID); }
        // 位块传送 拷贝到指定帧缓冲
        // @renderTexture 帧缓冲（默认拷贝到窗口）
        // @mask 指定要读取的缓冲区 // GL_COLOR_BUFFER_BIT 颜色缓冲区 // GL_DEPTH_BUFFER_BIT 深度缓冲区 // GL_STENCIL_BUFFER_BIT 模板缓冲区
        // @filter 伸缩变形时的插值方法 GL_NEAREST  GL_LINEAR
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
    //    //@brief 创建多采样渲染贴图
    //    //@param name 名称
    //    //@param samples 采样数
    //    //@param width 宽度
    //    //@param height 高度
    //    //@param format 格式：GL_RGB、GL_RGBA
    //    //@param attachment 纹理附件格式 GL_COLOR_ATTACHMENT0、GL_DEPTH_ATTACHMENT
    //    //@param isDepthAndStencilOn 是否开启深度和模板 GL_DEPTH24_STENCIL8
    //    //@param rboFormat 缓冲对象附件格式
    //    static unique_ptr<MultisampleRenderTexture> CreateRenderTexture(const string& name,
    //        int samples, GLsizei width, GLsizei height, GLenum internalformat,
    //        AttachmentType attachment = AttachmentType::COLOR_ATTACHMENT0,
    //        bool isDepthAndStencilOn = true,
    //        GLenum rboFormat = GL_DEPTH24_STENCIL8);

    //    //inline GLenum GetTextureType() const override { return GL_TEXTURE_2D_MULTISAMPLE; }

    //    // 获取采样数
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
