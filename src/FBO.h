#pragma once

#include <glad/glad.h>
#include <iostream>

// Simple OOP class encapsulation for OpenGL FBO
class FBO
{
public:
	GLuint ID;

	FBO();
	~FBO();
	void activate();
	void deactivate();
	void remove();
	void setTexture(GLuint ID, int type);	
	void setRenderBuffer(GLuint ID);
	void setDraw(GLuint inst);
	void setRead(GLuint inst);
	void checkStatus();
	void attatchRenderBuffer(GLenum target, GLenum attachment, GLuint renderbuffer);
};

