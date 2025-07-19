#pragma once

#include <cstddef>

#include <glad/glad.h>
#include <glm/glm.hpp>

// Simple OOP class encapsulation for OpenGL UBO
class UBO
{
public:
	GLuint ID;

	UBO();
	UBO(const GLsizei range);
	~UBO();
	void activate() const;
	void deactivate() const;
	void bindRange(const GLsizei range) const;
	void remove() const;
	void activateSubData(const GLsizei offset, const GLsizei size, const glm::f32* data) const;
};

