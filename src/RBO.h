#pragma once

#include <glad/glad.h>

// Simple OOP class encapsulation for OpenGL RBO
class RBO
{
public:
	GLuint ID;

	RBO();
	~RBO();
	void activate();
	void deactivate();
	void remove();
	void assignSize(GLenum format, GLsizei width, GLsizei height);
	void assignSize(GLenum format, GLsizei size);
};

