#include "RBO.h"

RBO::RBO()
{
	glGenRenderbuffers(1, &ID);
}

RBO::~RBO()
{
}

void RBO::activate()
{
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
}

void RBO::deactivate()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::remove()
{
	glDeleteRenderbuffers(1, &ID);
}

void RBO::assignSize(GLenum format, GLsizei width, GLsizei height)
{
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

void RBO::assignSize(GLenum format, GLsizei size)
{
	glRenderbufferStorage(GL_RENDERBUFFER, format, size, size);
}
