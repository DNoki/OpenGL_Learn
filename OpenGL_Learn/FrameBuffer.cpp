#include "FrameBuffer.h"

#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(GLuint width, GLuint height, GLenum format, bool isDepthOn, GLenum rboFormat)
{
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// ������
	auto tex = Texture2D::CreateTexture2D(width, height, format, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->ID, 0);

	// ��Ⱦ������󸽼�
	unsigned int rbo = 0;
	if (isDepthOn)
	{
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, rboFormat, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}

	// ���֡�����Ƿ�����
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	this->ID = fbo;
	this->RenderTexture = move(tex);
	this->renderBufferObjectID = rbo;
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &(this->ID));
}