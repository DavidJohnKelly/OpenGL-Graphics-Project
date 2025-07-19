#pragma once

#include "Vertex.h"

#include <glad/glad.h>
#include <stdint.h>

// Simple OOP class encapsulation for OpenGL VAO
class VAO
{
public:
	GLuint ID;

	VAO();
	~VAO();
	void activate() const;
	void deactivate() const;
	void remove() const;
	void activateAttribute(const GLuint index, const GLuint size, const GLsizei stride, const GLsizei offset) const;
	void activateVertexAttributes();
	void draw(const GLuint count) const;
	void draw(const size_t size) const;
};

