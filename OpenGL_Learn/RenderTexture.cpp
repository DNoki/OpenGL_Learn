#include "RenderTexture.h"

#include <iostream>

#include "GameSystem.h"

namespace OpenGL_Learn
{
    Texture2D* RenderTexture::AttachmentTexture2D(GLenum internalformat, FormatType format, TextureType type, WrapType wrapMode, ScaleFilterType scaleFilterMode, AttachmentType attachment)
    {
        this->BindFramebuffer();
        auto tex = Texture2D::CreateTexture2D(this->Name, this->_width, this->_height,
            internalformat, format, type, NULL, wrapMode, scaleFilterMode);

        // ������
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D, tex->ID, 0);
        if (attachment == AttachmentType::DEPTH_ATTACHMENT) // �����������ʱ����ʽ����OpenGL�������κ���ɫ���ݽ�����Ⱦ
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

        // ������
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D_MULTISAMPLE, tex->ID, 0);
        if (attachment == AttachmentType::DEPTH_ATTACHMENT) // �����������ʱ����ʽ����OpenGL�������κ���ɫ���ݽ�����Ⱦ
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

        // ������
        glFramebufferTexture(GL_FRAMEBUFFER, static_cast<int>(attachment), tex->ID, 0);
        if (attachment == AttachmentType::DEPTH_ATTACHMENT) // �����������ʱ����ʽ����OpenGL�������κ���ɫ���ݽ�����Ⱦ
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
        // ���֡�����Ƿ�����
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
        cout << "LOG:: ��Ⱦ��ͼ:: Name��" << Name << " FramebufferID��" << framebufferID << " ֡�������ͷš�";

        if (this->_renderBufferObjectID != GL_INVALID_INDEX)
        {
            glDeleteRenderbuffers(1, &(this->_renderBufferObjectID));
            cout << " RenderBufferID��" << renderBufferObjectID << " ��Ⱦ���帽�����ͷš�";
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

    //    // ��������
    //    glGenTextures(1, &id);
    //    glBindTexture(GL_TEXTURE_2D, id);
    //    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, static_cast<int>(format), static_cast<int>(type), NULL);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrapMode));
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(scaleFilterMode));
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(scaleFilterMode));
    //    glBindTexture(GL_TEXTURE_2D, 0);

    //    // ����֡����
    //    glGenFramebuffers(1, &framebufferID);
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    //    // ������
    //    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D, id, 0);
    //    if (attachment == AttachmentType::DEPTH_ATTACHMENT) // �����������ʱ����ʽ����OpenGL�������κ���ɫ���ݽ�����Ⱦ
    //    {
    //        glDrawBuffer(GL_NONE);
    //        glReadBuffer(GL_NONE);
    //    }

    //    // ��Ⱦ������󸽼�
    //    if (isDepthAndStencilOn)
    //    {
    //        glGenRenderbuffers(1, &renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectID);
    //        glRenderbufferStorage(GL_RENDERBUFFER, rboFormat, width, height);

    //        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //    }

    //    // ���֡�����Ƿ�����
    //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    {
    //        cout << "ERROR::FRAMEBUFFER:: RenderTexture::Framebuffer is not complete!" << endl;
    //        throw;
    //    }
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //    auto rt = unique_ptr<RenderTexture>(new RenderTexture(id, name, width, height));
    //    rt->_framebufferID = framebufferID;
    //    rt->_renderBufferObjectID = renderBufferObjectID;

    //    cout << "LOG:: " << "��Ⱦ��ͼ:: " << "ID=" << id << " Name��" << name
    //        << " ������֡���壺ID=" << framebufferID;
    //    if (isDepthAndStencilOn)cout << " ��������Ⱦ�������ID=" << renderBufferObjectID;
    //    cout << endl << endl;

    //    return rt;
    //}

    //unique_ptr<MultisampleRenderTexture> MultisampleRenderTexture::CreateRenderTexture(const string& name,
    //    int samples, GLsizei width, GLsizei height, GLenum internalformat,
    //    AttachmentType attachment, bool isDepthAndStencilOn, GLenum rboFormat)
    //{
    //    GLuint id, framebufferID, renderBufferObjectID(GL_INVALID_INDEX);

    //    // ��������
    //    glGenTextures(1, &id);
    //    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    //    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalformat, width, height, GL_TRUE);
    //    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);


    //    // ����֡����
    //    glGenFramebuffers(1, &framebufferID);
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    //    // ������
    //    glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D_MULTISAMPLE, id, 0);
    //    if (attachment == AttachmentType::DEPTH_ATTACHMENT) // �����������ʱ����ʽ����OpenGL�������κ���ɫ���ݽ�����Ⱦ
    //    {
    //        glDrawBuffer(GL_NONE);
    //        glReadBuffer(GL_NONE);
    //    }

    //    // ��Ⱦ������󸽼�
    //    if (isDepthAndStencilOn)
    //    {
    //        glGenRenderbuffers(1, &renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObjectID);
    //        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, rboFormat, width, height);
    //        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObjectID);
    //        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //    }

    //    // ���֡�����Ƿ�����
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

    //    cout << "LOG:: " << "�������Ⱦ��ͼ:: " << "ID=" << id << " Name��" << name
    //        << " �����ɶ����֡���壺ID=" << framebufferID;
    //    if (isDepthAndStencilOn)cout << " �����ɶ������Ⱦ�������ID=" << renderBufferObjectID;
    //    cout << endl << endl;

    //    return rt;
    //}

    //unique_ptr<CubeRenderTexture> CubeRenderTexture::CreateRenderTexture(const string& name,
    //    GLsizei width, GLsizei height,
    //    GLenum internalformat, FormatType format, TextureType type,
    //    WrapType wrapMode, ScaleFilterType scaleFilterMode, AttachmentType attachment)
    //{
    //    GLuint id, framebufferID, renderBufferObjectID(GL_INVALID_INDEX);

    //    // ��������
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

    //    // ����֡����
    //    glGenFramebuffers(1, &framebufferID);
    //    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    //    // ������
    //    glFramebufferTexture(GL_FRAMEBUFFER, static_cast<int>(attachment), id, 0);
    //    //glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<int>(attachment), GL_TEXTURE_2D, id, 0);
    //    if (attachment == AttachmentType::DEPTH_ATTACHMENT) // �����������ʱ����ʽ����OpenGL�������κ���ɫ���ݽ�����Ⱦ
    //    {
    //        glDrawBuffer(GL_NONE);
    //        glReadBuffer(GL_NONE);
    //    }

    //    // ���֡�����Ƿ�����
    //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    {
    //        cout << "ERROR::FRAMEBUFFER:: RenderTexture::Framebuffer is not complete!" << endl;
    //        throw;
    //    }
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //    auto rt = unique_ptr<CubeRenderTexture>(new CubeRenderTexture(id, name, width, height));
    //    rt->_framebufferID = framebufferID;
    //    rt->_renderBufferObjectID = renderBufferObjectID;

    //    cout << "LOG:: " << "Cube��Ⱦ��ͼ:: " << "ID=" << id << " Name��" << name
    //        << " ������֡���壺ID=" << framebufferID << endl << endl;
    //    return rt;
    //}

}