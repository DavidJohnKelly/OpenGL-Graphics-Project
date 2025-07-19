#include "FBO.h"

FBO::FBO()
{
	glGenFramebuffers(1, &ID);
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &ID);
}

void FBO::activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::deactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::remove()
{
	glDeleteFramebuffers(1, &ID);
}

void FBO::setTexture(GLuint ID, int type)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, ID, 0);
}

void FBO::setRenderBuffer(GLuint ID)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ID);
}

void FBO::setDraw(GLuint inst)
{
	glDrawBuffer(inst);
}

void FBO::setRead(GLuint inst)
{
	glReadBuffer(inst);
}

void FBO::checkStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}

void FBO::attatchRenderBuffer(GLenum target, GLenum attachment, GLuint renderbuffer)
{
	glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, renderbuffer);
}
