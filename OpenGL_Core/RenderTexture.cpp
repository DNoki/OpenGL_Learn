#include "RenderTexture.h"

#include <iostream>

#include "GameSystem.h"

namespace OpenGL_Core
{
    unique_ptr<RenderTexture> RenderTexture::CreateRenderTexture(const string& name, GLsizei width, GLsizei height, int samples)
    {
        auto rt = unique_ptr<RenderTexture>(new RenderTexture(name, width, height));
        rt->_samples = samples;
        // 生成帧缓冲
        glGenFramebuffers(1, &rt->_framebufferID);

        return rt;
    }

    Texture2D* RenderTexture::AttachmentTexture2D(GLenum internalformat, FormatType format, TextureType type, WrapType wrapMode, ScaleFilterType scaleFilterMode, AttachmentType attachment)
    {
        this->BindFramebuffer();
        auto tex = Texture2D::CreateTexture2D(this->Name, this->_width, this->_height,
            internalformat, format, type, NULL, wrapMode, scaleFilterMode);

        // 纹理附件
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D, tex->ID, 0);
        if (attachment == AttachmentType::DEPTH_ATTACHMENT) // 纹理附件是深度时，显式告诉OpenGL不适用任何颜色数据进行渲染
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        UnBindFramebuffer();

        auto result = tex.get();
        this->_textureIDs.push_back(move(tex));
        return result;
    }

    Texture2DMultisample* RenderTexture::AttachmentMultisampleTexture(GLenum internalformat, AttachmentType attachment)
    {
        this->BindFramebuffer();
        auto tex = Texture2DMultisample::CreateTexture2DMultisample(this->Name, this->_samples,
            this->_width, this->_height, internalformat);

        // 纹理附件
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D_MULTISAMPLE, tex->ID, 0);
        if (attachment == AttachmentType::DEPTH_ATTACHMENT) // 纹理附件是深度时，显式告诉OpenGL不适用任何颜色数据进行渲染
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        UnBindFramebuffer();

        auto result = tex.get();
        this->_textureIDs.push_back(move(tex));
        return result;
    }

    TextureCube* RenderTexture::AttachmentTextureCube(GLenum internalformat, FormatType format, TextureType type, WrapType wrapMode, ScaleFilterType scaleFilterMode, AttachmentType attachment)
    {
        this->BindFramebuffer();
        auto tex = TextureCube::CreateTextureCube(this->Name, this->_width, this->_height,
            internalformat, format, type, NULL, wrapMode, scaleFilterMode);

        // 纹理附件
        glFramebufferTexture(GL_FRAMEBUFFER, static_cast<int>(attachment), tex->ID, 0);
        if (attachment == AttachmentType::DEPTH_ATTACHMENT) // 纹理附件是深度时，显式告诉OpenGL不适用任何颜色数据进行渲染
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        UnBindFramebuffer();

        auto result = tex.get();
        this->_textureIDs.push_back(move(tex));
        return result;
    }

    GLuint RenderTexture::AttachmentRenderBuffer(GLenum internalformat, AttachmentType attachmentType)
    {
        this->BindFramebuffer();
        //this->_renderBufferObjectID = GL_INVALID_INDEX;
        glGenRenderbuffers(1, &this->_renderBufferObjectID);
        glBindRenderbuffer(GL_RENDERBUFFER, this->_renderBufferObjectID);
        if (this->_samples == 0)
            glRenderbufferStorage(GL_RENDERBUFFER, internalformat, this->_width, this->_height);
        else
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->_samples, internalformat, this->_width, this->_height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<int>(attachmentType), GL_RENDERBUFFER, this->_renderBufferObjectID);
        UnBindFramebuffer();

        return this->_renderBufferObjectID;
    }

    bool RenderTexture::CheckFramebufferSuccess()
    {
        this->BindFramebuffer();
        // 检查帧缓冲是否完整
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            cout << "ERROR::FRAMEBUFFER:: RenderTexture::Framebuffer is not complete!" << endl;
            throw;
        }
        return true;
        UnBindFramebuffer();
    }

    void RenderTexture::Blit(RenderTexture& renderTexture, GLbitfield mask, ScaleFilterType filter)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebufferID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderTexture._framebufferID);
        glBlitFramebuffer(0, 0, _width, _height, 0, 0, renderTexture._width, renderTexture._height, mask, static_cast<int>(filter));
    }

    RenderTexture::~RenderTexture()
    {
        GLuint framebufferID = this->_framebufferID, renderBufferObjectID = this->_renderBufferObjectID;

        glDeleteFramebuffers(1, &(this->_framebufferID));
        cout << "LOG:: 渲染贴图:: Name：" << Name << " FramebufferID：" << framebufferID << " 帧缓冲已释放。";

        if (this->_renderBufferObjectID != GL_INVALID_INDEX)
        {
            glDeleteRenderbuffers(1, &(this->_renderBufferObjectID));
            cout << " RenderBufferID：" << renderBufferObjectID << " 渲染缓冲附件已释放。";
        }
        cout << endl;
    }


    //unique_ptr<RenderTexture> RenderTexture::CreateRenderTexture(const string& name,
    //    GLsizei width, GLsizei height,
    //    GLenum internalformat, FormatType format, TextureType type,
    //    WrapType wrapMode, ScaleFilterType scaleFilterMode,
    //    AttachmentType attachment, bool isDepthAndStencilOn, GLenum rboFormat)
    //{
    //    GLuint id, framebufferID, renderBufferObjectID(GL_INVALID_INDEX);

    //    // 生成纹理
    //    glGenTextures(1, &id);
    //    glBindTexture(GL_TEXTURE_2D, id);
    //    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(type), NULL);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(scaleFilterMode));
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(scaleFilterMode));
    //    glBindTexture(GL_TEXTURE_2D, 0);

    //    // 生成帧缓冲
    //    glGenFramebuffers(1, &framebufferID);
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    //    // 纹理附件
    //    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D, id, 0);
    //    if (attachment == AttachmentType::DEPTH_ATTACHMENT) // 纹理附件是深度时，显式告诉OpenGL不适用任何颜色数据进行渲染
    //    {
    //        glDrawBuffer(GL_NONE);
    //        glReadBuffer(GL_NONE);
    //    }

    //    // 渲染缓冲对象附件
    //    if (isDepthAndStencilOn)
    //    {
    //        glGenRenderbuffers(1, &renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectID);
    //        glRenderbufferStorage(GL_RENDERBUFFER, rboFormat, width, height);

    //        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //    }

    //    // 检查帧缓冲是否完整
    //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    {
    //        cout << "ERROR::FRAMEBUFFER:: RenderTexture::Framebuffer is not complete!" << endl;
    //        throw;
    //    }
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //    auto rt = unique_ptr<RenderTexture>(new RenderTexture(id, name, width, height));
    //    rt->_framebufferID = framebufferID;
    //    rt->_renderBufferObjectID = renderBufferObjectID;

    //    cout << "LOG:: " << "渲染贴图:: " << "ID=" << id << " Name：" << name
    //        << " 已生成帧缓冲：ID=" << framebufferID;
    //    if (isDepthAndStencilOn)cout << " 已生成渲染缓冲对象：ID=" << renderBufferObjectID;
    //    cout << endl << endl;

    //    return rt;
    //}

    //unique_ptr<MultisampleRenderTexture> MultisampleRenderTexture::CreateRenderTexture(const string& name,
    //    int samples, GLsizei width, GLsizei height, GLenum internalformat,
    //    AttachmentType attachment, bool isDepthAndStencilOn, GLenum rboFormat)
    //{
    //    GLuint id, framebufferID, renderBufferObjectID(GL_INVALID_INDEX);

    //    // 生成纹理
    //    glGenTextures(1, &id);
    //    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    //    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_TRUE);
    //    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);


    //    // 生成帧缓冲
    //    glGenFramebuffers(1, &framebufferID);
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    //    // 纹理附件
    //    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D_MULTISAMPLE, id, 0);
    //    if (attachment == AttachmentType::DEPTH_ATTACHMENT) // 纹理附件是深度时，显式告诉OpenGL不适用任何颜色数据进行渲染
    //    {
    //        glDrawBuffer(GL_NONE);
    //        glReadBuffer(GL_NONE);
    //    }

    //    // 渲染缓冲对象附件
    //    if (isDepthAndStencilOn)
    //    {
    //        glGenRenderbuffers(1, &renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectID);
    //        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, rboFormat, width, height);
    //        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //    }

    //    // 检查帧缓冲是否完整
    //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    {
    //        cout << "ERROR::FRAMEBUFFER:: RenderTexture::Framebuffer is not complete!" << endl;
    //        throw;
    //    }
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //    auto rt = unique_ptr<MultisampleRenderTexture>(new MultisampleRenderTexture(id, name, width, height));
    //    rt->_samples = samples;
    //    rt->_framebufferID = framebufferID;
    //    rt->_renderBufferObjectID = renderBufferObjectID;

    //    cout << "LOG:: " << "多采样渲染贴图:: " << "ID=" << id << " Name：" << name
    //        << " 已生成多采样帧缓冲：ID=" << framebufferID;
    //    if (isDepthAndStencilOn)cout << " 已生成多采样渲染缓冲对象：ID=" << renderBufferObjectID;
    //    cout << endl << endl;

    //    return rt;
    //}

    //unique_ptr<CubeRenderTexture> CubeRenderTexture::CreateRenderTexture(const string& name,
    //    GLsizei width, GLsizei height,
    //    GLenum internalformat, FormatType format, TextureType type,
    //    WrapType wrapMode, ScaleFilterType scaleFilterMode, AttachmentType attachment)
    //{
    //    GLuint id, framebufferID, renderBufferObjectID(GL_INVALID_INDEX);

    //    // 生成纹理
    //    glGenTextures(1, &id);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    //    for (size_t i = 0; i < 6; i++)
    //        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    //            0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(type), NULL);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<int>(scaleFilterMode));
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<int>(scaleFilterMode));
    //    glBindTexture(GL_TEXTURE_2D, 0);

    //    // 生成帧缓冲
    //    glGenFramebuffers(1, &framebufferID);
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    //    // 纹理附件
    //    glFramebufferTexture(GL_FRAMEBUFFER, static_cast<int>(attachment), id, 0);
    //    //glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D, id, 0);
    //    if (attachment == AttachmentType::DEPTH_ATTACHMENT) // 纹理附件是深度时，显式告诉OpenGL不适用任何颜色数据进行渲染
    //    {
    //        glDrawBuffer(GL_NONE);
    //        glReadBuffer(GL_NONE);
    //    }

    //    // 检查帧缓冲是否完整
    //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    {
    //        cout << "ERROR::FRAMEBUFFER:: RenderTexture::Framebuffer is not complete!" << endl;
    //        throw;
    //    }
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //    auto rt = unique_ptr<CubeRenderTexture>(new CubeRenderTexture(id, name, width, height));
    //    rt->_framebufferID = framebufferID;
    //    rt->_renderBufferObjectID = renderBufferObjectID;

    //    cout << "LOG:: " << "Cube渲染贴图:: " << "ID=" << id << " Name：" << name
    //        << " 已生成帧缓冲：ID=" << framebufferID << endl << endl;
    //    return rt;
    //}

}