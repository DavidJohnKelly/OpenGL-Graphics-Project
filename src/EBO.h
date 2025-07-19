#pragma once

#include <glad/glad.h>
#include <vector>

// Simple OOP class encapsulation for OpenGL EBO
class EBO
{
public:
	GLuint ID;

	EBO();
	~EBO();
	void activate() const;
	void deactivate() const;
	void remove() const;
	void initialiseIndices(const std::vector<GLuint>& indices) const;
	void initialiseIndices(const GLsizei size, const void* data) const;
};

