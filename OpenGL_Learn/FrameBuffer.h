#pragma once

#include <memory>
#include <glad/glad.h>
#include "Texture.h"

class FrameBuffer
{
public:
    // 使用默认窗口缓冲
	static void UseDefaultFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    // 帧缓冲ID
	GLuint ID;
    // 绑定的纹理
	std::unique_ptr<Texture2D> RenderTexture;

    // 返回绑定纹理的ID
	inline GLuint GetRenderTextureID() { return this->RenderTexture->ID; }
    // 使用帧缓冲
	inline void UseFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, this->ID); }

    //@brief FrameBuffer
    //@param width 宽度
    //@param height 高度
    //@param format 格式 GL_RGBA GL_RGB
    //@param isDepthOn 是否开启深度和模板
    //@param rboFormat 缓冲对象附件格式
    //
    //@return 
	FrameBuffer(GLuint width, GLuint height, GLenum format, bool isDepthOn = true, GLenum rboFormat = GL_DEPTH24_STENCIL8);
	~FrameBuffer();
private:
    // 渲染缓冲对象附件ID
	GLuint renderBufferObjectID;
};


//unsigned int fbo;
//glGenFramebuffers(1, &fbo);
//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
////unsigned int textureColorbuffer;
////glGenTextures(1, &textureColorbuffer);
////glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//auto texture = Texture2D::CreateTexture2D(ScreenWidth, ScreenHeight, GL_RGB, GL_CLAMP_TO_EDGE);
//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->ID, 0);
//
//unsigned int rbo;
//glGenRenderbuffers(1, &rbo);
//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight);
//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//
//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//glBindRenderbuffer(GL_RENDERBUFFER, 0);